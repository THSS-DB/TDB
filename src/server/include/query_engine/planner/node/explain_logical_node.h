#pragma once

#include "logical_node.h"

/**
 * @brief Explain逻辑算子
 * @ingroup LogicalNode
 */
class ExplainLogicalNode : public LogicalNode
{
public:
  ExplainLogicalNode() = default;
  ~ExplainLogicalNode() override = default;

  LogicalNodeType type() const override
  {
    return LogicalNodeType::EXPLAIN;
  }

private:
};
