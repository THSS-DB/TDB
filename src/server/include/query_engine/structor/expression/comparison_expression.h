#pragma once

#include "expression.h"

/**
 * @brief 比较表达式
 * @ingroup Expression
 */
class ComparisonExpr : public Expression {
 public:
  ComparisonExpr(CompOp comp, std::unique_ptr<Expression> left,
                 std::unique_ptr<Expression> right)
      : comp_(comp), left_(std::move(left)), right_(std::move(right)) {
    this->type_ = ExprType::COMPARISON;
  }

  virtual ~ComparisonExpr() {}

  RC get_value(const Tuple &tuple, Value &value) const override;

  AttrType value_type() const override { return BOOLEANS; }

  CompOp comp() const { return comp_; }

  std::unique_ptr<Expression> &left() { return left_; }
  const std::unique_ptr<Expression> &_left_() const { return left_; }
  std::unique_ptr<Expression> &right() { return right_; }
  const std::unique_ptr<Expression> &_right_() const { return right_; }

  /**
   * 尝试在没有tuple的情况下获取当前表达式的值
   * 在优化的时候，可能会使用到
   */
  RC try_get_value(Value &value) const override;

  /**
   * compare the two tuple cells
   * @param value the result of comparison
   */
  RC compare_value(const Value &left, const Value &right, bool &value) const;

  RC set_trx(Trx *trx) const;

  ComparisonExpr *copy() const override {
    ComparisonExpr *res = new ComparisonExpr(
        comp_, static_cast<std::unique_ptr<Expression>>(left_->copy()),
        static_cast<std::unique_ptr<Expression>>(right_->copy()));
    res->set_name(name());
    res->set_alias(alias());
    return res;
  }

 private:
  CompOp comp_;
  std::unique_ptr<Expression> left_ = nullptr;
  std::unique_ptr<Expression> right_ = nullptr;
};