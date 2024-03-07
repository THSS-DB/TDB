#include "include/query_engine/analyzer/statement/create_index_stmt.h"
#include "include/storage_engine/recorder/table.h"
#include "include/storage_engine/schema/database.h"
#include "common/lang/string.h"
#include "common/log/log.h"

using namespace std;
using namespace common;

RC CreateIndexStmt::create(Db *db, const CreateIndexSqlNode &create_index, Stmt *&stmt)
{
  stmt = nullptr;

  const char *table_name = create_index.relation_name.c_str();
  if (is_blank(table_name) || is_blank(create_index.index_name.c_str()) || create_index.multi_attribute_names.size() == 0) {
    LOG_WARN("invalid argument. db=%p, table_name=%p, index name=%s, attributes amount=%d",
        db, table_name, create_index.index_name.c_str(), create_index.multi_attribute_names.size());
    return RC::INVALID_ARGUMENT;
  }

  // check whether the table exists
  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  std::vector<const FieldMeta*> multi_field_metas;
  for (int i = 0; i < create_index.multi_attribute_names.size(); i++) {
    const FieldMeta *field_meta = table->table_meta().field(create_index.multi_attribute_names[i].c_str());
    if (nullptr == field_meta) {
      LOG_WARN("no such field in table. db=%s, table=%s, field name=%s",
             db->name(), table_name, create_index.multi_attribute_names[i].c_str());
      return RC::SCHEMA_FIELD_NOT_EXIST;
    }
    multi_field_metas.emplace_back(field_meta);
  }

  for (int i = 0; i < table->table_meta().sys_field_num(); i ++) {
    multi_field_metas.emplace_back(table->table_meta().field(i));
  }

  Index *index = table->find_index(create_index.index_name.c_str());
  if (nullptr != index) {
    LOG_WARN("index with name(%s) already exists. table name=%s", create_index.index_name.c_str(), table_name);
    return RC::SCHEMA_INDEX_NAME_REPEAT;
  }

  stmt = new CreateIndexStmt(table, multi_field_metas, create_index.index_name, create_index.is_unique_);

  return RC::SUCCESS;
}
