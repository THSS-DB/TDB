#pragma once

#include "include/storage_engine/recover/log_file.h"
#include "include/common/global_context.h"

class Db;

/**
 * @brief 日志项遍历器
 * @details 使用时先执行初始化(init)，然后多次调用next，直到valid返回false。
 */
class LogEntryIterator
{
 public:
  LogEntryIterator() = default;
  ~LogEntryIterator() = default;
  
  RC init(LogFile &log_file);
  bool valid() const;
  RC next();
  const LogEntry &log_entry();

 private:
  LogFile *log_file_ = nullptr;
  LogEntry *log_entry_ = nullptr;
};

/**
 * @brief 日志管理器
 * @details 一个日志管理器属于某一个DB（当前仅有一个DB sys）。
 * 管理器负责写日志（运行时）、读日志与恢复（启动时）
 */
class LogManager
{
public:
  LogManager() = default;
  ~LogManager();

  /**
   * @brief 初始化
   * @param path 日志文件所在的目录
   */
  RC init(const char *path);

  /**
   * @brief 开启一个事务
   */
  RC append_begin_trx_log(int32_t trx_id);
  /**
   * @brief 回滚一个事务
   */
  RC append_rollback_trx_log(int32_t trx_id);
  /**
   * @brief 提交一个事务
   */
  RC append_commit_trx_log(int32_t trx_id, int32_t commit_xid);

  /**
   * @brief 新增一条数据更新的日志
   */
  RC append_record_log(LogEntryType type, int32_t trx_id, int32_t table_id, const RID &rid, int32_t data_len, int32_t data_offset, const char *data);
  /**
   * @brief 也可以调用这个函数直接增加一条日志
   */
  RC append_log(LogEntry *log_entry);

  /**
   * @brief 刷新日志到磁盘
   */
  RC sync();

  /**
   * @brief 重做
   * @details 当前会重做所有日志。也就是说，所有buffer pool页面都不会写入到磁盘中，
   * 否则可能无法恢复成功。
   */
  RC recover(Db *db);
private:
  LogBuffer *log_buffer_ = nullptr;  // 日志缓存。新增日志时先放到这个buffer中
  LogFile *log_file_ = nullptr;  // 管理日志，比如读写日志
};