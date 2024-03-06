#include "include/query_engine/analyzer/statement/create_table_stmt.h"

RC CreateTableStmt::create(Db *db, const CreateTableSqlNode &create_table, Stmt *&stmt)
{
  stmt = new CreateTableStmt(create_table.relation_name, create_table.attr_infos);
  return RC::SUCCESS;
}
