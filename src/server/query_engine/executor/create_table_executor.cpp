#include "include/query_engine/executor/create_table_executor.h"

#include "include/query_engine/structor/query_info.h"
#include "include/query_engine/analyzer/statement/stmt.h"
#include "include/query_engine/analyzer/statement/create_table_stmt.h"
#include "include/session/session.h"
#include "storage/db/db.h"

RC CreateTableExecutor::execute(QueryInfo *query_info)
{
  Stmt *stmt = query_info->stmt();
  Session *session = query_info->session_event()->session();
  ASSERT(stmt->type() == StmtType::CREATE_TABLE, 
         "create table executor can not run this command: %d", static_cast<int>(stmt->type()));

  CreateTableStmt *create_table_stmt = static_cast<CreateTableStmt *>(stmt);

  const int attribute_count = static_cast<int>(create_table_stmt->attr_infos().size());

  const char *table_name = create_table_stmt->table_name().c_str();
  RC rc = session->get_current_db()->create_table(table_name, attribute_count, create_table_stmt->attr_infos().data());

  return rc;
}