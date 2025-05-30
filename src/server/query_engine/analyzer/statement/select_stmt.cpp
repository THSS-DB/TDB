#include "include/query_engine/analyzer/statement/select_stmt.h"
#include "include/query_engine/analyzer/statement/filter_stmt.h"
#include "include/query_engine/analyzer/statement/group_by_stmt.h"
#include "include/query_engine/analyzer/statement/orderby_stmt.h"
#include "include/query_engine/structor/expression/field_expression.h"
#include "include/query_engine/structor/expression/attribute_expression.h"

#include "common/log/log.h"
#include "common/lang/string.h"
#include "include/storage_engine/schema/database.h"
#include "include/storage_engine/recorder/table.h"

#include <algorithm>

SelectStmt::~SelectStmt()
{
  // 这些都是 create 中创建的对象，独占所有权，需要释放
  for (auto *field : query_fields_) {
    delete field;
  }
  for (auto *project : projects_) {
    delete project;
  }

  if (nullptr != filter_stmt_) {
    delete filter_stmt_;
    filter_stmt_ = nullptr;
  }

  delete group_by_stmt_;
  delete having_stmt_;
  delete order_stmt_;
  for (auto *join_filter_stmt : join_filter_stmts_) {
    delete join_filter_stmt;
  }
}

static void wildcard_fields_without_table_name(Table *table, std::vector<Expression*> &projects, const std::string &table_name) {
  const TableMeta &table_meta = table->table_meta();
  const int field_num = table_meta.field_num();
  for (int i = table_meta.sys_field_num(); i < field_num; i++) {
    if (table_meta.field(i)->visible()) {
      auto *field_expr = new FieldExpr(table, table_meta.field(i));
      field_expr->set_field_table_alias(table_name);
      field_expr->set_alias(table_meta.field(i)->name());
      projects.push_back(field_expr);
    }
  }
}

static void wildcard_fields_with_table_name(Table *table, std::vector<Expression*> &projects, const std::string &table_name) {
  const TableMeta &table_meta = table->table_meta();
  const int field_num = table_meta.field_num();
  for (int i = table_meta.sys_field_num(); i < field_num; i++) {
    if (table_meta.field(i)->visible()) {
      auto *field_expr = new FieldExpr(table, table_meta.field(i));
      field_expr->set_field_table_alias(table_name);
      field_expr->set_alias(table_name + "." + table_meta.field(i)->name());
      projects.push_back(field_expr);
    }
  }
}

static void wildcard_fields(
    Table *table,
    int table_count,
    std::vector<Expression*> &projects,
    const std::string &alias) {

  if (table_count == 1) {
    wildcard_fields_without_table_name(table, projects, alias);
  } else {
    wildcard_fields_with_table_name(table, projects, alias);
  }
}

RC _process_attribute_expression(Db *db, const Expression *expr, const char *table_name, const char* field_name,
    const std::vector<Table *> &tables /*in*/,
    std::vector<Expression *> &projects /*out*/,
    const std::vector<std::string> &table_alias /*in*/) {
  int table_count = static_cast<int>(tables.size());
  /**
   * There should be four possible states for attribute_expression with '*'
   * 1. *.* [get all the fields in all the tables]
   * 2. *.attr [error]
   * 3. rel.attr [just get]
   * 4. rel.* [get all fields]
   * Besides, we should also consider the situation where tableName is null
   * This method is complex, you can try to optimize it and submit PR if you like.
   */

  bool table_is_blank = common::is_blank(table_name);
  bool table_is_wildcard = 0 == strcmp(table_name, "*");
  bool field_is_wildcard = 0 == strcmp(field_name, "*");

  if (field_is_wildcard) {
    if (table_is_blank || table_is_wildcard) {
      // select * or select *.*
      for (int i = 0; i < table_count; i++) {
        wildcard_fields(tables[i], table_count, projects, table_alias[i]);
      }
      return RC::SUCCESS;
    }
  } else {
    if (table_is_wildcard) {
      // select *.attr is unsupported, because you can't guarantee there is
      // corresponding 'attr' in each table
      LOG_WARN("invalid field name while table is *. attr=%s", field_name);
      return RC::SCHEMA_FIELD_MISSING;
    }
  }

  // select attr or select table.(attr|*), all for a certain table
  Table *table = nullptr;
  std::string alias;

  if (table_is_blank) {
    // If table_name is null, there should be only one table as default table, otherwise it
    // will be ambiguous
    if (table_count != 1) {
      LOG_WARN("invalid. I do not know the attr's table. attr=%s", field_name);
      return RC::SCHEMA_FIELD_MISSING;
    }

    table = tables.front();
    alias = table_alias.front();
  } else {
    auto iter = std::find(table_alias.begin(), table_alias.end(), table_name);
    if (iter == table_alias.end()) {
      LOG_WARN("no such table in from list: %s", table_name);
      return RC::SCHEMA_FIELD_MISSING;
    }

    table = tables[iter - table_alias.begin()];
    alias = *iter;
  }

  if (field_is_wildcard) {
    // select table.*
    wildcard_fields_with_table_name(table, projects, alias);
  } else {
    // select a single field
    const FieldMeta *field_meta = table->table_meta().field(field_name);
    if (nullptr == field_meta) {
      LOG_WARN("no such field. field=%s.%s", table->name(), field_name);
      return RC::SCHEMA_FIELD_MISSING;
    }
    auto *field_expr = new FieldExpr(table, field_meta);
    field_expr->set_field_table_alias(alias);
    field_expr->set_alias(expr->alias().empty() ? expr->name() : expr->alias());
    projects.push_back(field_expr);
  }
  return RC::SUCCESS;
}


