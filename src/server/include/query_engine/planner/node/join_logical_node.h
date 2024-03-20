#pragma once

#include "logical_node.h"

class JoinLogicalNode : public LogicalNode {
 public:
  JoinLogicalNode() = default;
  ~JoinLogicalNode() override = default;

  LogicalNodeType type() const override { return LogicalNodeType::JOIN; }

 private:
};
