#include "include/query_engine/analyzer/statement/orderby_stmt.h"

#include "include/query_engine/analyzer/statement/filter_stmt.h"
#include "include/query_engine/structor/expression/field_expression.h"

RC OrderByStmt::create_order_unit(
    Db *db, Table *default_table,
    std::unordered_map<std::string, Table *> *tables,
    const OrderByNode &orderByNode, OrderByUnit *&order_unit) {
  Table *table = nullptr;
  const FieldMeta *field = nullptr;
  RC rc = get_table_and_field(db, default_table, tables, orderByNode.sort_attr,
                              table, field);
  if (rc != RC::SUCCESS) {
    LOG_WARN("cannot find attr");
    return rc;
  }
  Expression *expr = new FieldExpr(table, field);
  ((FieldExpr *)expr)->set_field_table_alias(table->name());
  order_unit = new OrderByUnit;
  order_unit->set_sort_type(orderByNode.is_asc);
  order_unit->set_expr(expr);
  return rc;
}
RC OrderByStmt::create(Db *db, Table *default_table,
                       std::unordered_map<std::string, Table *> *tables,
                       const std::vector<OrderByNode> &order_by_list,
                       int order_by_num, OrderByStmt *&stmt) {
  RC rc = RC::SUCCESS;
  stmt = nullptr;

  OrderByStmt *tmp_stmt = new OrderByStmt();
  for (int i = 0; i < order_by_num; i++) {
    OrderByUnit *order_unit = nullptr;
    rc = create_order_unit(db, default_table, tables, order_by_list[i],
                           order_unit);
    if (rc != RC::SUCCESS) {
      delete tmp_stmt;
      LOG_WARN("failed to create order by unit. condition index=%d", i);
      return rc;
    }
    tmp_stmt->order_units_.push_back(order_unit);
  }

  stmt = tmp_stmt;
  return rc;
}
OrderByStmt::~OrderByStmt() {
  for (OrderByUnit *unit : order_units_) {
    delete unit;
  }
  order_units_.clear();
}
