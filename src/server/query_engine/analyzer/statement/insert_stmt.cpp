#include "include/query_engine/analyzer/statement/insert_stmt.h"
#include "common/log/log.h"
#include "storage/db/db.h"
#include "storage/table/table.h"
#include "storage/table/table_meta.h"

InsertStmt::InsertStmt(Table *table, const std::vector<std::vector<Value>> &multi_values, int record_amount, int value_amount)
    : table_(table), multi_values_(multi_values), record_amount_(record_amount),value_amount_(value_amount)
{}

RC InsertStmt::create(Db *db, const InsertSqlNode &inserts, Stmt *&stmt)
{
  const char *table_name = inserts.relation_name.c_str();
  if (nullptr == db || nullptr == table_name || inserts.multi_values.empty()) {
    LOG_WARN("invalid argument. db=%p, table_name=%p, record_num=%d",
        db, table_name, static_cast<int>(inserts.multi_values.size()));
    return RC::INVALID_ARGUMENT;
  }

  // check whether the table exists
  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }
  Table *view;
  bool is_view = table->is_view();
  if (is_view) {
    view = table;
    table = db->find_table(table->origin_table_name());
    if (nullptr == table) {
      LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }
  }

  // check the fields number
  const TableMeta &table_meta = table->table_meta();
  const int field_num = table_meta.field_num() - table_meta.sys_field_num() - table_meta.null_filed_num();
  std::vector<std::vector<Value>> multi_values;
  for (int i = 0; i < inserts.multi_values.size(); i++) {
    std::vector<Value> values = inserts.multi_values[i];
    const int value_num = static_cast<int>(values.size());
    if (field_num != value_num) {
      if (is_view) {
        if (view->table_meta().field_num() != field_num) {
          LOG_WARN("schema mismatch. value num=%d, field num in schema=%d", value_num, field_num);
          return RC::SCHEMA_FIELD_MISSING;
        }

        std::vector<Value> new_values;
        const std::vector<FieldMeta> *view_metas = view->table_meta().field_metas();
        const std::vector<FieldMeta> *origin_metas = table->table_meta().field_metas();
        const int sys_field_num = table->table_meta().sys_field_num();
        for (int j = sys_field_num; j < origin_metas->size() - 1; j++) {
          bool matched = false;
          for (int k = 0; k < view_metas->size(); k++) {
            if (strcmp(view_metas->at(k).name(), origin_metas->at(j).name()) == 0) {
              matched = true;
              new_values.emplace_back(values[k]);
            }
            if (matched) {
              break;
            }
          }
          if (!matched) {
            const FieldMeta &origin_meta = origin_metas->at(j);
            Value value = Value(AttrType::NULLS);
            new_values.emplace_back(value);
          }
        }
        multi_values.emplace_back(new_values);
        continue;
      }
      LOG_WARN("schema mismatch. value num=%d, field num in schema=%d", value_num, field_num);
      return RC::SCHEMA_FIELD_MISSING;
    }
    multi_values.emplace_back(values);
  }

  // check fields type
  const int sys_field_num = table_meta.sys_field_num();
  for (int v = 0; v < multi_values.size(); v++) {
    std::vector<Value> &values = const_cast<std::vector<Value>&>(multi_values[v]);
//  }
//  for (auto & values : inserts.multi_values) {
    for (int i = 0; i < field_num; i++) {
      const FieldMeta *field_meta = table_meta.field(i + sys_field_num);
      const AttrType field_type = field_meta->type();
      const AttrType value_type = values[i].attr_type();
      if (AttrType::NULLS == value_type) {
        if (!field_meta->nullable()) {
          LOG_WARN("field type mismatch. table=%s, field=%s, field type=%d, value_type=%d",
          table_name, field_meta->name(), field_type, value_type);
          return RC::SCHEMA_FIELD_TYPE_MISMATCH;
        }
        continue;
      }
      if (field_type == TEXTS && value_type == CHARS) {
        if (values[i].get_string().size() > 65535) {
          return RC::INVALID_ARGUMENT;
        }
        std::hash<std::string> hashFunction;
        std::size_t hashValue = hashFunction(values[i].get_string());
        std::string value_search_for_text = std::string(table->dir()) + "/" + std::string(table_name) + std::to_string(hashValue) + ".text";
        std::string value_stored_in_file = values[i].get_string();
        std::ofstream outputFile(value_search_for_text);
        if (outputFile.is_open()) {
          outputFile << value_stored_in_file;
          outputFile.close();
        } else {
          LOG_ERROR(" failed to write text value to file!");
        }
        values[i].set_text(value_search_for_text.c_str(), value_search_for_text.size());
        continue;
      }
      if (field_type != value_type) {  // TODO try to convert the value type to field type
        LOG_WARN("field type mismatch. table=%s, field=%s, field type=%d, value_type=%d",
          table_name, field_meta->name(), field_type, value_type);
        return RC::SCHEMA_FIELD_TYPE_MISMATCH;
      }
    }
  }
  // everything alright
  stmt = new InsertStmt(table, multi_values, multi_values.size(), field_num);
  return RC::SUCCESS;
}
