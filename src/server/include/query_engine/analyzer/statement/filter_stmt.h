#pragma once

#include <vector>
#include <unordered_map>
#include "include/query_engine/parser/parse_defs.h"
#include "stmt.h"
#include "include/query_engine/structor/expression/expression.h"
#include "include/query_engine/structor/expression/analyze_expression.h"

class Db;
class Table;
class FieldMeta;

struct FilterObj 
{
  bool is_attr;
  Field field;
  Value value;

  void init_attr(const Field &field)
  {
    is_attr = true;
    this->field = field;
  }

  void init_value(const Value &value)
  {
    is_attr = false;
    this->value = value;
  }
};

class FilterUnit 
{
public:
  FilterUnit() = default;
  ~FilterUnit()
  {
    // FilterUnit 只会在 create_filter_unit 中创建，这里的左右表达式都是独占所有权，需要释放
    delete left_expr_;
    delete right_expr_;
  }

  void set_comp(CompOp comp)
  {
    comp_ = comp;
  }

  CompOp comp() const
  {
    return comp_;
  }

  void set_left_expr(Expression *expr) {
    left_expr_ = expr;
  }

  void set_right_expr(Expression *expr) {
    right_expr_ = expr;
  }

  Expression *left_expr() {
    return left_expr_;
  }

  Expression *right_expr() {
      return right_expr_;
  }

  const Expression *left_expr() const {
    return left_expr_;
  }

  const Expression *right_expr() const {
    return right_expr_;
  }

private:
  CompOp comp_ = NO_OP;
  Expression* left_expr_ = nullptr;
  Expression* right_expr_ = nullptr;
};

/**
 * @brief Filter/谓词/过滤语句
 * @ingroup Statement
 */
class FilterStmt 
{
public:
  FilterStmt() = default;
  virtual ~FilterStmt();

public:
  const std::vector<FilterUnit *> &filter_units() const
  {
    return filter_units_;
  }

public:
  static RC create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
      const ConditionSqlNode *conditions, int condition_num, FilterStmt *&stmt);

  static RC create_filter_unit(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
      const ConditionSqlNode &condition, FilterUnit *&filter_unit);

  void add_filter_unit(FilterUnit* filterUnit) {
    filter_units_.emplace_back(filterUnit);
  }

  void set_conjunction_type(ConjunctionType type) {
    conjunction_type_ = type;
  }

  ConjunctionType get_conjunction_type() const {
    return conjunction_type_;
  }

private:
  ConjunctionType conjunction_type_ = ConjunctionType::AND;
  std::vector<FilterUnit *> filter_units_;
};

RC get_table_and_field(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
    const RelAttrSqlNode &attr, Table *&table, const FieldMeta *&field);