#pragma once

#include <cstdint>
#include <atomic>
#include <deque>
#include <string>
#include <fcntl.h>

#include "include/storage_engine/recover/log_entry.h"
#include "common/io/io.h"
#include "common/lang/mutex.h"

class LogFile;

/**
 * @brief 缓存运行时产生的日志项
 * @details 当前的实现非常简单，没有像其它数据库一样，将日志序列化成二进制buffer，
 * 这里仅仅把日志项存储到到链表中。如果日志数量超过某个阈值，就会调用flush_buffer将日志刷新到磁盘中。
 */
class LogBuffer
{
public:
  LogBuffer() {}
  ~LogBuffer() {}

  /**
   * @brief 在缓存中增加一条日志项
   */
  RC append_log_entry(LogEntry *log_entry);

  /**
   * @brief 将当前缓存的日志项都刷新到日志文件中
   */
  RC flush_buffer(LogFile &log_file);

private:
  /**
   * @brief 将日志记录写入到日志文件中
   * @param log_file 日志文件，概念上来讲不一定是某个特定的文件
   * @param log_record 要写入的日志记录
   */
  RC write_log_entry(LogFile &log_file, LogEntry *log_entry);

private:
  common::Mutex lock_;  // 加锁支持多线程并发写入
  std::deque<std::unique_ptr<LogEntry>> log_entrys_;  // 当前等待刷盘的日志项
  std::atomic_int32_t total_size_;  // 当前缓存中的日志项的总大小
};

/**
 * @brief 读写日志文件
 * @details 这里的名字不太贴切，因为这个类希望管理所有日志文件，而不是特定的某个文件。不过当前
 * 只有一个文件，并且文件名是固定的。
 */
class LogFile
{
public:
  LogFile() = default;
  ~LogFile();

  /**
   * @brief 初始化
   * @param path 日志文件存放的路径, 会打开这个目录下叫做 log 的文件。
   */
  RC init(const char *path);

  /**
   * @brief 写入指定数据，全部写入成功返回成功，否则返回失败
   * @param data 要写的数据
   * @param len  数据的长度
   */
  RC write(const char *data, int len);

  /**
   * @brief 读取指定长度的数据。全部读取成功返回成功，否则返回失败
   * @param data 存储读出的数据
   * @param len  要读取的长度
   */
  RC read(char *data, int len);

  /**
   * @brief 将当前写的文件执行sync同步数据到磁盘
   */
  RC sync();

  /**
   * @brief 获取当前读取的文件位置
   */
  RC offset(int64_t &off) const;

  /**
   * @brief 当前是否已经读取到文件尾
   */
  bool eof() const { return eof_; }

protected:
  std::string filename_;  // 日志文件名。总是init函数参数path路径下的日志文件
  int fd_ = -1;  // 文件描述符
  bool eof_ = false;  // 是否已经读取到文件尾
};