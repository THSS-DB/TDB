#pragma once

#include <memory>

#include "include/query_engine/analyzer/statement/orderby_stmt.h"
#include "include/query_engine/structor/expression/expression.h"
#include "physical_operator.h"

class OrderByStmt;

/**
 * @brief 排序物理算子
 * @ingroup PhysicalOperator
 */
class OrderPhysicalOperator : public PhysicalOperator {
 public:
  OrderPhysicalOperator(std::vector<OrderByUnit *> order_units);

  virtual ~OrderPhysicalOperator() = default;

  PhysicalOperatorType type() const override {
    return PhysicalOperatorType::ORDER_BY;
  }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  Tuple *current_tuple() override;

 private:
  RC sort_table();

 private:
  std::vector<OrderByUnit *> order_units_;
  bool is_init_ = true;
  std::vector<std::vector<Record *>> st_;  // sort table
  std::vector<int> st_idx_;
  std::vector<int>::iterator it_;
};
