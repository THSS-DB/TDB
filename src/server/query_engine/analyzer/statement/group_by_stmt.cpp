#include "include/query_engine/analyzer/statement/group_by_stmt.h"

#include "common/lang/string.h"
#include "include/query_engine/structor/expression/field_expression.h"
#include "include/storage_engine/schema/database.h"

GroupByStmt::~GroupByStmt() {
  for (auto *expr : exprs_) {
    delete expr;
  }
  exprs_.clear();
}

RC GroupByStmt::create(Db *db, Table *default_table,
                       std::unordered_map<std::string, Table *> *tables,
                       const std::vector<RelAttrSqlNode> units,
                       GroupByStmt *&stmt) {
  RC rc = RC::SUCCESS;
  stmt = nullptr;

  GroupByStmt *tmp_stmt = new GroupByStmt();
  for (const auto &unit : units) {
    Expression *expr = nullptr;
    rc = create_group_by_unit(db, default_table, tables, unit, expr);
    if (rc != RC::SUCCESS) {
      delete tmp_stmt;
      LOG_WARN("failed to create group by stmt");
      return rc;
    }
    tmp_stmt->exprs_.push_back(expr);
  }

  stmt = tmp_stmt;
  return rc;
}

RC GroupByStmt::create_group_by_unit(
    Db *db, Table *default_table,
    std::unordered_map<std::string, Table *> *tables,
    const RelAttrSqlNode &unit, Expression *&res_expr) {
  Table *table = nullptr;
  if (common::is_blank(unit.relation_name.c_str())) {
    table = default_table;

  } else if (nullptr != tables) {
    auto iter = tables->find(std::string(unit.relation_name));
    if (iter != tables->end()) {
      table = iter->second;
    }

  } else {
    table = db->find_table(unit.relation_name.c_str());
  }

  if (nullptr == table) {
    LOG_WARN("No such table: unit.relation_name: %s",
             unit.relation_name.c_str());
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  const FieldMeta *field =
      table->table_meta().field(unit.attribute_name.c_str());
  if (nullptr == field) {
    LOG_WARN("no such field in table: table %s, field %s", table->name(),
             unit.attribute_name.c_str());
    return RC::SCHEMA_FIELD_NOT_EXIST;
  }

  res_expr = new FieldExpr(table, field);
  ((FieldExpr *)res_expr)->set_field_table_alias(table->name());
  res_expr->set_name(unit.attribute_name.c_str());
  res_expr->set_alias(unit.attribute_name.c_str());
  return RC::SUCCESS;
}