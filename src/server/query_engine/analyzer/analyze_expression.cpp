#include "include/query_engine/structor/expression/analyze_expression.h"
#include "include/query_engine/structor/expression/field_expression.h"
#include "include/query_engine/structor/expression/aggregation_expression.h"
#include "include/query_engine/structor/expression/attribute_expression.h"
#include "include/query_engine/structor/expression/arithmetic_expression.h"
#include "include/query_engine/structor/expression/value_expression.h"
#include "include/query_engine/analyzer/statement/select_stmt.h"

RC analyze_expression(
    const Expression* expr,
    Db *db,
    const std::unordered_map<std::string, Table *> &table_map,
    const std::vector<Table *> &tables,
    Expression *&res_expr,
    bool is_create_table_select) {

  if (expr->type() == ExprType::AGGR) {
    const auto *aggr_expr = dynamic_cast<const AggrExpr *>(expr);
    Expression *field_expr = nullptr;

    if (aggr_expr->_aggr_type_() == AggrType::AGGR_COUNT &&
        strcmp(((RelAttrExpr *)aggr_expr->_expr_().get())->rel_attr_sql_node().relation_name.c_str(), "") == 0 &&
        strcmp(((RelAttrExpr *)aggr_expr->_expr_().get())->rel_attr_sql_node().attribute_name.c_str(), "*") == 0) {
      field_expr = new FieldExpr(tables.front(), new FieldMeta("*", AttrType::INTS, 0, 1, true));
      field_expr->set_name("*");
      field_expr->set_alias("*");
      ((FieldExpr *) field_expr)->set_field_table_alias(tables.front()->name());
      res_expr = new AggrExpr(aggr_expr->_aggr_type_(), field_expr);
      res_expr->set_name(expr->name());
      res_expr->set_alias(expr->alias());
      return RC::SUCCESS;
    }

    RC rc = analyze_expression(aggr_expr->_expr_().get(), db, table_map, tables, field_expr, is_create_table_select);
    if (rc != RC::SUCCESS) {
      LOG_ERROR("Parse AggrExpr inner expression failed. RC = %d:%s", rc, strrc(rc));
      return rc;
    }

    if (!strcmp(dynamic_cast<FieldExpr *>(field_expr)->field_name(), "*") &&
        aggr_expr->_aggr_type_() != AggrType::AGGR_COUNT) {
      return RC::INVALID_ARGUMENT;
    }

    res_expr = new AggrExpr(aggr_expr->_aggr_type_(), field_expr);
    res_expr->set_name(expr->name());
    res_expr->set_alias(expr->alias());
    return RC::SUCCESS;

  } else if (expr->type() == ExprType::ARITHMETIC) {
    const auto *arithmetic_expr = dynamic_cast<const ArithmeticExpr *>(expr);
    Expression *left_expr = nullptr;
    Expression *right_expr = nullptr;

    RC rc = analyze_expression(arithmetic_expr->_left_().get(), db, table_map, tables, left_expr, is_create_table_select);
    if (rc != RC::SUCCESS) {
      LOG_ERROR("Parse ArithmeticExpr left expression failed. RC = %d:%s", rc, strrc(rc));
      return rc;
    }

    if (arithmetic_expr->_right_() != nullptr) {
      rc = analyze_expression(arithmetic_expr->_right_().get(), db, table_map, tables, right_expr, is_create_table_select);
      if (rc != RC::SUCCESS) {
        LOG_ERROR("Parse ArithmeticExpr right expression failed. RC = %d:%s", rc, strrc(rc));
        return rc;
      }
    }

    if (left_expr->type() == ExprType::VALUE && (right_expr == nullptr || right_expr->type() == ExprType::VALUE)) {
      // Simplify expression
      Value left_value;
      Value right_value;
      left_expr->try_get_value(left_value);
      if (right_expr != nullptr) {
        right_expr->try_get_value(right_value);
      }

      Value result_value;
      arithmetic_expr->calc_value(left_value, right_value, result_value);
      res_expr = new ValueExpr(result_value);
      res_expr->set_name(expr->name());
      res_expr->set_alias(expr->alias());
      return RC::SUCCESS;
    }

    // Create new expression
    res_expr = new ArithmeticExpr(
      arithmetic_expr->arithmetic_type(),
      std::unique_ptr<Expression>(left_expr),
      std::unique_ptr<Expression>(right_expr));
    res_expr->set_name(expr->name());
    res_expr->set_alias(expr->alias());
    return RC::SUCCESS;

  } else if (expr->type() == ExprType::REL_ATTR) {
    const auto *rel_attr_expr = dynamic_cast<const RelAttrExpr *>(expr);
    const auto rel_attr_sql_node = rel_attr_expr->rel_attr_sql_node();
    const char *table_name = rel_attr_sql_node.relation_name.c_str();
    const char *field_name = rel_attr_sql_node.attribute_name.c_str();

    if (common::is_blank(table_name)) { // Select from only one table
      if (tables.size() != 1) {
        LOG_WARN("invalid. I do not know the attr's table. attr=%s", field_name);
        return RC::SCHEMA_FIELD_MISSING;
      }
      Table *table = tables[0];
      const FieldMeta *field_meta;
      if (!strcmp(field_name, "*")) {
        field_meta = new FieldMeta(field_name, AttrType::CHARS, 0, 1, true);
      } else {
        field_meta = table->table_meta().field(field_name);
        if (nullptr == field_meta) {
          LOG_WARN("no such field. field=%s.%s", table->name(), field_name);
          return RC::SCHEMA_FIELD_MISSING;
        }
      }

      res_expr = new FieldExpr(table, field_meta);
      std::string alias = table->name();
      for (const auto &it : table_map) {
        if (strcmp(it.second->name(), alias.c_str()) == 0 &&
            strcmp(it.first.c_str(), it.second->name()) != 0) {
          alias = it.first;
          break;
        }
      }
      ((FieldExpr *) res_expr)->set_field_table_alias(alias);
      res_expr->set_name(expr->name());
      res_expr->set_alias(expr->alias());
      return RC::SUCCESS;

    } else { // Select from specified table
      auto iter = table_map.find(table_name);
      if (iter == table_map.end()) {
        LOG_WARN("no such table in from list: %s", table_name);
        return RC::SCHEMA_FIELD_MISSING;
      }

      Table *table = iter->second;
      const FieldMeta *field_meta = table->table_meta().field(field_name);
      if (nullptr == field_meta) {
        LOG_WARN("no such field. field=%s.%s", table->name(), field_name);
        return RC::SCHEMA_FIELD_MISSING;
      }

      res_expr = new FieldExpr(table, field_meta);
      ((FieldExpr *) res_expr)->set_field_table_alias(rel_attr_sql_node.relation_name);
      res_expr->set_name(expr->name());
      res_expr->set_alias(expr->alias());
      if (std::string(table_name) != std::string(table->name())) {
        if (tables.size() != 1) {
          res_expr->set_alias(std::string(table_name) + "." + std::string(field_name));
        }
      }
      return RC::SUCCESS;
    }

  } else if (expr->type() == ExprType::VALUE) {
    res_expr = new ValueExpr(dynamic_cast<const ValueExpr *>(expr)->get_value());
    res_expr->set_name(expr->name());
    res_expr->set_alias(expr->alias());
    return RC::SUCCESS;

  } else if (expr->type() == ExprType::VALUES) {
    res_expr = dynamic_cast<const ValuesExpr *>(expr)->copy();
    res_expr->set_name(expr->name());
    res_expr->set_alias(expr->alias());
    return RC::SUCCESS;
  }

  return RC::UNIMPLENMENT;
}