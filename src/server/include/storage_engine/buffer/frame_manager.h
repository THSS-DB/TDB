#pragma once

#include <mutex>
#include "include/common/rc.h"
#include "include/storage_engine/buffer/frame.h"
#include "common/mm/mem_pool.h"
#include "common/lang/lru_cache.h"

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
  * 当分配的frame已满时，就尝试驱逐一些pin count=0的frame
  * @param count 想要驱逐多少个frame
  * @param evict_action 需要在释放frame之前，对页面做些什么操作，应该是把脏数据刷到磁盘
  * @return 返回本次驱逐了多少个frame
  */
 int evict_frames(int count, std::function<RC(Frame *frame)> evict_action);

 /**
  * @brief 列出所有指定文件的页面
  * @param file_desc 文件描述符
  * @return std::list<Frame *> 页帧列表
  */
 std::list<Frame *> find_list(int file_desc);

 size_t frame_num() const { return frames_.count(); }

 RC free(int file_desc, PageNum page_num, Frame *frame);

private:
 Frame *get_internal(const FrameId &frame_id);
 RC free_internal(const FrameId &frame_id, Frame *frame);

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

 std::mutex lock_;  // 对frames_进行操作时需要加锁
 FrameLruCache  frames_;  // 用于存放Frame，但内存有限
 FrameAllocator allocator_;  // 用于分配新的Frame
};