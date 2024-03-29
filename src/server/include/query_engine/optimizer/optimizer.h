#pragma once

#include "include/query_engine/planner/node/logical_node.h"
#include <memory>

class Optimizer{
public:
    RC rewrite(std::unique_ptr<LogicalNode> &logical_operator);
};