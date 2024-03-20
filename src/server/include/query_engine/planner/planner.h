#pragma once

#include <memory>
#include "logical_plan_generator.h"
#include "include/query_engine/optimizer/optimizer.h"
#include "include/query_engine/planner/operator/physical_operator_generator.h"
#include "include/query_engine/structor/query_info.h"
#include "include/query_engine/planner/node/logical_node.h"

class Planner{
public:
    RC plan_logical_tree(QueryInfo *query_info, std::unique_ptr<LogicalNode> &logical_operator);
    RC plan_physical_operator(std::unique_ptr<LogicalNode> &logical_operator, QueryInfo *query_info);
private:
    LogicalPlanGenerator logical_plan_generator_;
    PhysicalOperatorGenerator physical_plan_generator_;
};