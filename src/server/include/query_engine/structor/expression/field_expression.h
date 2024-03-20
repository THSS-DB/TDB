#pragma once

#include "expression.h"

class FieldExpr : public Expression {
 public:
  FieldExpr() { type_ = ExprType::FIELD; }

  explicit FieldExpr(const Field &field) : field_(field) {
    type_ = ExprType::FIELD;
  }

  FieldExpr(const Table *table, const FieldMeta *field) : field_(table, field) {
    type_ = ExprType::FIELD;
  }

  virtual ~FieldExpr() = default;
  AttrType value_type() const override { return field_.attr_type(); }

  Field &field() { return field_; }

  const Field &field() const { return field_; }

  const char *table_name() const { return field_.table_name(); }

  const char *field_name() const { return field_.field_name(); }

  void set_field_table_alias(const std::string &s) {
    field_.set_table_alias(s);
  }

  int len() override { return field_.len(); }

  bool nullable() override { return field_.nullable(); }

  RC get_value(const Tuple &tuple, Value &value) const override;

  void getFields(std::vector<Field *> &query_fields) const override;

  FieldExpr *copy() const override {
    auto *res = new FieldExpr(field_);
    res->set_name(name());
    res->set_alias(alias());
    return res;
  }

 private:
  Field field_;
};
