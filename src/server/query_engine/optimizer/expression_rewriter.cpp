#include "include/query_engine/optimizer/expression_rewriter.h"

#include "common/log/log.h"
#include "include/query_engine/optimizer/comparison_simplification_rule.h"
#include "include/query_engine/optimizer/conjunction_simplification_rule.h"
#include "include/query_engine/structor/expression/comparison_expression.h"
#include "include/query_engine/structor/expression/conjunction_expression.h"

ExpressionRewriter::ExpressionRewriter() {
  expr_rewrite_rules_.emplace_back(new ComparisonSimplificationRule);
  expr_rewrite_rules_.emplace_back(new ConjunctionSimplificationRule);
}

RC ExpressionRewriter::rewrite(std::unique_ptr<LogicalNode> &oper,
                               bool &change_made) {
  RC rc = RC::SUCCESS;

  bool sub_change_made = false;
  std::vector<std::unique_ptr<Expression>> &expressions = oper->expressions();
  for (std::unique_ptr<Expression> &expr : expressions) {
    rc = rewrite_expression(expr, sub_change_made);
    if (rc != RC::SUCCESS) {
      break;
    }

    if (sub_change_made && !change_made) {
      change_made = true;
    }
  }

  if (rc != RC::SUCCESS) {
    return rc;
  }

  std::vector<std::unique_ptr<LogicalNode>> &child_opers = oper->children();
  for (std::unique_ptr<LogicalNode> &child_oper : child_opers) {
    bool sub_change_made = false;
    rc = rewrite(child_oper, sub_change_made);
    if (sub_change_made && !change_made) {
      change_made = true;
    }
    if (rc != RC::SUCCESS) {
      break;
    }
  }
  return rc;
}

RC ExpressionRewriter::rewrite_expression(std::unique_ptr<Expression> &expr,
                                          bool &change_made) {
  RC rc = RC::SUCCESS;

  change_made = false;
  for (std::unique_ptr<ExpressionRewriteRule> &rule : expr_rewrite_rules_) {
    bool sub_change_made = false;
    rc = rule->rewrite(expr, sub_change_made);
    if (sub_change_made && !change_made) {
      change_made = true;
    }
    if (rc != RC::SUCCESS) {
      break;
    }
  }

  if (change_made || rc != RC::SUCCESS) {
    return rc;
  }

  switch (expr->type()) {
    case ExprType::FIELD:
    case ExprType::VALUE: {
      // do nothing
    } break;

    case ExprType::COMPARISON: {
      auto comparison_expr = static_cast<ComparisonExpr *>(expr.get());
      std::unique_ptr<Expression> &left_expr = comparison_expr->left();
      std::unique_ptr<Expression> &right_expr = comparison_expr->right();

      bool left_change_made = false;
      rc = rewrite_expression(left_expr, left_change_made);
      if (rc != RC::SUCCESS) {
        return rc;
      }

      bool right_change_made = false;
      if (right_expr != nullptr) {
        rc = rewrite_expression(right_expr, right_change_made);
        if (rc != RC::SUCCESS) {
          return rc;
        }
      }

      if (left_change_made || right_change_made) {
        change_made = true;
      }
    } break;

    case ExprType::CONJUNCTION: {
      auto conjunction_expr = static_cast<ConjunctionExpr *>(expr.get());
      std::vector<std::unique_ptr<Expression>> &children =
          conjunction_expr->children();
      for (std::unique_ptr<Expression> &child_expr : children) {
        bool sub_change_made = false;
        rc = rewrite_expression(child_expr, sub_change_made);
        if (rc != RC::SUCCESS) {
          LOG_WARN("failed to rewriter conjunction sub expression. rc=%s",
                   strrc(rc));
          return rc;
        }

        if (sub_change_made && !change_made) {
          change_made = true;
        }
      }
    } break;

    default: {
      // do nothing
    } break;
  }
  return rc;
}
