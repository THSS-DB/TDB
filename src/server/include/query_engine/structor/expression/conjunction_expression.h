#pragma once

#include "expression.h"

class ConjunctionExpr : public Expression {
 public:
 public:
  ConjunctionExpr(ConjunctionType type,
                  std::vector<std::unique_ptr<Expression>> &children)
      : conjunction_type_(type), children_(std::move(children)) {
    this->type_ = ExprType::CONJUNCTION;
  }
  virtual ~ConjunctionExpr() {}

  AttrType value_type() const override { return BOOLEANS; }

  RC get_value(const Tuple &tuple, Value &value) const override;

  ConjunctionType conjunction_type() const { return conjunction_type_; }

  std::vector<std::unique_ptr<Expression>> &children() { return children_; }

  RC set_trx(Trx *trx) const;

  ConjunctionExpr *copy() const override {
    std::vector<std::unique_ptr<Expression>> children;
    for (auto &child : children_) {
      children.emplace_back(
          static_cast<std::unique_ptr<Expression>>(child->copy()));
    }
    auto *res = new ConjunctionExpr(conjunction_type_, children);
    res->set_name(name());
    res->set_alias(alias());
    return res;
  }

 private:
  ConjunctionType conjunction_type_ = ConjunctionType::AND;
  std::vector<std::unique_ptr<Expression>> children_;
};