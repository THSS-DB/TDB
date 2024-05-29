#include "include/storage_engine/recover/log_file.h"

using namespace std;
using namespace common;

static const int LOG_BUFFER_SIZE = 4 * 1024 * 1024;
static const char *LOG_FILE_NAME = "redo.log";

RC LogBuffer::append_log_entry(LogEntry *log_entry)
{
  if (nullptr == log_entry) {
    return RC::INVALID_ARGUMENT;
  }

  // total_size_ 的计算没有考虑日志头
  if (total_size_ + log_entry->log_entry_len() >= LOG_BUFFER_SIZE) {
    return RC::LOGBUF_FULL;
  }

  lock_guard<Mutex> lock_guard(lock_);
  log_entrys_.emplace_back(log_entry);
  total_size_ += log_entry->log_entry_len();
  LOG_DEBUG("append log. log_entry={%s}", log_entry->to_string().c_str());
  return RC::SUCCESS;
}

RC LogBuffer::write_log_entry(LogFile &log_file, LogEntry *log_entry)
{
  const LogEntryHeader &header = log_entry->header();
  RC rc = log_file.write(reinterpret_cast<const char *>(&header), sizeof(header));
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to write log record header. size=%d, rc=%s", sizeof(header), strrc(rc));
    return rc;
  }

  switch (log_entry->log_type()) {
    case LogEntryType::MTR_BEGIN:
    case LogEntryType::MTR_ROLLBACK: {
      // do nothing
    } break;

    case LogEntryType::MTR_COMMIT: {
      rc = log_file.write(reinterpret_cast<const char *>(&log_entry->commit_entry()),
                          log_entry->header().log_entry_len_);
    } break;

    default: {
      rc = log_file.write(reinterpret_cast<const char *>(&log_entry->record_entry()), RecordEntry::HEADER_SIZE);
      if (RC_FAIL(rc)) {
        LOG_WARN("failed to write record entry header. size=%d, rc=%s", RecordEntry::HEADER_SIZE, strrc(rc));
        return rc;
      }
      rc = log_file.write(log_entry->record_entry().data_, log_entry->record_entry().data_len_);
      if (RC_FAIL(rc)) {
        LOG_WARN("failed to write log data. size=%d, rc=%s", log_entry->record_entry().data_len_, strrc(rc));
        return rc;
      }
    } break;
  }

  return rc;
}

RC LogBuffer::flush_buffer(LogFile &log_file)
{
  RC rc = RC::SUCCESS;
  int count = 0;
  while (!log_entrys_.empty()) {
    lock_.lock();  // log buffer 需要支持并发，所以要考虑加锁
    if (log_entrys_.empty()) {
      lock_.unlock();
      return RC::SUCCESS;
    }

    unique_ptr<LogEntry> log_entry = std::move(log_entrys_.front());  // 从队列中取出日志记录然后写入到文件中
    log_entrys_.pop_front();

    rc = write_log_entry(log_file, log_entry.get());
    // 当前无法处理日志写不完整的情况，所以直接粗暴退出
    ASSERT(rc == RC::SUCCESS, "failed to write log record. log_record=%s, rc=%s", log_entry->to_string().c_str(), strrc(rc));

    lock_.unlock();
    total_size_ -= log_entry->log_entry_len();
    count++;
  }

  LOG_WARN("flush log buffer done. write log record number=%d", count);
  return log_file.sync();
}

////////////////////////////////////////////////////////////////////////////////

LogFile::~LogFile()
{
  if (fd_ >= 0) {
    LOG_INFO("close log file. file=%s, fd=%d", filename_.c_str(), fd_);
    ::close(fd_);
    fd_ = -1;
  }
}

RC LogFile::init(const char *path)
{
  RC rc = RC::SUCCESS;
  std::string log_file_path = std::string(path) + common::FILE_PATH_SPLIT_STR + LOG_FILE_NAME;
  int fd = ::open(log_file_path.c_str(), O_RDWR | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
  if (fd < 0) {
    rc = RC::IOERR_OPEN;
    LOG_WARN("failed to open log file. filename=%s, error=%s", log_file_path.c_str(), strerror(errno));
    return rc;
  }
  filename_ = log_file_path;
  fd_ = fd;
  LOG_INFO("open log file success. file=%s, fd=%d", filename_.c_str(), fd_);
  return rc;
}

RC LogFile::write(const char *data, int len)
{
  int ret = writen(fd_, data, len);
  if (0 != ret) {
    LOG_WARN("failed to write data to file. filename=%s, data len=%d, error=%s", filename_.c_str(), len, strerror(ret));
    return RC::IOERR_WRITE;
  }
  return RC::SUCCESS;
}

RC LogFile::read(char *data, int len)
{
  int ret = readn(fd_, data, len);
  if (ret != 0) {
    if (ret == -1) {
      eof_ = true;
      LOG_TRACE("file read touch eof. filename=%s", filename_.c_str());
    } else {
      LOG_WARN("failed to read data from file. file=%s, data len=%d, error=%s", filename_.c_str(), len, strerror(ret));
    }
    return RC::IOERR_READ;
  }
  return RC::SUCCESS;
}

RC LogFile::sync()
{
  int ret = fsync(fd_);
  if (ret != 0) {
    LOG_WARN("failed to sync file. file=%s, error=%s", filename_.c_str(), strerror(errno));
    return RC::IOERR_SYNC;
  }
  return RC::SUCCESS;
}

RC LogFile::offset(int64_t &off) const
{
  off_t pos = lseek(fd_, 0, SEEK_CUR);
  if (pos == -1) {
    LOG_WARN("failed to seek. error=%s", strerror(errno));
    return RC::IOERR_SEEK;
  }
  off = static_cast<int64_t>(pos);
  return RC::SUCCESS;
}