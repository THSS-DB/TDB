#pragma once

#include <memory>

#include "include/common/rc.h"

class Stmt;
class SelectStmt;
class FilterStmt;
class InsertStmt;
class DeleteStmt;
class UpdateStmt;
class ExplainStmt;
class LogicalNode;

class LogicalPlanGenerator {
 public:
  LogicalPlanGenerator() = default;
  virtual ~LogicalPlanGenerator() = default;

  RC create(Stmt *stmt, std::unique_ptr<LogicalNode> &logical_operator);

 private:
  RC plan_node(SelectStmt *select_stmt,
               std::unique_ptr<LogicalNode> &logical_operator);
  RC plan_node(FilterStmt *filter_stmt,
               std::unique_ptr<LogicalNode> &logical_operator);
  RC plan_node(InsertStmt *insert_stmt,
               std::unique_ptr<LogicalNode> &logical_operator);
  RC plan_node(DeleteStmt *delete_stmt,
               std::unique_ptr<LogicalNode> &logical_operator);
  RC plan_node(UpdateStmt *update_stmt,
               std::unique_ptr<LogicalNode> &logical_operator);
  RC plan_node(ExplainStmt *explain_stmt,
               std::unique_ptr<LogicalNode> &logical_operator);
};