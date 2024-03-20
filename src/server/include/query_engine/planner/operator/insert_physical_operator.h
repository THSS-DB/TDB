#pragma once

#include <vector>

#include "physical_operator.h"

class InsertStmt;

/**
 * @brief 插入算子，用于执行数据的插入操作
 * @ingroup PhysicalOperator
 */
class InsertPhysicalOperator : public PhysicalOperator {
 public:
  InsertPhysicalOperator(Table *table,
                         std::vector<std::vector<Value>> &&multi_values);

  virtual ~InsertPhysicalOperator() = default;

  PhysicalOperatorType type() const override {
    return PhysicalOperatorType::INSERT;
  }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  Tuple *current_tuple() override { return nullptr; }

 private:
  Table *table_ = nullptr;
  std::vector<std::vector<Value>> multi_values_;
};
