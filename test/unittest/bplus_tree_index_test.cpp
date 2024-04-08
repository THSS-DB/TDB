#include "include/common/rc.h"
#include "include/storage_engine/index/bplus_tree_index.h"
#include "gtest/gtest.h"

/**
 * 假设table的元数据为(id int, name char, score float);
 */


/**
 * 虽然支持在多列字段上创建索引，但该测试样例仅仅在单一字段上面插入和删除索引
 */
TEST(test_bplus_tree_index, single_attribute)
{
  RC rc = RC::SUCCESS;

  /* 1. 创建index */
  std::vector<const FieldMeta *> multi_field_metas;
  FieldMeta id_meta;
  id_meta.init("id", AttrType::INTS, 0, 4, true, true);
  multi_field_metas.emplace_back(&id_meta);
  bool is_unique = false;
  const char* index_name = "i_id";
  IndexMeta new_index_meta;
  new_index_meta.init(is_unique, index_name, multi_field_metas);
  BplusTreeIndex *index = new BplusTreeIndex();
  const char* index_file = "table1-i_id.index";
  ::remove(index_file);
  std::vector<FieldMeta> new_multi_field_metas;
  for (int i = 0; i < multi_field_metas.size(); i++) {
    new_multi_field_metas.emplace_back(*(multi_field_metas[i]));
  }
  rc = index->create(index_file, new_index_meta, new_multi_field_metas);
  if (rc != RC::SUCCESS) {
    delete index;
    LOG_ERROR("Failed to create bplus tree index. file name=%s, rc=%d:%s", index_file, rc, strrc(rc));
  }

  /* 2. 插入10个索引项 */
  Record record;
  char buffer[13];
  for(int i = 0; i < 10; i++) {
    int id = 1 + i;
    char name = 'a' + i;
    double score = 3.6 + i;
    std::memcpy(buffer, &id, sizeof(int));
    std::memcpy(buffer + sizeof(int), &name, sizeof(char));
    std::memcpy(buffer + sizeof(int) + sizeof(char), &score, sizeof(double));
    record.set_rid(1, i);
    record.set_data(buffer, 13);
    rc = index->insert_entry(record.data(), &record.rid());
    ASSERT_EQ(rc, RC::SUCCESS);
  }

  /* 3. 查看B+Tree */
  BplusTreeHandler &index_handler = index->get_index_handler();
  printf("After insert ten items, the B+Tree looks like: \n");
  index_handler.print_tree();
  printf("\n");

  /* 4. 查看最后一个索引项的插入结果 */
  int multi_keys_amount = new_multi_field_metas.size();
  const char * multi_keys[multi_keys_amount];
  for (int i = 0; i < multi_keys_amount; i++) {
    multi_keys[i] = record.data() + new_multi_field_metas[i].offset();
  }
  std::list<RID> find_rids;
  rc = index_handler.get_entry(multi_keys, find_rids, new_multi_field_metas.size());
  ASSERT_EQ(rc, RC::SUCCESS);
  ASSERT_NE(find_rids.size(), 0);
  RID result_rid = find_rids.back();
  ASSERT_EQ(result_rid.page_num, 1);
  ASSERT_EQ(result_rid.slot_num, 9);

  /* 5. 删除最后一个索引项 */
  rc = index->delete_entry(record.data(), &record.rid());
  ASSERT_EQ(rc, RC::SUCCESS);

  /* 6. 查看删除结果 */
  printf("After delete the last items, the B+Tree looks like: \n");
  index_handler.print_tree();
  printf("\n");
  find_rids.clear();
  rc = index_handler.get_entry(multi_keys, find_rids, new_multi_field_metas.size());
  ASSERT_EQ(rc, RC::SUCCESS);
  ASSERT_EQ(find_rids.size(), 0);

  /* 7. 关闭资源 */
  index_handler.sync();
  delete index;
}

int main(int argc, char **argv)
{
  // 分析gtest程序的命令行参数
  testing::InitGoogleTest(&argc, argv);

  BufferPoolManager* buffer_pool_manager_ = new BufferPoolManager();
  BufferPoolManager::set_instance(buffer_pool_manager_);

  // 调用RUN_ALL_TESTS()运行所有测试用例
  // main函数返回RUN_ALL_TESTS()的运行结果
  return RUN_ALL_TESTS();
}
