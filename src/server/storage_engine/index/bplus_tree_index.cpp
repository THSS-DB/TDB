/*
 * TODO: 需要同学们实现这个类的方法
 */

#include "include/storage_engine/index/bplus_tree_index.h"

BplusTreeIndex::~BplusTreeIndex() noexcept
{
  close();
}

RC BplusTreeIndex::create(const char *file_name, const IndexMeta &index_meta, const std::vector<FieldMeta> &multi_field_metas)
{
  return RC::SUCCESS;
}

RC BplusTreeIndex::open(const char *file_name, const IndexMeta &index_meta, const std::vector<FieldMeta> &multi_field_metas)
{
  return RC::SUCCESS;
}

RC BplusTreeIndex::close()
{
  return RC::SUCCESS;
}

RC BplusTreeIndex::insert_entry(const char *record, const RID *rid)
{
  return RC::SUCCESS;
}

RC BplusTreeIndex::insert_entry(const char *record, const RID *rid, int sys_field_num, MvccTrx *trx)
{
  return RC::SUCCESS;
}

RC BplusTreeIndex::delete_entry(const char *record, const RID *rid)
{
  return RC::SUCCESS;
}

IndexScanner *BplusTreeIndex::create_scanner(
    const char *left_key, int left_len, bool left_inclusive, const char *right_key, int right_len, bool right_inclusive)
{
  return nullptr;
}

RC BplusTreeIndex::sync()
{
  return RC::SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////

BplusTreeIndexScanner::BplusTreeIndexScanner(BplusTreeHandler &tree_handler) : tree_scanner_(tree_handler)
{}

BplusTreeIndexScanner::~BplusTreeIndexScanner() noexcept
{
  tree_scanner_.close();
}

RC BplusTreeIndexScanner::open(
    const char *left_key, int left_len, bool left_inclusive, const char *right_key, int right_len, bool right_inclusive)
{
  return tree_scanner_.open(left_key, left_len, left_inclusive, right_key, right_len, right_inclusive);
}

RC BplusTreeIndexScanner::next_entry(RID *rid, bool isdelete)
{
  return tree_scanner_.next_entry(*rid, isdelete);
}

RC BplusTreeIndexScanner::destroy()
{
  delete this;
  return RC::SUCCESS;
}