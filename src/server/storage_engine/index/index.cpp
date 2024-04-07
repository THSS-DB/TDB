#include "include/storage_engine/index/index.h"
#include "include/storage_engine/recorder/table.h"

RC Index::init(const IndexMeta &index_meta, const std::vector<FieldMeta> &multi_field_metas)
{
  index_meta_ = index_meta;
  multi_field_metas_ = multi_field_metas;
  return  RC::SUCCESS;
}
//
//RC Index::create(const char *file_name, const IndexMeta &index_meta, const std::vector<FieldMeta> &multi_field_metas)
//{
//  return RC::SUCCESS;
//}
//
//RC Index::open(const char *file_name, const IndexMeta &index_meta, const std::vector<FieldMeta> &multi_field_metas)
//{
//  return RC::SUCCESS;
//}
//
//RC Index::close()
//{
//  return RC::SUCCESS;
//}
//
//RC Index::insert_entry(const char *record, const RID *rid)
//{
//  return RC::SUCCESS;
//}
//
//RC Index::insert_entry(const char *record, const RID *rid, int sys_field_num, MvccTrx *trx)
//{
//  return RC::SUCCESS;
//}
//
//RC Index::delete_entry(const char *record, const RID *rid)
//{
//  return RC::SUCCESS;
//}
//
//RC Index::sync()
//{
//  return RC::SUCCESS;
//}
//
//IndexScanner * Index::create_scanner(const char *left_key, int left_len, bool left_inclusive, const char *right_key,
//                                     int right_len, bool right_inclusive) {
//    return nullptr;
//}