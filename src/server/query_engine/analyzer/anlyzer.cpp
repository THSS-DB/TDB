#include "include/query_engine/analyzer/analyzer.h"
#include "include/query_engine/analyzer/statement/stmt.h"

RC Analyzer::analyze(QueryInfo *query_info) {
  RC rc;
  SessionRequest *session_event = query_info->session_event();
  SqlResult *sql_result = session_event->sql_result();

  Db *db = session_event->session()->get_current_db();
  if (nullptr == db) {
    LOG_ERROR("cannot find current db");
    rc = RC::SCHEMA_DB_NOT_EXIST;
    sql_result->set_return_code(rc);
    sql_result->set_state_string("no db selected");
    return rc;
  }

  ParsedSqlNode *sql_node = query_info->sql_node().get();
  Stmt *stmt = nullptr;
  rc = Stmt::create_stmt(db, *sql_node, stmt);
  if (rc != RC::SUCCESS && rc != RC::UNIMPLENMENT && rc != RC::ONLY_FUNCTIONS) {
    LOG_WARN("failed to create stmt. rc=%d:%s", rc, strrc(rc));
    sql_result->set_return_code(rc);
    return rc;
  }

  query_info->set_stmt(stmt);

  return rc;
}