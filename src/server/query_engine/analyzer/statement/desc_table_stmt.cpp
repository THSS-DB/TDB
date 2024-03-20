#include "include/query_engine/analyzer/statement/desc_table_stmt.h"

#include "include/storage_engine/schema/database.h"

RC DescTableStmt::create(Db *db, const DescTableSqlNode &desc_table,
                         Stmt *&stmt) {
  if (db->find_table(desc_table.relation_name.c_str()) == nullptr) {
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }
  stmt = new DescTableStmt(desc_table.relation_name);
  return RC::SUCCESS;
}
