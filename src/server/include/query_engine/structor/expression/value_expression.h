#pragma once

#include "expression.h"

class ValueExpr : public Expression {
 public:
  ValueExpr() { this->type_ = ExprType::VALUE; }

  explicit ValueExpr(const Value &value) : value_(value) {
    this->type_ = ExprType::VALUE;
  }

  ~ValueExpr() override = default;

  RC get_value(const Tuple &tuple, Value &value) const override;
  RC try_get_value(Value &value) const override {
    value = value_;
    return RC::SUCCESS;
  }

  AttrType value_type() const override { return value_.attr_type(); }

  void get_value(Value &value) const { value = value_; }

  const Value &get_value() const { return value_; }

  ValueExpr *copy() const override {
    auto *res = new ValueExpr(value_);
    res->set_name(name());
    res->set_alias(alias());
    return res;
  }

 private:
  Value value_;
};

/**
 * @brief 很多常量值表达式
 * @ingroup Expression
 */
class ValuesExpr : public Expression {
 public:
  ValuesExpr() { this->type_ = ExprType::VALUES; }

  virtual ~ValuesExpr() = default;

  RC get_value(const Tuple &tuple, Value &value) const override {
    return RC::SUCCESS;
  }
  RC try_get_value(Value &value) const override { return RC::SUCCESS; }
  AttrType value_type() const override { return AttrType::UNDEFINED; }

  void add_value(const Value &value) { values_.push_back(value); }

  RC value_in(const Value &value, Value &result) const;
  RC value_exists(Value &result) const;

  ValuesExpr *copy() const override {
    auto *res = new ValuesExpr;
    for (auto &value : values_) {
      res->add_value(value);
    }
    res->set_name(name());
    res->set_alias(alias());
    return res;
  }

 private:
  std::vector<Value> values_;
};
