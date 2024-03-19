#include <cstring>
#include <sstream>

#include "gtest/gtest.h"
#include "include/storage_engine/buffer/buffer_pool.h"
#include "include/storage_engine/recorder/record.h"
#include "include/storage_engine/recorder/record_manager.h"

TEST(test_buffer, test_buffer_pool)
{
  const char *data_file = "test_buffer_pool.data";
  ::remove(data_file);
  BufferPoolManager *bpm = new BufferPoolManager();
  FileBufferPool *bp = nullptr;
  RC rc = bpm->create_file(data_file);
  ASSERT_EQ(rc, RC::SUCCESS);
  rc = bpm->open_file(data_file, bp);
  ASSERT_EQ(rc, RC::SUCCESS);

  Frame *frame = nullptr;
  rc = bp->allocate_page(&frame);
  ASSERT_EQ(rc, RC::SUCCESS);

  const int record_size = 8;
  RecordPageHandler record_page_handle;
  rc = record_page_handle.init_empty_page(*bp, frame->page_num(), record_size);
  ASSERT_EQ(rc, RC::SUCCESS);

  char data_buf[record_size] = "1234567";
  RID rid;
  Record record;
  for (int i = 0; i < 10; i++) {
    rid.page_num = i;
    rid.slot_num = i;
    rc = record_page_handle.insert_record(data_buf, &rid);
    ASSERT_EQ(rc, RC::SUCCESS);
  }

  RecordPageIterator iterator;
  iterator.init(record_page_handle);
  int count = 0;
  while (iterator.has_next()) {
    count++;
    rc = iterator.next(record);
    ASSERT_EQ(rc, RC::SUCCESS);
  }
  ASSERT_EQ(count, 10);

  bp->close_file();
  delete bpm;
}

int main(int argc, char **argv)
{
  // 分析gtest程序的命令行参数
  testing::InitGoogleTest(&argc, argv);

  // 调用RUN_ALL_TESTS()运行所有测试用例
  // main函数返回RUN_ALL_TESTS()的运行结果
  return RUN_ALL_TESTS();
}