#include "include/query_engine/planner/node/aggr_logical_node.h"

#include "include/query_engine/structor/expression/aggregation_expression.h"

AggrLogicalNode::AggrLogicalNode(
    const std::vector<AggrExpr *> &aggr_expressions) {
  for (auto *aggr_expr : aggr_expressions) {
    alias_.emplace_back(aggr_expr->name());
    aggr_types_.emplace_back(aggr_expr->_aggr_type_());
    auto *field_expr = dynamic_cast<FieldExpr *>(aggr_expr->_expr_().get());
    aggr_fields_.emplace_back(field_expr->field());
  }
}
