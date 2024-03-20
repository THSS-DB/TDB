#include "include/query_engine/planner/node/predicate_logical_node.h"

PredicateLogicalNode::PredicateLogicalNode(std::unique_ptr<Expression> expression)
{
  expressions_.emplace_back(std::move(expression));
}
