#pragma once

#include "physical_operator.h"

class Trx;
class UpdateStmt;

/**
 * @brief 物理算子，更新
 * @ingroup PhysicalOperator
 */
class UpdatePhysicalOperator : public PhysicalOperator
{
public:
  UpdatePhysicalOperator(Table *table, std::vector<UpdateUnit> update_units) : table_(table), update_units_(std::move(update_units))
  {}

  virtual ~UpdatePhysicalOperator();

  PhysicalOperatorType type() const override
  {
    return PhysicalOperatorType::UPDATE;
  }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  Tuple *current_tuple() override
  {
    return nullptr;
  }
  std::vector<UpdateUnit> update_units() {
    return update_units_;
  }

private:
  Table *table_ = nullptr;
  Trx *trx_ = nullptr;
  std::vector<UpdateUnit> update_units_;
};