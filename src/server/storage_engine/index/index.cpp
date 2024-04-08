#include "include/storage_engine/index/index.h"
#include "include/storage_engine/recorder/table.h"

RC Index::init(const IndexMeta &index_meta, const std::vector<FieldMeta> &multi_field_metas)
{
  index_meta_ = index_meta;
  multi_field_metas_ = multi_field_metas;
  return  RC::SUCCESS;
}