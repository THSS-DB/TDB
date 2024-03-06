#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "src/server/include/common/rc.h"
#include "stmt.h"
#include "src/server/include/query_engine/structor/expression/expression.h"
#include "src/server/storage/field/field.h"
#include "src/server/include/query_engine/structor/expression/analyze_expression.h"

class FieldMeta;
class FilterStmt;
class GroupByStmt;
class OrderByStmt;
class Db;
class Table;
class Expression;

/**
 * @brief 表示select语句
 * @ingroup Statement
 */
class SelectStmt : public Stmt {
public:
  SelectStmt() = default;
  ~SelectStmt() override;

  StmtType type() const override {
    return StmtType::SELECT;
  }

public:
  static RC create(Db *db, const SelectSqlNode &select_sql, Stmt *&stmt);

public:
  const std::vector<Field *> &query_fields() const {
    return query_fields_;
  }

  const std::vector<Expression *> &projects() const {
    return projects_;
  }

  const std::vector<Table *> &tables() const {
    return tables_;
  }

  const std::vector<std::string> &table_alias() const {
    return table_alias_;
  }

  FilterStmt *filter_stmt() const {
    return filter_stmt_;
  }

  GroupByStmt *group_by_stmt() const {
    return group_by_stmt_;
  }

  OrderByStmt *group_by_order_stmt() const {
    return group_by_order_stmt_;
  }

  FilterStmt *having_stmt() const {
    return having_stmt_;
  }

  OrderByStmt *order_stmt() const {
    return order_stmt_;
  }

  std::vector<FilterStmt *> join_filter_stmts() const {
    return join_filter_stmts_;
  }

  const std::vector<FuncResult> &function_results() const {
    return function_results_;
  }

private:

  static RC analyze_tables_and_projects(
      Db *db,
      const SelectSqlNode &select_sql,
      std::vector<Table *> &tables,
      std::vector<std::string> &table_alias,
      std::unordered_map<std::string, Table *> &table_map,
      std::vector<Expression *> &projects);

  std::vector<Field *> query_fields_;
  std::vector<Expression *> projects_;
  std::vector<Table *> tables_;
  std::vector<std::string> table_alias_;
  std::vector<FilterStmt *> join_filter_stmts_;
  FilterStmt *filter_stmt_ = nullptr;
  GroupByStmt *group_by_stmt_ = nullptr;
  OrderByStmt *group_by_order_stmt_ = nullptr;
  FilterStmt *having_stmt_ = nullptr;
  OrderByStmt *order_stmt_ = nullptr;
  std::vector<FuncResult> function_results_;
};
