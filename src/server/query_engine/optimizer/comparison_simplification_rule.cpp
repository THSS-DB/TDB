#include "include/query_engine/optimizer/comparison_simplification_rule.h"

#include "include/query_engine/structor/expression/comparison_expression.h"
#include "include/query_engine/structor/expression/value_expression.h"

RC ComparisonSimplificationRule::rewrite(std::unique_ptr<Expression> &expr,
                                         bool &change_made) {
  RC rc = RC::SUCCESS;
  change_made = false;
  if (expr->type() == ExprType::COMPARISON) {
    auto *cmp_expr = dynamic_cast<ComparisonExpr *>(expr.get());
    Value value;
    RC sub_rc = cmp_expr->try_get_value(value);
    if (sub_rc == RC::SUCCESS) {
      std::unique_ptr<Expression> new_expr(new ValueExpr(value));
      expr.swap(new_expr);
      change_made = true;
    }
  }
  return rc;
}