RC SelectStmt::analyze_tables_and_projects(
    Db *db,
    const SelectSqlNode &select_sql,
    std::vector<Table *> &tables,
    std::vector<std::string> &table_alias,
    std::unordered_map<std::string, Table *> &table_map,
    std::vector<Expression *> &projects) {

  // 1. Collect tables in `from` statement
  for (size_t i = 0; i < select_sql.relations.size(); i++) {
    const string &table_name = select_sql.relations[i].relation_name;
    string alias_name = select_sql.relations[i].alias;
    
    // If alias is empty, use table_name as alias
    if (alias_name.empty()) {
      alias_name = table_name;
    }

    // check if alias duplicate
    if (0 != table_map.count(alias_name)) {
      return RC::SQL_SYNTAX;
    }

    // Find relevant table object from memory(opened_tables)
    Table *table = db->find_table(table_name.c_str());
    if (nullptr == table) {
      LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name.c_str());
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }

    tables.push_back(table);

    table_map.insert(std::pair<std::string, Table *>(alias_name, table));
    table_alias.emplace_back(alias_name);
  }

  // 2. Collect info in `select` statement
  for (int i = (int)select_sql.attributes.size() - 1; i >= 0; i--) {
    const Expression *expr = select_sql.attributes[i];

    // AttributeExpression needs to be processed separately because '*' in attributes may cause difference.
    if (expr->type() == ExprType::REL_ATTR) { // Simple FiledExpr
      const auto *rel_attr_expr = dynamic_cast<const RelAttrExpr *>(expr);
      const RelAttrSqlNode relation_attr = rel_attr_expr->rel_attr_sql_node();
      // special logic for attribute expression to deal with '*'
      RC rc = _process_attribute_expression(db, expr, relation_attr.relation_name.c_str(), relation_attr.attribute_name.c_str(),
          tables, projects, table_alias);
      if (rc != RC::SUCCESS) {
        return rc;
      }

    } else {
      // normal expression analysis
      Expression *res_project = nullptr;
      RC rc = analyze_expression(expr, db, table_map, tables, res_project);
      if (rc != RC::SUCCESS) {
        return rc;
      }
      projects.emplace_back(res_project);
    }
  }
  return RC::SUCCESS;
}

