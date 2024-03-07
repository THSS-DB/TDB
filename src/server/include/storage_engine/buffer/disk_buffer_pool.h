#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <string>
#include <mutex>
#include <unordered_map>
#include <functional>
#include <errno.h>

#include "include/common/rc.h"
#include "include/storage_engine/buffer/frame.h"
#include "common/lang/mutex.h"
#include "common/mm/mem_pool.h"
#include "common/lang/lru_cache.h"
#include "common/lang/bitmap.h"
#include "common/lang/mutex.h"
#include "common/log/log.h"
#include "common/io/io.h"

class BufferPoolManager;
class FileBufferPool;

/**
 * @brief BufferPool 的实现
 * @defgroup BufferPool
 */

/**
 * @brief 文件第一个页面，存放一些元数据信息，包括了后面每页的分配信息。
 * TODO 1. 当前的做法，只能分配比较少的页面，你可以扩展来支持更多的页面或无限多的页面吗？可以参考Linux ext(n)和Windows NTFS等文件系统
 *      2. 当前使用bitmap存放页面分配情况，但是这种方法在页面非常多的时候，查找空闲页面的效率非常低，你有办法优化吗？
 */
struct FileHeader 
{
  int32_t page_count;       // 当前文件一共有多少个页面
  int32_t allocated_pages;  // 已经分配了多少个页面
  char bitmap[0];           // 页面分配位图, 第0个页面(就是当前页面)，总是1

  // 能够分配的最大的页面个数，即bitmap的字节数 乘以8
  static const int MAX_PAGE_NUM = (BP_PAGE_DATA_SIZE - sizeof(page_count) - sizeof(allocated_pages)) * 8;

  std::string to_string() const;
};

/**
 * @brief 管理页帧Frame
 * @details 管理内存中的页帧。内存是有限的，内存中能够存放的页帧个数也是有限的。
 * 当内存中的页帧不够用时，需要从内存中淘汰一些页帧，以便为新的页帧腾出空间。
 * 这个管理器负责为所有的BufferPool提供页帧管理服务，也就是所有的磁盘文件在访问时都使用这个管理器映射到内存。
 */
class FrameManager 
{
public:
  FrameManager(const char *tag);

  /**
   * @brief 初始化FrameManager
   * @param pool_num 指定FrameManager的内存池数量
   */
  RC init(int pool_num);

  /**
   * @brief 清理所有的frame
   * @details 一般在关闭数据库时调用
   */
  RC cleanup();
  /**
   * @brief 分配一个新的页面：先从LRUCache中找，如果找到就直接返回；如果没找到再用Allocator分配。
   * @param file_desc 文件描述符
   * @param page_num 页面编号
   * @return Frame* 页帧指针
   */
  Frame *alloc(int file_desc, PageNum page_num);

  /**
   * @brief 从LRUCache中获取指定的页面
   * @param file_desc 文件描述符，也可以当做buffer pool文件的标识
   * @param page_num  页面号
   * @return Frame* 页帧指针, 如果没有找到，返回nullptr
   */
  Frame *get(int file_desc, PageNum page_num);

  /**
   * 尽管frame中已经包含了file_desc和page_num，但是依然要求传入，
   * 因为frame可能忘记初始化或者没有初始化
   */
  RC free(int file_desc, PageNum page_num, Frame *frame);

  /**
   * @brief 列出所有指定文件的页面
   * @param file_desc 文件描述符
   * @return std::list<Frame *> 页帧列表
   */
  std::list<Frame *> find_list(int file_desc);

  /**
   * 如果不能从空闲链表中分配新的页面，就使用这个接口，
   * 尝试从pin count=0的页面中淘汰一些
   * @param count 想要purge多少个页面
   * @param purger 需要在释放frame之前，对页面做些什么操作。当前是刷新脏数据到磁盘
   * @return 返回本次清理了多少个页面
   */
  int purge_frames(int count, std::function<RC(Frame *frame)> purger);

private:
  Frame *get_internal(const FrameId &frame_id);

private:
  class FrameIdHasher {
  public:
    size_t operator()(const FrameId &frame_id) const
    {
      return frame_id.hash();
    }
  };

  using FrameLruCache = common::LruCache<FrameId, Frame *, FrameIdHasher>;
  using FrameAllocator = common::MemPoolSimple<Frame>;

