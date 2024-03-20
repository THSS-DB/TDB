#include "include/query_engine/analyzer/statement/load_data_stmt.h"

#include <unistd.h>

#include "common/lang/string.h"
#include "common/log/log.h"
#include "include/storage_engine/recorder/table.h"
#include "include/storage_engine/schema/database.h"

using namespace common;

RC LoadDataStmt::create(Db *db, const LoadDataSqlNode &load_data, Stmt *&stmt) {
  RC rc = RC::SUCCESS;
  const char *table_name = load_data.relation_name.c_str();
  if (is_blank(table_name) || is_blank(load_data.file_name.c_str())) {
    LOG_WARN("invalid argument. db=%p, table_name=%p, file name=%s", db,
             table_name, load_data.file_name.c_str());
    return RC::INVALID_ARGUMENT;
  }

  // check whether the table exists
  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  if (0 != access(load_data.file_name.c_str(), R_OK)) {
    LOG_WARN("no such file to load. file name=%s, error=%s",
             load_data.file_name.c_str(), strerror(errno));
    return RC::FILE_NOT_EXIST;
  }

  stmt = new LoadDataStmt(table, load_data.file_name.c_str());
  return rc;
}
