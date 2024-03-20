#pragma once

#include "physical_operator.h"

class Trx;
class DeleteStmt;

/**
 * @brief 物理算子，删除
 * @ingroup PhysicalOperator
 */
class DeletePhysicalOperator : public PhysicalOperator {
 public:
  DeletePhysicalOperator(Table *table) : table_(table) { isdelete_ = true; }

  virtual ~DeletePhysicalOperator() = default;

  PhysicalOperatorType type() const override {
    return PhysicalOperatorType::DELETE;
  }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  Tuple *current_tuple() override { return nullptr; }

 private:
  Table *table_ = nullptr;
  Trx *trx_ = nullptr;
};
