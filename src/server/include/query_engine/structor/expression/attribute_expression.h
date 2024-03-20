#pragma once

#include "expression.h"

/**
 * @brief 属性表达式
 */
class RelAttrExpr : public Expression {
public:
  RelAttrExpr(RelAttrSqlNode rel_attr_sql_node) : rel_attr_sql_node_(std::move(rel_attr_sql_node)) {
    this->type_ = ExprType::REL_ATTR;
  }

  ~RelAttrExpr() {}

  AttrType value_type() const override { return AttrType::UNDEFINED; }

  RC get_value(const Tuple &tuple, Value &value) const override { return RC::SUCCESS; }

  RC try_get_value(Value &value) const override { return RC::SUCCESS; }

  const RelAttrSqlNode rel_attr_sql_node() const { return rel_attr_sql_node_; }

  RelAttrExpr* copy() const override {
    auto *res = new RelAttrExpr(rel_attr_sql_node_);
    res->set_name(name());
    res->set_alias(alias());
    return res;
  }

private:
  RelAttrSqlNode rel_attr_sql_node_;
};
