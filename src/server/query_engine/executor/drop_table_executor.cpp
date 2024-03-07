#include "include/query_engine/executor/drop_table_executor.h"

#include "include/query_engine/structor/query_info.h"
#include "include/session/session.h"
#include "common/log/log.h"
#include "include/storage_engine/recorder/table.h"
#include "include/query_engine/analyzer/statement/drop_table_stmt.h"
#include "include/query_engine/structor/query_info.h"
#include "include/storage_engine/schema/database.h"

RC DropTableExecutor::execute(QueryInfo *query_info)
{
  Stmt *stmt = query_info->stmt();
  Session *session = query_info->session_event()->session();
  ASSERT(stmt->type() == StmtType::DROP_TABLE,
      "drop table executor can not run this command: %d", static_cast<int>(stmt->type()));

  DropTableStmt *drop_table_stmt = static_cast<DropTableStmt *>(stmt);

  const char *table_name = drop_table_stmt->table_name().c_str();
  RC rc = session->get_current_db()->drop_table(table_name);

  return rc;
}