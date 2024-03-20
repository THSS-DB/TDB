#pragma once

#include <memory>

#include "include/query_engine/planner/node/logical_node.h"

class Optimizer {
 public:
  RC rewrite(std::unique_ptr<LogicalNode> &logical_operator);
};