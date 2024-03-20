#include "include/query_engine/analyzer/statement/explain_stmt.h"

#include "common/log/log.h"
#include "include/query_engine/analyzer/statement/stmt.h"

ExplainStmt::ExplainStmt(std::unique_ptr<Stmt> child_stmt)
    : child_stmt_(std::move(child_stmt)) {}

RC ExplainStmt::create(Db *db, const ExplainSqlNode &explain, Stmt *&stmt) {
  Stmt *child_stmt = nullptr;
  RC rc = Stmt::create_stmt(db, *explain.sql_node, child_stmt);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create explain's child stmt. rc=%s", strrc(rc));
    return rc;
  }

  std::unique_ptr<Stmt> child_stmt_ptr = std::unique_ptr<Stmt>(child_stmt);
  stmt = new ExplainStmt(std::move(child_stmt_ptr));
  return rc;
}
