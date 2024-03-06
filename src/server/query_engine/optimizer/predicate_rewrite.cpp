#include "include/query_engine/optimizer/predicate_rewrite.h"
#include "include/query_engine/planner/node/logical_node.h"
#include "include/query_engine/structor/expression/value_expression.h"

RC PredicateRewriteRule::rewrite(std::unique_ptr<LogicalNode> &oper, bool &change_made)
{
  std::vector<std::unique_ptr<LogicalNode>> &child_opers = oper->children();
  if (child_opers.size() != 1) {
    return RC::SUCCESS;
  }

  auto &child_oper = child_opers.front();
  if (child_oper->type() != LogicalNodeType::PREDICATE) {
    return RC::SUCCESS;
  }

  std::vector<std::unique_ptr<Expression>> &expressions = child_oper->expressions();
  if (expressions.size() != 1) {
    return RC::SUCCESS;
  }

  std::unique_ptr<Expression> &expr = expressions.front();
  if (expr->type() != ExprType::VALUE) {
    return RC::SUCCESS;
  }

  // 如果仅有的一个子节点是predicate
  // 并且这个子节点可以判断为恒为TRUE，那么可以省略这个子节点，并把他的子节点们（就是孙子节点）接管过来
  // 如果可以判断恒为false，那么就可以删除子节点
  auto value_expr = static_cast<ValueExpr *>(expr.get());
  bool bool_value = value_expr->get_value().get_boolean();
  if (true == bool_value) {
    std::vector<std::unique_ptr<LogicalNode>> grand_child_opers;
    grand_child_opers.swap(child_oper->children());
    child_opers.clear();
    for (auto &grand_child_oper : grand_child_opers) {
      oper->add_child(std::move(grand_child_oper));
    }
  } else {
    child_opers.clear();
  }

  change_made = true;
  return RC::SUCCESS;
}
