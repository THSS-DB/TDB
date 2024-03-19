#include "include/common/rc.h"
#include "include/storage_engine/buffer/frame.h"
#include "include/storage_engine/buffer/frame_manager.h"
#include "gtest/gtest.h"


TEST(test_buffer, test_frame_manager)
{
  FrameManager frame_manager("Test");
  frame_manager.init(2);

  std::list<Frame *> used_list;

  /**
   * 测试1：FrameManager的alloc方法
   */
  const int file_desc = 0;
  int size = 0;
  Frame *frame_alloc;
  for ( ; true; size++) {
    frame_alloc = frame_manager.alloc(file_desc, size);
    if (frame_alloc != nullptr) {
      frame_alloc->set_file_desc(file_desc);
      frame_alloc->set_page_num(size);
      used_list.push_back(frame_alloc);
    } else {
      break;
    }
  }
  ASSERT_EQ(used_list.size(), frame_manager.frame_num());

  /**
   * 测试2：FrameManager的get方法
   */
  Frame *frame_get = frame_manager.get(file_desc, size - 1);
  ASSERT_EQ(frame_get, used_list.back());

  /**
   * 测试3：FrameManager的purge_frames方法
   */
  Frame *frame_fail = frame_manager.alloc(file_desc, ++size);  // 分配失败
  ASSERT_EQ(frame_fail, nullptr);

  auto evict_action = [&](Frame *frame) { return RC::SUCCESS; };
  frame_manager.evict_frames(3, evict_action);  // 驱逐3个Frame
  ASSERT_EQ(used_list.size() - 3, frame_manager.frame_num());

  Frame *item2 = frame_manager.alloc(file_desc, size);  // 分配成功
  ASSERT_NE(item2, nullptr);

  frame_manager.cleanup();
}

int main(int argc, char **argv)
{
  // 分析gtest程序的命令行参数
  testing::InitGoogleTest(&argc, argv);

  // 调用RUN_ALL_TESTS()运行所有测试用例
  // main函数返回RUN_ALL_TESTS()的运行结果
  return RUN_ALL_TESTS();
}