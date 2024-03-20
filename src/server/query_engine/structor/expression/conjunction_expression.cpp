#include "include/query_engine/structor/expression/conjunction_expression.h"

#include "include/query_engine/structor/expression/comparison_expression.h"

RC ConjunctionExpr::get_value(const Tuple &tuple, Value &value) const {
  RC rc = RC::SUCCESS;
  if (children_.empty()) {
    value.set_boolean(true);
    return rc;
  }

  Value tmp_value;
  for (const std::unique_ptr<Expression> &expr : children_) {
    rc = expr->get_value(tuple, tmp_value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value by child expression. rc=%s", strrc(rc));
      return rc;
    }
    bool bool_value = tmp_value.get_boolean();
    if ((conjunction_type_ == ConjunctionType::AND && !bool_value) ||
        (conjunction_type_ == ConjunctionType::OR && bool_value)) {
      value.set_boolean(bool_value);
      return rc;
    }
  }

  bool default_value = (conjunction_type_ == ConjunctionType::AND);
  value.set_boolean(default_value);
  return rc;
}

RC ConjunctionExpr::set_trx(Trx *trx) const {
  for (const std::unique_ptr<Expression> &expr : children_) {
    const auto *compare_expr = dynamic_cast<const ComparisonExpr *>(expr.get());
    compare_expr->set_trx(trx);
  }
  return RC::SUCCESS;
}