#pragma once

#include "logical_node.h"
#include "include/query_engine/structor/expression/expression.h"

/**
 * @brief 谓词/过滤逻辑算子
 * @ingroup LogicalNode
 */
class PredicateLogicalNode : public LogicalNode
{
public:
  PredicateLogicalNode(std::unique_ptr<Expression> expression);
  ~PredicateLogicalNode() override = default;

  LogicalNodeType type() const override {
    return LogicalNodeType::PREDICATE;
  }
};
