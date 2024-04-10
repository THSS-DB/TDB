#include "include/query_engine/optimizer/predicate_pushdown_rewriter.h"
#include "include/query_engine/planner/node/logical_node.h"
#include "include/query_engine/planner/node/table_get_logical_node.h"
#include "include/query_engine/structor/expression/value_expression.h"
#include "include/query_engine/structor/expression/conjunction_expression.h"
#include "include/query_engine/structor/expression/comparison_expression.h"

RC PredicatePushdownRewriter::rewrite(std::unique_ptr<LogicalNode> &oper, bool &change_made)
{
  RC rc = RC::SUCCESS;
  if (oper->type() != LogicalNodeType::PREDICATE) {
    return rc;
  }

  if (oper->children().size() != 1) {
    return rc;
  }

  std::unique_ptr<LogicalNode> &child_oper = oper->children().front();
  if (child_oper->type() != LogicalNodeType::TABLE_GET) {
    return rc;
  }

  auto table_get_oper = static_cast<TableGetLogicalNode *>(child_oper.get());

  std::vector<std::unique_ptr<Expression>> &predicate_oper_exprs = oper->expressions();
  if (predicate_oper_exprs.size() != 1) {
    return rc;
  }

  std::unique_ptr<Expression> &predicate_expr = predicate_oper_exprs.front();
  std::vector<std::unique_ptr<Expression>> pushdown_exprs;
  rc = get_exprs_can_pushdown(predicate_expr, pushdown_exprs);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get exprs can pushdown. rc=%s", strrc(rc));
    return rc;
  }

  if (!predicate_expr) {
    // 所有的表达式都下推到了下层算子
    // 这个predicate operator其实就可以不要了。但是这里没办法删除，弄一个空的表达式吧
    LOG_TRACE("all expressions of predicate operator were pushdown to table get operator, then make a fake one");

    Value value((bool)true);
    predicate_expr = std::unique_ptr<Expression>(new ValueExpr(value));
  }

  if (!pushdown_exprs.empty()) {
    change_made = true;
    table_get_oper->set_predicates(std::move(pushdown_exprs));
  }
  return rc;
}

/**
 * 查看表达式是否可以直接下放到table get算子的filter
 * @param expr 是当前的表达式。如果可以下放给table get 算子，执行完成后expr就失效了
 * @param pushdown_exprs 当前所有要下放给table get 算子的filter。此函数执行多次，
 *                       pushdown_exprs 只会增加，不要做清理操作
 */
RC PredicatePushdownRewriter::get_exprs_can_pushdown(
    std::unique_ptr<Expression> &expr, std::vector<std::unique_ptr<Expression>> &pushdown_exprs)
{
  RC rc = RC::SUCCESS;
  if (expr->type() == ExprType::CONJUNCTION) {
    ConjunctionExpr *conjunction_expr = static_cast<ConjunctionExpr *>(expr.get());
    // 或 操作的比较，太复杂，现在不考虑
    if (conjunction_expr->conjunction_type() == ConjunctionType::OR) {
      return rc;
    }

    std::vector<std::unique_ptr<Expression>> &child_exprs = conjunction_expr->children();
    for (auto iter = child_exprs.begin(); iter != child_exprs.end();) {
      // 对每个子表达式，判断是否可以下放到table get 算子
      // 如果可以的话，就从当前孩子节点中删除他
      rc = get_exprs_can_pushdown(*iter, pushdown_exprs);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to get pushdown expressions. rc=%s", strrc(rc));
        return rc;
      }

      if (!*iter) {
        child_exprs.erase(iter);
      } else {
        ++iter;
      }
    }
  } else if (expr->type() == ExprType::COMPARISON) {
    // 如果是比较操作，并且比较的左边或右边是表某个列值，那么就下推下去
    auto comparison_expr = static_cast<ComparisonExpr *>(expr.get());
    std::unique_ptr<Expression> &left_expr = comparison_expr->left();
    std::unique_ptr<Expression> &right_expr = comparison_expr->right();
    // 比较操作的左右两边只要有一个是取列字段值的并且另一边也是取字段值或常量，就pushdown
    if (left_expr->type() != ExprType::FIELD && right_expr->type() != ExprType::FIELD) {
      return rc;
    }
    if (left_expr->type() != ExprType::FIELD && left_expr->type() != ExprType::VALUE &&
        right_expr->type() != ExprType::FIELD && right_expr->type() != ExprType::VALUE) {
      return rc;
    }

    pushdown_exprs.emplace_back(std::move(expr));
  }
  return rc;
}
