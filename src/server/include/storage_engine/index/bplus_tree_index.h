#pragma once

#include "include/storage_engine/index/index.h"
#include "include/storage_engine/index/bplus_tree.h"
#include "include/storage_engine/recorder/table.h"


class BplusTreeIndex : public Index
{
 public:
  BplusTreeIndex() = default;
  virtual ~BplusTreeIndex() noexcept;

  BplusTreeIndex(Table * table) {
    table_ = table;
  }

  RC create(const char *file_name, const IndexMeta &index_meta, const std::vector<FieldMeta> &multi_field_metas);
  RC open(const char *file_name, const IndexMeta &index_meta, const std::vector<FieldMeta> &multi_field_metas);
  RC close();

  RC insert_entry(const char *record, const RID *rid) override;
  RC delete_entry(const char *record, const RID *rid) override;

  /**
   * 扫描指定范围的数据
   */
  IndexScanner *create_scanner(const char *left_key, int left_len, bool left_inclusive, const char *right_key,
                               int right_len, bool right_inclusive) override;

  RC sync() override;

  BplusTreeHandler &get_index_handler()
  {
    return index_handler_;
  }

 private:
  bool inited_ = false;
  BplusTreeHandler index_handler_;
  Table * table_;
};

/**
 * @brief B+树索引扫描器
 * @ingroup Index
 */
class BplusTreeIndexScanner : public IndexScanner
{
 public:
  BplusTreeIndexScanner(BplusTreeHandler &tree_handle);
  ~BplusTreeIndexScanner() noexcept override;

  RC next_entry(RID *rid, bool isdelete) override;
  RC destroy() override;

  RC open(const char *left_key, int left_len, bool left_inclusive, const char *right_key, int right_len,
          bool right_inclusive);

 private:
  BplusTreeScanner tree_scanner_;
};
