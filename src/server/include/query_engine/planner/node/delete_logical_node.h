#pragma once

#include "logical_node.h"

/**
 * @brief 逻辑算子，用于执行delete语句
 * @ingroup LogicalNode
 */
class DeleteLogicalNode : public LogicalNode
{
public:
  DeleteLogicalNode(Table *table);
  ~DeleteLogicalNode() override = default;

  LogicalNodeType type() const override
  {
    return LogicalNodeType::DELETE;
  }
  Table *table() const
  {
    return table_;
  }

private:
  Table *table_ = nullptr;
};