  std::mutex lock_;  // 对frame_进行操作时需要加锁
  FrameLruCache  frames_;  // 用于存放Frame，但内存有限，所以不可能存放所有的Frame
  FrameAllocator allocator_;  // 用于分配新的Frame，当LRUCache中的Frame不够用时，就从这里分配
};

/**
 * @brief 用于遍历BufferPool中的所有页面
 */
class BufferPoolIterator
{
public:
  BufferPoolIterator();
  ~BufferPoolIterator();

  RC init(FileBufferPool &bp, PageNum start_page = 0);
  bool has_next();
  PageNum next();
  RC reset();

private:
  common::Bitmap bitmap_;
  PageNum current_page_num_ = -1;
};

/**
 * @brief BufferPool的实现，负责实际与磁盘交互
 * 每个 FileBufferPool 对象对应一个物理文件
 */
class FileBufferPool 
{
public:
  FileBufferPool(BufferPoolManager &bp_manager, FrameManager &frame_manager);
  ~FileBufferPool();

  /**
   * 根据文件名打开一个分页文件
   */
  RC open_file(const char *file_name);

  /**
   * 关闭分页文件
   */
  RC close_file();

  /**
   * 根据文件ID和页号获取指定页面到缓冲区，返回页面句柄指针。
   */
  RC get_this_page(PageNum page_num, Frame **frame);

  /**
   * 在指定文件中分配一个新的页面，并将其放入缓冲区，返回页面句柄指针。
   * 分配页面时，如果文件中有空闲页，就直接分配一个空闲页；
   * 如果文件中没有空闲页，则扩展文件规模来增加新的空闲页。
   */
  RC allocate_page(Frame **frame);

  /**
   * @brief 释放某个页面，将此页面设置为未分配状态
   * 
   * @param page_num 待释放的页面
   */
  RC dispose_page(PageNum page_num);

  /**
   * @brief 释放一个frame
   * 如果已经脏，则刷到磁盘，除了pinned page
   */
  RC purge_page(PageNum page_num);
  RC purge_all_pages();

  /**
   * @brief 用于解除pageHandle对应页面的驻留缓冲区限制
   * 
   * 在调用GetThisPage或AllocatePage函数将一个页面读入缓冲区后，
   * 该页面被设置为驻留缓冲区状态，以防止其在处理过程中被置换出去，
   * 因此在该页面使用完之后应调用此函数解除该限制，使得该页面此后可以正常地被淘汰出缓冲区
   */
  RC unpin_page(Frame *frame);

  int file_desc() const;

  /**
   * 将dirty frame中的数据刷新到磁盘上
   */
  RC flush_page(Frame &frame);

  /**
   * 回放日志时处理page0中已被认定为不存在的page
   */
  RC recover_page(PageNum page_num);

protected:
  RC allocate_frame(PageNum page_num, Frame **buf);
  /**
   * 加载指定页面的数据到内存的Frame中
   */
  RC load_page(PageNum page_num, Frame *frame);

private:
  BufferPoolManager &  bp_manager_;
  FrameManager &     frame_manager_;

  std::string          file_name_;
  int                  file_desc_ = -1;
  Frame *              hdr_frame_ = nullptr;  // 文件头所在的frame
  FileHeader *       file_header_ = nullptr;  // 文件头
  std::set<PageNum>    disposed_pages_;  // 已经释放的页面

  common::Mutex        lock_;
private:
  friend class BufferPoolIterator;
};

/**
 * @brief BufferPool的管理类，对上层可见的接口
 */
class BufferPoolManager
{
public:
  BufferPoolManager(int memory_size = 0);
  ~BufferPoolManager();

  RC create_file(const char *file_name);
  RC open_file(const char *file_name, FileBufferPool *&bp);
  RC close_file(const char *file_name);

  RC flush_page(Frame &frame);

public:
  static void set_instance(BufferPoolManager *bpm);
  static BufferPoolManager &instance();

private:
  FrameManager frame_manager_{"BufPool"};
  common::Mutex  lock_;
  std::unordered_map<std::string, FileBufferPool *> buffer_pools_;  // 已经打开的文件
  std::unordered_map<int, FileBufferPool *> fd_buffer_pools_;
};
