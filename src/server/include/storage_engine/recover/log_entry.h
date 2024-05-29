#pragma once

#include <cstdint>
#include <string>

#include "include/storage_engine/recorder/record.h"

enum class LogEntryType
{
  ERROR,
  MTR_BEGIN,
  MTR_COMMIT,
  MTR_ROLLBACK,
  INSERT,
  DELETE
};

const char* logentry_type_name(LogEntryType type);  // log entry type 转换成字符串
int32_t logentry_type_to_integer(LogEntryType type);  // log entry type 转换成数字
LogEntryType logentry_type_from_integer(int32_t value);  // 数字转换成 log entry type

/**
 * @brief LogEntry的头部信息，每条日志项都带有它。
 */
struct LogEntryHeader
{
  int32_t trx_id_ = -1;  // 该日志项所属事务的事务id
  int32_t type_ = logentry_type_to_integer(LogEntryType::ERROR);  // 日志项类型
  int32_t log_entry_len_ = 0;  // 日志项的长度，但不包含header长度

  bool operator==(const LogEntryHeader &other) const
  {
    return trx_id_ == other.trx_id_ && type_ == other.type_ && log_entry_len_ == other.log_entry_len_;
  }

  std::string to_string() const;
};

/**
 * @brief 提交语句对应的日志项
 */
struct CommitEntry
{
  int32_t commit_xid_ = -1;  // 事务提交时的事务号

  bool operator == (const CommitEntry &other) const
  {
    return this->commit_xid_ == other.commit_xid_;
  }

  std::string to_string() const;
};

/**
 * @brief 修改数据的日志项（比如插入、删除一条数据）
 */
struct RecordEntry
{
  int32_t          table_id_ = -1;    // 操作的表
  RID              rid_;              // 操作的哪条记录
  int32_t          data_len_ = 0;     // 记录的数据长度(因为header中也包含长度信息，这个长度可以不要)
  int32_t          data_offset_ = 0;  // 操作的数据在完整记录中的偏移量
  char *           data_ = nullptr;   // 具体的数据，可能没有任何数据

  ~RecordEntry();

  bool operator==(const RecordEntry &other) const
  {
    return table_id_ == other.table_id_ &&
           rid_ == other.rid_ &&
           data_len_ == other.data_len_ &&
           data_offset_ == other.data_offset_ &&
           0 == memcmp(data_, other.data_, data_len_);
  }

  std::string to_string() const;

  const static int32_t HEADER_SIZE;  // 指RecordEntry的头长度，即不包含data_的长度
};


/**
 * @brief 表示一条日志项
 */
class LogEntry
{
public:
  LogEntry() = default;  // 通常不需要直接调用这个函数来创建一条日志，而是调用 `build_xxx`创建对象。
  ~LogEntry() {}

  /**
   * @brief 创建一个事务相关的日志项
   * @details 除了MTR_COMMIT的日志
   * @param type 日志类型
   * @param trx_id 事务id
   */
  static LogEntry *build_mtr_entry(LogEntryType type, int32_t trx_id);

  /**
   * @brief 创建一个表示事务提交的日志项
   * @param trx_id 事务id
   * @param commit_xid 事务提交时的id
   */
  static LogEntry *build_commit_entry(int32_t trx_id, int32_t commit_xid);

  /**
   * @brief 创建一个表示修改数据的日志项
   * @param type 日志项类型
   * @param trx_id 事务id
   * @param table_id 操作的表
   * @param rid 操作的哪条记录
   * @param data_len 数据的长度
   * @param data_offset 修改数据在记录中的偏移量
   * @param data 具体的数据
   */
  static LogEntry *build_record_entry(LogEntryType type, int32_t trx_id, int32_t table_id, const RID &rid, int32_t data_len, int32_t data_offset, const char *data);

  /**
   * @brief 根据二进制数据创建日志项
   * @details 通常是从日志文件中读取数据，然后调用此函数创建日志项
   * @param header 日志头信息
   * @param data   读取的剩余数据信息，长度是header.log_entry_len_
   */
  static LogEntry *build(const LogEntryHeader &header, char *data);

  int32_t  trx_id() const { return entry_header_.trx_id_; }
  LogEntryType log_type() const  { return logentry_type_from_integer(entry_header_.type_); }
  int32_t  log_entry_len() const { return entry_header_.log_entry_len_; }

  LogEntryHeader &header() { return entry_header_; }
  CommitEntry &commit_entry() { return commit_entry_; }
  RecordEntry &record_entry() { return record_entry_; }
  const LogEntryHeader &header() const { return entry_header_; }
  const CommitEntry &commit_entry() const { return commit_entry_; }
  const RecordEntry &record_entry() const { return record_entry_; }

  std::string to_string() const;

 protected:
  LogEntryHeader  entry_header_;  // 日志头信息
  RecordEntry  record_entry_;  // 如果是修改数据的日志项，此结构体生效
  CommitEntry  commit_entry_;  // 如果是事务提交的日志项，此结构体生效
};
