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
 * TODO [Lab1] 需要同学们实现指定页帧的释放
 */
RC FrameManager::free(int file_desc, PageNum page_num, Frame *frame)
{
  return RC::SUCCESS;
}

/**
 * TODO [Lab1] 需要同学们实现页帧驱逐，下面是部分可参考的代码，但不完整
 */
int FrameManager::evict_frames(int count, std::function<RC(Frame *frame)> evict_action)
{
  //  std::lock_guard<std::mutex> lock_guard(lock_);
  //  std::vector<Frame *> frames_can_evict;
  //  auto evict_finder = [&frames_can_evict, count](const FrameId &frame_id, Frame *const frame) {
  //    if (frame->can_evict()) {
  //      frame->pin();
  //      frames_can_evict.push_back(frame);
  //      if (frames_can_evict.size() >= static_cast<size_t>(count)) {
  //        return false;
  //      }
  //    }
  //    return true;
  //  };
  //  frames_.foreach_reverse(evict_finder);
  //  LOG_INFO("evict frames find %ld pages total", frames_can_evict.size());

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