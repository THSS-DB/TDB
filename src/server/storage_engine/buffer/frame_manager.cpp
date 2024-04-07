#include "include/storage_engine/buffer/frame_manager.h"

FrameManager::FrameManager(const char *tag) : allocator_(tag)
{}

RC FrameManager::init(int pool_num)
{
  int ret = allocator_.init(false, pool_num);
  if (ret == 0) {
    return RC::SUCCESS;
  }
  return RC::NOMEM;
}

RC FrameManager::cleanup()
{
  if (frames_.count() > 0) {
    return RC::INTERNAL;
  }
  frames_.destroy();
  return RC::SUCCESS;
}

Frame *FrameManager::alloc(int file_desc, PageNum page_num)
{
  FrameId frame_id(file_desc, page_num);
  std::lock_guard<std::mutex> lock_guard(lock_);
  Frame *frame = get_internal(frame_id);
  if (frame != nullptr) {
    return frame;
  }

  frame = allocator_.alloc();
  if (frame != nullptr) {
    ASSERT(frame->pin_count() == 0, "got an invalid frame that pin count is not 0. frame=%s",
        to_string(*frame).c_str());
    frame->set_page_num(page_num);
    frame->pin();
    frames_.put(frame_id, frame);
  }
  return frame;
}

Frame *FrameManager::get(int file_desc, PageNum page_num)
{
  FrameId frame_id(file_desc, page_num);
  std::lock_guard<std::mutex> lock_guard(lock_);
  return get_internal(frame_id);
}

/**
 * TODO [Lab1] 需要同学们实现页帧驱逐
 */
int FrameManager::evict_frames(int count, std::function<RC(Frame *frame)> evict_action)
{
  return 0;
}

Frame *FrameManager::get_internal(const FrameId &frame_id)
{
  Frame *frame = nullptr;
  (void)frames_.get(frame_id, frame);
  if (frame != nullptr) {
    frame->pin();
  }
  return frame;
}

/**
 * @brief 查找目标文件的frame
 * FramesCache中选出所有与给定文件描述符(file_desc)相匹配的Frame对象，并将它们添加到列表中
 */
std::list<Frame *> FrameManager::find_list(int file_desc)
{
  std::lock_guard<std::mutex> lock_guard(lock_);

  std::list<Frame *> frames;
  auto fetcher = [&frames, file_desc](const FrameId &frame_id, Frame *const frame) -> bool {
    if (file_desc == frame_id.file_desc()) {
      frame->pin();
      frames.push_back(frame);
    }
    return true;
  };
  frames_.foreach (fetcher);
  return frames;
}

RC FrameManager::free(int file_desc, PageNum page_num, Frame *frame)
{
  FrameId frame_id(file_desc, page_num);

  std::lock_guard<std::mutex> lock_guard(lock_);
  return free_internal(frame_id, frame);
}

RC FrameManager::free_internal(const FrameId &frame_id, Frame *frame)
{
  Frame *frame_source = nullptr;
  [[maybe_unused]] bool found = frames_.get(frame_id, frame_source);
  ASSERT(found && frame == frame_source && frame->pin_count() == 1,
         "failed to free frame. found=%d, frameId=%s, frame_source=%p, frame=%p, pinCount=%d, lbt=%s",
         found, to_string(frame_id).c_str(), frame_source, frame, frame->pin_count(), lbt());

  frame->unpin();
  frames_.remove(frame_id);
  allocator_.free(frame);
  return RC::SUCCESS;
}