#include "include/query_engine/planner/node/order_by_logical_node.h"
#include "include/query_engine/analyzer/statement/orderby_stmt.h"
OrderByLogicalNode::OrderByLogicalNode(std::vector<OrderByUnit *> order_units): order_units_(std::move(order_units))
{}
