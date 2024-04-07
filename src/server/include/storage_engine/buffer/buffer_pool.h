#pragma once
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdio>
#include <cstdlib>

#include "common/lang/bitmap.h"
#include "common/lang/mutex.h"
#include "common/log/log.h"
#include "common/io/io.h"
#include "include/common/rc.h"
#include "include/storage_engine/buffer/frame_manager.h"

class BufferPoolManager;

/**
 * @brief BufferPool 的实现
 * @defgroup BufferPool
 */

/**
 * @brief BufferPool的实现，负责实际与磁盘交互
 * 每个 FileBufferPool 对象对应一个物理文件
 */
class FileBufferPool
{
public:
  FileBufferPool(BufferPoolManager &bp_manager, FrameManager &frame_manager);
  ~FileBufferPool();

  RC open_file(const char *file_name);
  RC close_file();

  /**
   * 在指定文件中分配一个新的页面，并将其放入缓冲区，返回页帧句柄指针。
   */
  RC allocate_page(Frame **frame);
  /**
   * 根据文件ID和页号获取指定页面到缓冲区，返回页面句柄指针。
   */
  RC get_this_page(PageNum page_num, Frame **frame);
  /**
   * 减少对指定页帧的引用
   */
  RC unpin_page(Frame *frame);

  /**
   * 将dirty frame中的数据刷新到磁盘上
   */
  RC flush_page(Frame &frame);

  /**
   * 驱逐frame
   */
  RC evict_page(PageNum page_num, Frame *buf);
  RC evict_all_pages();

  int file_desc() const;

  RC recover_page(PageNum page_num);

  /**
   * @brief 释放某个页面，将此页面设置为未分配状态
   * @param page_num 待释放的页面
   */
  RC dispose_page(PageNum page_num);

protected:
  RC allocate_frame(PageNum page_num, Frame **buf);
  RC flush_page_internal(Frame &frame);
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
