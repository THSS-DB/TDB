#include "include/query_engine/analyzer/statement/update_stmt.h"

#include <utility>
#include "include/query_engine/analyzer/statement/filter_stmt.h"
#include "common/log/log.h"
#include "include/storage_engine/schema/database.h"
#include "include/storage_engine/recorder/table.h"

UpdateStmt::UpdateStmt(Table *table, std::vector<UpdateUnit> update_units, FilterStmt *filter_stmt) : table_(table), update_units_(std::move(update_units)), filter_stmt_(filter_stmt)
{}

UpdateStmt::~UpdateStmt()
{
  // 这些都是 create 中创建的对象，独占所有权，需要释放
  delete filter_stmt_;
  for (auto &unit : update_units_) {
    delete unit.value;
  }
}

RC UpdateStmt::create(Db *db, const UpdateSqlNode &update, Stmt *&stmt)
{
  const char *table_name = update.relation_name.c_str();
  if(nullptr == db || nullptr == table_name || update.update_units.empty()) {
    LOG_WARN("invalid argument. db=%p, table_name=%p", db, table_name);
    return RC::INVALID_ARGUMENT;
  }

  // check whether the table exists
  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }
  if (table->is_view()) {
    table = db->find_table(table->origin_table_name());
    if (nullptr == table) {
      LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }
    table_name = table->name();
  }

  // check attribute
  std::vector<UpdateUnit> update_units;
  const TableMeta &table_meta = table->table_meta();
  const int field_num = table_meta.field_num() - table_meta.sys_field_num() - table_meta.null_filed_num();
  const int sys_field_num = table_meta.sys_field_num();
  for (int v = 0; v < update.update_units.size(); v++) {
    bool field_exist = false;
    std::string attribute_name = update.update_units[v].attribute_name;
    Expression *expr = update.update_units[v].value;
    for (int i = 0; i < field_num; i++) {
      const FieldMeta *field_meta = table_meta.field(i + sys_field_num);
      if (attribute_name == field_meta->name()) {
        field_exist = true;
        Expression *expression = nullptr;
        const std::unordered_map <std::string, Table *> table_map;
        const std::vector <Table *>                     tables;
        if (expr->type() == ExprType::VALUE) {
          RC rc = analyze_expression(expr, db, table_map, tables, expression);
          if (RC::SUCCESS != rc) {
            LOG_ERROR("Analyze ValueExpr Failed. RC = %d:%s", rc, strrc(rc));
            return rc;
          }
        } else {
          return RC::INVALID_ARGUMENT;
        }
        UpdateUnit unit;
        unit.value = expression;
        unit.attribute_name = std::move(attribute_name);
        update_units.emplace_back(unit);
        break;
      }
    }
    if (!field_exist) {
      LOG_WARN("Field %s.%s is not exist", table_name, attribute_name.data());
      return RC::SCHEMA_FIELD_NOT_EXIST;
    }
  }

  std::unordered_map<std::string, Table *> table_map;
  table_map.insert(std::pair<std::string, Table *>(std::string(table_name), table));

  FilterStmt *filter_stmt = nullptr;
  RC rc = FilterStmt::create(db, table, &table_map, update.conditions.data(), static_cast<int>(update.conditions.size()), filter_stmt);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create filter statement. rc=%d:%s", rc, strrc(rc));
    return rc;
  }
  // everything alright
  stmt = new UpdateStmt(table, update_units, filter_stmt);
  return rc;
}