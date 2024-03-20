#include "include/query_engine/executor/create_index_executor.h"

#include "include/query_engine/analyzer/statement/create_index_stmt.h"
#include "include/query_engine/structor/query_info.h"
#include "include/session/session.h"

RC CreateIndexExecutor::execute(QueryInfo *query_info) {
  Stmt *stmt = query_info->stmt();
  Session *session = query_info->session_event()->session();
  ASSERT(stmt->type() == StmtType::CREATE_INDEX,
         "create index executor can not run this command: %d",
         static_cast<int>(stmt->type()));

  CreateIndexStmt *create_index_stmt = static_cast<CreateIndexStmt *>(stmt);

  Trx *trx = session->current_trx();
  Table *table = create_index_stmt->table();
  return table->create_index(trx, create_index_stmt->multi_field_metas(),
                             create_index_stmt->index_name().c_str(),
                             create_index_stmt->is_unique());
}