#pragma once

#include "physical_operator.h"
#include "include/storage_engine/recorder/record_manager.h"
#include "include/common/rc.h"
#include "include/query_engine/structor/tuple/row_tuple.h"

class Table;

/**
 * @brief 表扫描物理算子
 * @ingroup PhysicalOperator
 */
class TableScanPhysicalOperator : public PhysicalOperator
{
public:
  TableScanPhysicalOperator(Table *table, const std::string &table_alias, bool readonly)
      : table_(table), table_alias_(table_alias), readonly_(readonly)
  {}

  virtual ~TableScanPhysicalOperator() = default;

  std::string param() const override;

  PhysicalOperatorType type() const override
  {
    return PhysicalOperatorType::TABLE_SCAN;
  }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  Tuple *current_tuple() override;

  void set_predicates(std::vector<std::unique_ptr<Expression>> &&exprs);

private:
  RC filter(RowTuple &tuple, bool &result);

private:
  Table *                                  table_ = nullptr;
  std::string                              table_alias_;
  Trx *                                    trx_ = nullptr;
  bool                                     readonly_ = false;
  RecordFileScanner                        record_scanner_;
  Record                                   current_record_;
  RowTuple                                 tuple_;
  std::vector<std::unique_ptr<Expression>> predicates_; // TODO chang predicate to table tuple filter
};