RC SelectStmt::create(Db *db, const SelectSqlNode &select_sql, Stmt *&stmt)
{
  if (nullptr == db) {
    LOG_WARN("invalid argument. db is null");
    return RC::INVALID_ARGUMENT;
  }

  if (select_sql.relations.empty()) {
    return RC::INVALID_ARGUMENT;
  }

  std::vector<Table *> tables;
  std::vector<std::string> table_alias;
  std::unordered_map<std::string, Table *> table_map;
  std::vector<Expression *> projects;

  RC rc = analyze_tables_and_projects(db, select_sql, tables, table_alias, table_map, projects);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to analyze tables and projects. rc=%d:%s", rc, strrc(rc));
    return rc;
  }

  // collect query fields in `select` statement
  std::vector<Field *> query_fields;
  for (auto& project : projects) {
    project->getFields(query_fields);
  }

  LOG_DEBUG("got %d tables in from stmt and %d fields in query stmt", tables.size(), projects.size());

  Table *default_table = nullptr;
  if (tables.size() == 1) {
    default_table = tables[0];
  }

  // create filter statements in join_lists
  std::vector<FilterStmt *> join_filter_stmts;
  for (auto& join_list :select_sql.join_lists) {
    FilterStmt *join_filter_stmt = nullptr;
    rc = FilterStmt::create(db,
        default_table,
        &table_map,
        join_list.join_conditions.data(),
        static_cast<int>(join_list.join_conditions.size()),
        join_filter_stmt);
    join_filter_stmts.emplace_back(join_filter_stmt);
    if (rc != RC::SUCCESS) {
      LOG_WARN("cannot construct filter stmt");
      return rc;
    }
  }

  // create filter statement in `where` statement
  FilterStmt *filter_stmt = nullptr;
  rc = FilterStmt::create(db,
      default_table,
      &table_map,
      select_sql.where_conditions.conditions.data(),
      static_cast<int>(select_sql.where_conditions.conditions.size()),
      filter_stmt);
  if (rc != RC::SUCCESS) {
    LOG_WARN("cannot construct filter stmt");
    return rc;
  }
  filter_stmt->set_conjunction_type(select_sql.where_conditions.type);

  GroupByStmt *group_by_stmt = nullptr;
  if (select_sql.group_by_attributes.size() > 0) {
    rc = GroupByStmt::create(db, default_table, &table_map, select_sql.group_by_attributes, group_by_stmt);
    if (rc != RC::SUCCESS) {
      LOG_WARN("cannot construct group by stmt");
      return rc;
    }
  }

  // create having filter statement in `having` clause
  FilterStmt *having_stmt = nullptr;
  rc = FilterStmt::create(db,
      default_table,
      &table_map,
      select_sql.having_conditions.conditions.data(),
      static_cast<int>(select_sql.having_conditions.conditions.size()),
      having_stmt);
  if (rc != RC::SUCCESS) {
    LOG_WARN("cannot construct filter stmt");
    return rc;
  }

  having_stmt->set_conjunction_type(select_sql.having_conditions.type);

  // create order by statement in `order by` statement
  OrderByStmt *order_stmt = nullptr;
  rc = OrderByStmt::create(db,
      default_table,
      &table_map,
      select_sql.order_lists,
      static_cast<int>(select_sql.order_lists.size()),
      order_stmt);
  if (rc != RC::SUCCESS) {
    LOG_WARN("cannot construct order by stmt");
    return rc;
  }

  int aggr_count = 0, field_count = 0;
  for (Expression *expr : projects) {
    if (expr->type() == ExprType::AGGR) {
      ++aggr_count;
    }
    if (expr->type() == ExprType::FIELD) {
      ++field_count;
    }
  }
  if (aggr_count && field_count) {
    if (group_by_stmt == nullptr) {
      // either all aggr_expr or all expr without group by
      return RC::INVALID_ARGUMENT;
    }
    for (Expression *expr : projects) {
      if (expr->type() == ExprType::FIELD) {
        bool exist = false;
        auto *field_expr = dynamic_cast<FieldExpr *>(expr);
        for (auto *group_by_expr : group_by_stmt->group_by_exprs()) {
          if (field_expr->field() == ((FieldExpr *) group_by_expr)->field()) {
            exist = true;
            break;
          }
        }
        if (!exist) {
          return RC::INVALID_ARGUMENT;
        }
      }
    }
  }

  // everything alright
  auto *select_stmt = new SelectStmt();
  select_stmt->query_fields_.swap(query_fields);
  select_stmt->projects_.swap(projects);
  select_stmt->tables_.swap(tables);
  select_stmt->table_alias_.swap(table_alias);
  select_stmt->filter_stmt_ = filter_stmt;
  select_stmt->group_by_stmt_ = group_by_stmt;
  select_stmt->having_stmt_ = having_stmt;
  select_stmt->order_stmt_ = order_stmt;
  select_stmt->join_filter_stmts_.swap(join_filter_stmts);
  stmt = select_stmt;
  return RC::SUCCESS;
}
