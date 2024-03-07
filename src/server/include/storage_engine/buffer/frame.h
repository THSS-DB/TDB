#pragma once

#include <pthread.h>
#include <cstring>
#include <string>
#include <mutex>
#include <set>
#include <atomic>

#include "include/storage_engine/buffer/page.h"
#include "common/log/log.h"
#include "common/lang/mutex.h"
#include "include/common/setting.h"
#include "include/session/thread_data.h"
#include "include/session/session.h"

/**
 * @brief 页帧标识符
 */
class FrameId 
{
public:
  FrameId(int file_desc, PageNum page_num);
  bool    equal_to(const FrameId &other) const;
  bool    operator==(const FrameId &other) const;
  size_t  hash() const;
  int     file_desc() const;
  PageNum page_num() const;

  friend std::string to_string(const FrameId &frame_id);
private:
  int     file_desc_;
  PageNum page_num_;
};

/**
 * @brief 页帧
 * @details 页帧是磁盘文件在内存中的表示。磁盘文件按照页面来操作，操作之前先映射到内存中，将磁盘数据读取到内存中，也就是页帧。
 * 当某个页面被淘汰时，如果有些内容曾经变更过，那么就需要将这些内容刷新到磁盘上。这里有一个dirty标识，用来标识页面是否被修改过。
 * 为了防止使用过程中页面被淘汰，使用pin count：当页面被使用，pin count会增加；当页面不再使用，pin count会减少。当pin count为0时，页面可以被淘汰。
 */
class Frame
{
public:
  ~Frame()
  {
     LOG_DEBUG("deallocate frame. this=%p, lbt=%s", this, common::lbt());
  }

  /**
   * @brief reinit 和 reset 在 MemPoolSimple 中使用
   * @details 在 MemPoolSimple 分配和释放一个Frame对象时，不会调用构造函数和析构函数 而是调用reinit和reset。
   */
  void reinit() {}
  void reset() {}
  
  void clear_page()
  {
    memset(&page_, 0, sizeof(page_));
  }

  int     file_desc() const { return file_desc_; }
  void    set_file_desc(int fd) { file_desc_ = fd; }
  Page &  page() { return page_; }
  PageNum page_num() const { return page_.page_num; }
  void    set_page_num(PageNum page_num) { page_.page_num = page_num; }
  FrameId frame_id() const { return FrameId(file_desc_, page_.page_num); }
  LSN     lsn() const { return page_.lsn; }
  void    set_lsn(LSN lsn) { page_.lsn = lsn; }

  /// 刷新访问时间
  void access();

  /**
   * @brief 标记指定页面为“脏”页。如果修改了页面的内容，则应调用此函数，
   * 以便该页面被淘汰出缓冲区时，系统将新的页面数据写入磁盘文件
   */
  void mark_dirty() { dirty_ = true; }
  void clear_dirty() { dirty_ = false; }
  bool dirty() const { return dirty_; }

  char *data() { return page_.data; }

  /**
   * @brief 判断当前页帧是否可以被淘汰
   * @details 当pin count为0时，页面可以被淘汰
   */
  bool can_purge() { return pin_count_.load() == 0; }

  /**
   * @brief 给当前页帧增加引用计数
   * @details pin通常都会加着frame manager锁来访问
   */
  void pin();

  /**
   * @brief 释放一个当前页帧的引用计数
   * 与pin对应，但是通常不会加着frame manager的锁来访问
   */
  int  unpin();

  int  pin_count() const { return pin_count_.load(); }

  friend std::string to_string(const Frame &frame);

private:
  bool              dirty_     = false;
  std::atomic<int>  pin_count_{0};
  unsigned long     acc_time_  = 0;
  int               file_desc_ = -1;
  Page              page_;
};

