#pragma once

#include "logical_node.h"
#include "include/query_engine/structor/expression/expression.h"
#include "include/query_engine/analyzer/statement/orderby_stmt.h"

class OrderByLogicalNode : public LogicalNode
{
public:
  explicit OrderByLogicalNode(std::vector<OrderByUnit *> order_units);
  ~OrderByLogicalNode() override = default;

  LogicalNodeType            type() const override
  {
    return LogicalNodeType::ORDER;
  }
  std::vector<OrderByUnit *> order_units() {
    return order_units_;
  }
private:
  std::vector<OrderByUnit *> order_units_;
};
