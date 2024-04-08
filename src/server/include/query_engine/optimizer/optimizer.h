#pragma once

#include <memory>

#include "include/query_engine/planner/node/logical_node.h"
#include "rewriter.h"

class Optimizer{
public:
    RC rewrite(std::unique_ptr<LogicalNode> &logical_operator);
private:
    Rewriter rewriter_;
};