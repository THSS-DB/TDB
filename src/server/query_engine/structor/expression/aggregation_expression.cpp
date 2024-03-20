#include "include/query_engine/structor/expression/aggregation_expression.h"

#include "include/query_engine/structor/expression/arithmetic_expression.h"
#include "include/query_engine/structor/tuple/tuple.h"

RC AggrExpr::get_value(const Tuple &tuple, Value &value) const {
  TupleCellSpec spec(name().c_str());
  tuple.find_cell(spec, value);
  return RC::SUCCESS;
}

void AggrExpr::getFields(std::vector<Field *> &query_fields) const {
  expr_->getFields(query_fields);
}

RC AggrExpr::getAggrExprs(Expression *expr,
                          std::vector<AggrExpr *> &aggr_exprs) {
  if (expr->type() == ExprType::ARITHMETIC) {
    auto *arithmetic_expr = dynamic_cast<ArithmeticExpr *>(expr);
    getAggrExprs(arithmetic_expr->left().get(), aggr_exprs);
    if (arithmetic_expr->right() != nullptr) {
      getAggrExprs(arithmetic_expr->right().get(), aggr_exprs);
    }

  } else if (expr->type() == ExprType::AGGR) {
    auto *aggr_expr = dynamic_cast<AggrExpr *>(expr);
    aggr_exprs.emplace_back(aggr_expr->copy());
  }
  return RC::SUCCESS;
}