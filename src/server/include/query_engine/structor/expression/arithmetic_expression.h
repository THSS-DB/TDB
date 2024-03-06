#pragma once

#include "expression.h"

class ArithmeticExpr : public Expression
{
public:
  enum class Type {
    ADD,
    SUB,
    MUL,
    DIV,
    NEGATIVE,
  };

public:
  ArithmeticExpr(Type type, Expression *left, Expression *right)
      : arithmetic_type_(type), left_(left), right_(right) {
    this->type_ = ExprType::ARITHMETIC;
  }

  ArithmeticExpr(Type type, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
      : arithmetic_type_(type), left_(std::move(left)), right_(std::move(right)) {
    this->type_ = ExprType::ARITHMETIC;
  }

  virtual ~ArithmeticExpr() {}

  AttrType value_type() const override;

  int len() override {
    return left_->len();
  }

  bool nullable() override {
    return left_->nullable();
  }

  RC get_value(const Tuple &tuple, Value &value) const override;
  RC try_get_value(Value &value) const override;

  Type arithmetic_type() const { return arithmetic_type_; }

  std::unique_ptr<Expression> &left() { return left_; }
  const std::unique_ptr<Expression> &_left_() const { return left_; }
  std::unique_ptr<Expression> &right() { return right_; }
  const std::unique_ptr<Expression> &_right_() const { return right_; }

  void getFields(std::vector<Field *> &query_fields) const override;

  ArithmeticExpr* copy() const override {
    Expression* right_copy = nullptr;
    if (arithmetic_type_ != Type::NEGATIVE) {
      right_copy = right_->copy();
    }
    auto *res = new ArithmeticExpr(
        arithmetic_type_,
        static_cast<std::unique_ptr<Expression>>(left_->copy()),
        static_cast<std::unique_ptr<Expression>>(right_copy));
    res->set_name(name());
    res->set_alias(alias());
    return res;
  }

  RC calc_value(const Value &left_value, const Value &right_value, Value &value) const;

private:
  Type arithmetic_type_;
  std::unique_ptr<Expression> left_ = nullptr;
  std::unique_ptr<Expression> right_ = nullptr;
};