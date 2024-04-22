#include "include/query_engine/planner/node/group_by_logical_node.h"

// TODO 添加你自己的GroupBy节点实现
GroupByLogicalNode::GroupByLogicalNode(
    const std::vector<Expression *> &field_exprs,
    const std::vector<AggrExpr *> &aggr_exprs)
    : AggrLogicalNode(aggr_exprs) {}