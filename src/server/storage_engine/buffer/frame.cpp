#include "include/storage_engine/buffer/frame.h"

using namespace std;

FrameId::FrameId(int file_desc, PageNum page_num) : file_desc_(file_desc), page_num_(page_num) {}

bool FrameId::equal_to(const FrameId &other) const
{
  return file_desc_ == other.file_desc_ && page_num_ == other.page_num_;
}

bool FrameId::operator==(const FrameId &other) const
{
  return this->equal_to(other);
}

size_t FrameId::hash() const
{
  return (static_cast<size_t>(file_desc_) << 32L) | page_num_;
}

int FrameId::file_desc() const
{
  return file_desc_;
}

PageNum FrameId::page_num() const
{
  return page_num_;
}

string to_string(const FrameId &frame_id)
{
  stringstream ss;
  ss << "fd:" << frame_id.file_desc() << ",page_num:" << frame_id.page_num();
  return ss.str();
}

////////////////////////////////////////////////////////////////////////////////

void Frame::pin()
{
  ++pin_count_;
}

int Frame::unpin()
{
  ASSERT(pin_count_.load() > 0, "try to unpin a frame that pin count <= 0.");
  int pin_count = --pin_count_;
  return pin_count;
}

void Frame::access()
{
  struct timespec tp;
  clock_gettime(CLOCK_MONOTONIC, &tp);
  acc_time_ = tp.tv_sec * 1000 * 1000 * 1000UL + tp.tv_nsec;
}

string to_string(const Frame &frame)
{
  stringstream ss;
  ss << "frame id:" << to_string(frame.frame_id()) 
     << ", dirty=" << frame.dirty()
     << ", pin=" << frame.pin_count()
     << ", fd=" << frame.file_desc()
     << ", page num=" << frame.page_num()
     << ", lsn=" << frame.lsn();
  return ss.str();
}
