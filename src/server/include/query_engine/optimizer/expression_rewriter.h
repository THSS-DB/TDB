#pragma once

#include <memory>

#include "include/common/rc.h"
#include "include/query_engine/planner/node/logical_node.h"
#include "include/query_engine/structor/expression/expression.h"
#include "rewrite_rule.h"

class ExpressionRewriter : public RewriteRule {
 public:
  ExpressionRewriter();
  virtual ~ExpressionRewriter() = default;

  RC rewrite(std::unique_ptr<LogicalNode> &oper, bool &change_made) override;

 private:
  RC rewrite_expression(std::unique_ptr<Expression> &expr, bool &change_made);

 private:
  std::vector<std::unique_ptr<ExpressionRewriteRule>> expr_rewrite_rules_;
};
