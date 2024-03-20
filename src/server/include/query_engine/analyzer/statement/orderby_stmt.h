#pragma once

#include <unordered_map>
#include <vector>

#include "include/query_engine/parser/parse_defs.h"
#include "include/query_engine/structor/expression/expression.h"
#include "stmt.h"

class Db;
class Table;
class FieldMeta;

class OrderByUnit {
 public:
  OrderByUnit() = default;
  ~OrderByUnit(){};

 public:
  void set_sort_type(bool sort_type) { is_asc_ = sort_type; }
  bool sort_type() const { return is_asc_; }
  void set_expr(Expression *expr) { expr_ = expr; }
  Expression *expr() const { return expr_; }

 private:
  bool is_asc_ = true;
  Expression *expr_ = nullptr;
};

/**
 * @brief order by / 排序
 * @ingroup Statement
 */
class OrderByStmt {
 public:
  OrderByStmt() = default;
  virtual ~OrderByStmt();

 public:
  const std::vector<OrderByUnit *> &order_units() const { return order_units_; }

 public:
  static RC create(Db *db, Table *default_table,
                   std::unordered_map<std::string, Table *> *tables,
                   const std::vector<OrderByNode> &order_by_list,
                   int order_by_num, OrderByStmt *&stmt);

  static RC create_order_unit(Db *db, Table *default_table,
                              std::unordered_map<std::string, Table *> *tables,
                              const OrderByNode &orderByNode,
                              OrderByUnit *&order_unit);

 private:
  std::vector<OrderByUnit *> order_units_;
};
