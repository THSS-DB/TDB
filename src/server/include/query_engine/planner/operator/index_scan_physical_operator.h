#pragma once

#include "physical_operator.h"
#include "include/query_engine/structor/tuple/row_tuple.h"
#include "include/storage_engine/recorder/record_manager.h"

class IndexScanner;
/**
 * TODO [Lab2]
 * 通过索引来扫描文件,与TableScanOperator扮演同等的角色.
 * 需要实现index_scan_operator,存在索引时利用索引扫描数据
 * 同时补充physical_operator_generator逻辑,基于IndexScanNode生成IndexScanOperator
 */
class IndexScanPhysicalOperator : public PhysicalOperator
{
public:
  IndexScanPhysicalOperator(Table *table, Index *index, bool readonly,
                           const Value *left_value, bool left_inclusive,
                           const Value *right_value, bool right_inclusive) :
                                             table_(table), index_(index),
                                             readonly_(readonly),
                                             left_inclusive_(left_inclusive),
                                             right_inclusive_(right_inclusive)
 {
    if (left_value != nullptr) {
      left_value_ = *left_value;
    }
    if (right_value != nullptr) {
      right_value_ = *right_value;
    }
  }

  ~IndexScanPhysicalOperator() override = default;

  PhysicalOperatorType type() const override
  {
    return PhysicalOperatorType::INDEX_SCAN;
  }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  Tuple *current_tuple() override;

  std::string param() const override;

  void set_predicates(std::vector<std::unique_ptr<Expression>> &predicates) {
    predicates_ = std::move(predicates);
  }

 private:
  RC filter(RowTuple &tuple, bool &result);
  Table *table_ = nullptr;
  Index *index_ = nullptr;
  IndexScanner *index_scanner_ = nullptr;
  RecordFileHandler *record_handler_ = nullptr;
  bool  readonly_ = false;

  RecordPageHandler record_page_handler_;
  Record current_record_;
  RowTuple tuple_;

  Value left_value_;
  Value right_value_;
  bool left_inclusive_ = false;
  bool right_inclusive_ = false;
  std::vector<std::unique_ptr<Expression>> predicates_;
};
