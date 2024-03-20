#pragma once

#include "field_expression.h"

class AggrExpr : public Expression {

public:

  AggrExpr(AggrType aggr_type, Expression* expr) : aggr_type_(aggr_type), expr_(expr) {
    this->type_ = ExprType::AGGR;
  }

  ~AggrExpr() {}

  AttrType value_type() const override {
    if (aggr_type_ == AggrType::AGGR_COUNT) {
      return AttrType::INTS;
    }
    return ((FieldExpr *) expr_.get())->value_type();
  }

  bool nullable() override{
    return expr_->nullable();
  }

  int len() override {
    return 4;
  }

  RC get_value(const Tuple &tuple, Value &value) const override;
  RC try_get_value(Value &value) const override { return RC::SUCCESS; }

  const AggrType &_aggr_type_() const { return aggr_type_; }
  const std::unique_ptr<Expression> &_expr_() const { return expr_; }

  void getFields(std::vector<Field *> &query_fields) const override;
  static RC getAggrExprs(Expression *expr, std::vector<AggrExpr *> &aggr_exprs);

  AggrExpr* copy() const override {
    auto *res = new AggrExpr(aggr_type_, expr_->copy());
    res->set_name(name());
    res->set_alias(alias());
    return res;
  }

private:
  AggrType aggr_type_ = AggrType::AGGR_UNDEFINED;
  std::unique_ptr<Expression> expr_ = nullptr;
};
