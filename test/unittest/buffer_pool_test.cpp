#include <cstring>
#include <sstream>

#include "gtest/gtest.h"
#include "include/storage_engine/buffer/buffer_pool.h"
#include "include/storage_engine/recorder/record.h"
#include "include/storage_engine/recorder/record_manager.h"

void test1()
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
    rid.page_num = 1;
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

void test2()
{
  const char *data_file = "test_buffer_pool.data";
  BufferPoolManager *bpm = new BufferPoolManager();
  FileBufferPool *bp = nullptr;
  RC rc = bpm->open_file(data_file, bp);
  ASSERT_EQ(rc, RC::SUCCESS);

  RecordPageHandler record_page_handle;
  rc = record_page_handle.init(*bp, 1, true);
  ASSERT_EQ(rc, RC::SUCCESS);

  RecordPageIterator iterator;
  iterator.init(record_page_handle);
  int count = 0;
  Record record;
  while (iterator.has_next()) {
    count++;
    rc = iterator.next(record);
    ASSERT_EQ(rc, RC::SUCCESS);
    printf("%s\n",record.data());
  }
  ASSERT_EQ(count, 10);

  bp->close_file();
  delete bpm;
}

TEST(test_buffer, test_buffer_pool)
{
  test1();  // 创建新文件并写入数据
  test2();  // 读取该文件，检验是否持久化成功
}

int main(int argc, char **argv)
{
  // 分析gtest程序的命令行参数
  testing::InitGoogleTest(&argc, argv);

  // 调用RUN_ALL_TESTS()运行所有测试用例
  // main函数返回RUN_ALL_TESTS()的运行结果
  return RUN_ALL_TESTS();
}