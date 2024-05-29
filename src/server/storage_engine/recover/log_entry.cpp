#include "include/storage_engine/recover/log_entry.h"

using namespace std;

int _align8(int size)
{
  return size / 8 * 8 + ((size % 8 == 0) ? 0 : 8);
}

const char* logentry_type_name(LogEntryType type)
{
  switch (type)
  {
    case LogEntryType::ERROR:        return "ERROR";
    case LogEntryType::MTR_BEGIN:    return "MTR_BEGIN";
    case LogEntryType::MTR_COMMIT:   return "MTR_COMMIT";
    case LogEntryType::MTR_ROLLBACK: return "MTR_ROLLBACK";
    case LogEntryType::INSERT:       return "INSERT";
    case LogEntryType::DELETE:       return "DELETE";
    default:                        return "unknown redo log type";
  }
}
int32_t logentry_type_to_integer(LogEntryType type)
{
  return static_cast<int32_t>(type);
}
LogEntryType logentry_type_from_integer(int32_t value)
{
  return static_cast<LogEntryType>(value);
}

////////////////////////////////////////////////////////////////////////////////

string LogEntryHeader::to_string() const
{
  stringstream ss;
  ss << "trx_id:" << trx_id_
     << ", type:" << logentry_type_name(logentry_type_from_integer(type_)) << "(" << type_ << ")"
     << ", log_entry_len:" << log_entry_len_;
  return ss.str();
}

////////////////////////////////////////////////////////////////////////////////

string CommitEntry::to_string() const
{
  stringstream ss;
  ss << "commit_xid:" << commit_xid_;
  return ss.str();
}

////////////////////////////////////////////////////////////////////////////////

const int32_t RecordEntry::HEADER_SIZE = sizeof(RecordEntry) - sizeof(RecordEntry::data_);

RecordEntry::~RecordEntry()
{
  if (data_ == nullptr) {
    delete[] data_;
  }
}
string RecordEntry::to_string() const
{
  stringstream ss;
  ss << "table_id:" << table_id_ << ", rid:{" << rid_.to_string() << "}"
     << ", len:" << data_len_ << ", offset:" << data_offset_;
  return ss.str();
}

////////////////////////////////////////////////////////////////////////////////

LogEntry *LogEntry::build_mtr_entry(LogEntryType type, int32_t trx_id)
{
  LogEntry *log_entry = new LogEntry();

  LogEntryHeader &header = log_entry->entry_header_;
  header.trx_id_ = trx_id;
  header.type_ = logentry_type_to_integer(type);

  return log_entry;
}

LogEntry *LogEntry::build_commit_entry(int32_t trx_id, int32_t commit_xid)
{
  LogEntry *log_entry = new LogEntry();

  LogEntryHeader &header = log_entry->entry_header_;
  header.trx_id_ = trx_id;
  header.type_ = logentry_type_to_integer(LogEntryType::MTR_COMMIT);
  header.log_entry_len_ = sizeof(CommitEntry);

  CommitEntry &commit_entry = log_entry->commit_entry();
  commit_entry.commit_xid_ = commit_xid;

  return log_entry;
}

LogEntry *LogEntry::build_record_entry(LogEntryType type, int32_t trx_id, int32_t table_id, const RID &rid, int32_t data_len, int32_t data_offset, const char *data)
{
  LogEntry *log_entry = new LogEntry();

  LogEntryHeader &header = log_entry->entry_header_;
  header.trx_id_ = trx_id;
  header.type_ = logentry_type_to_integer(type);
  header.log_entry_len_ = RecordEntry::HEADER_SIZE + data_len;

  RecordEntry &record_entry = log_entry->record_entry();
  record_entry.table_id_ = table_id;
  record_entry.rid_ = rid;
  record_entry.data_len_ = data_len;
  record_entry.data_offset_ = data_offset;
  if (data_len > 0) {
    record_entry.data_ = new char[data_len];
    if (nullptr == record_entry.data_) {
      delete log_entry;
      LOG_WARN("failed to allocate memory while creating log entry. memory size=%d", data_len);
      return nullptr;
    }
    memcpy(record_entry.data_, data, data_len);
  }

  return log_entry;
}

LogEntry *LogEntry::build(const LogEntryHeader &header, char *data)
{
  LogEntry *log_entry = new LogEntry();
  log_entry->entry_header_ = header;

  if (header.log_entry_len_ <= 0) {
    return log_entry;
  }
  else if (header.type_ == logentry_type_to_integer(LogEntryType::MTR_COMMIT)) {
    ASSERT(header.log_entry_len_ == sizeof(CommitEntry), "invalid length of mtr commit. expect %d, got %d", sizeof(CommitEntry), header.log_entry_len_);
    CommitEntry &commit_entry = log_entry->commit_entry();
    memcpy(reinterpret_cast<void *>(&commit_entry), data, sizeof(CommitEntry));
    LOG_DEBUG("got a commit record %s", log_entry->to_string().c_str());
  }
  else {
    RecordEntry &record_entry = log_entry->record_entry();
    memcpy(reinterpret_cast<void *>(&record_entry), data, RecordEntry::HEADER_SIZE);
    if (header.log_entry_len_ > RecordEntry::HEADER_SIZE) {
      int data_len = header.log_entry_len_ - RecordEntry::HEADER_SIZE;
      record_entry.data_ = new char[data_len];
      memcpy(record_entry.data_, data + RecordEntry::HEADER_SIZE, data_len);
    }
  }

  return log_entry;
}

string LogEntry::to_string() const
{
  if (entry_header_.log_entry_len_ <= 0) {
    return entry_header_.to_string();
  } else if (entry_header_.type_ == logentry_type_to_integer(LogEntryType::MTR_COMMIT)) {
    return entry_header_.to_string() + ", " + commit_entry().to_string();
  } else {
    return entry_header_.to_string() + ", " + record_entry().to_string();
  }
}

