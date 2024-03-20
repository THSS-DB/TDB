#include "include/query_engine/planner/planner.h"

#include "include/query_engine/structor/query_info.h"

RC Planner::plan_logical_tree(QueryInfo *query_info,
                              std::unique_ptr<LogicalNode> &logical_operator) {
  Stmt *stmt = query_info->stmt();
  if (stmt == nullptr) {
    return RC::UNIMPLENMENT;
  }
  RC rc = logical_plan_generator_.create(stmt, logical_operator);
  if (rc != RC::SUCCESS) {
    if (rc != RC::UNIMPLENMENT) {
      LOG_WARN("failed to create logical plan. rc=%s", strrc(rc));
    }
    return rc;
  }
  return RC::SUCCESS;
}

RC Planner::plan_physical_operator(std::unique_ptr<LogicalNode> &logical_nodes,
                                   QueryInfo *query_info) {
  RC rc;
  std::unique_ptr<PhysicalOperator> physical_operators;
  rc = physical_plan_generator_.create(*logical_nodes, physical_operators);
  if (physical_operators) {
    query_info->set_operator(std::move(physical_operators));
  }

  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create physical operator. rc=%s", strrc(rc));
  }
  return rc;
}
