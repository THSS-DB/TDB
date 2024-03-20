#include "include/storage_engine/schema/schema_util.h"

std::string table_meta_file(const char *base_dir, const char *table_name) {
  return std::string(base_dir) + common::FILE_PATH_SPLIT_STR + table_name +
         TABLE_META_SUFFIX;
}
std::string table_data_file(const char *base_dir, const char *table_name) {
  return std::string(base_dir) + common::FILE_PATH_SPLIT_STR + table_name +
         TABLE_DATA_SUFFIX;
}

std::string table_index_file(const char *base_dir, const char *table_name,
                             const char *index_name) {
  return std::string(base_dir) + common::FILE_PATH_SPLIT_STR + table_name +
         "-" + index_name + TABLE_INDEX_SUFFIX;
}
