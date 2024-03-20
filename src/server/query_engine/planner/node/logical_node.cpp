#include "include/query_engine/planner/node/logical_node.h"

LogicalNode::~LogicalNode()
{}

void LogicalNode::add_child(std::unique_ptr<LogicalNode> oper)
{
  children_.emplace_back(std::move(oper));
}
