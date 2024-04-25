#pragma once

#include "aggr_logical_node.h"

class Expression;

/**
 * TODO GroupBy的逻辑节点定义, GroupBy需要在聚合指定列的基础上进行聚合运算,因此需要有相应的描述符
 */
class GroupByLogicalNode : public AggrLogicalNode
{
public:
  GroupByLogicalNode(
      const std::vector<Expression *> &field_exprs,
      const std::vector<AggrExpr *> &aggr_exprs);
  ~GroupByLogicalNode() override = default;

  LogicalNodeType type() const override {
    return LogicalNodeType::GROUP_BY;
  }
};
