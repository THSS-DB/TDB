#pragma once

#include <memory>

#include "include/common/rc.h"

class LogicalNode;
class Expression;

/**
 * @brief 逻辑计划的重写规则
 * @ingroup Rewriter
 */
class RewriteRule {
 public:
  virtual ~RewriteRule() = default;

  virtual RC rewrite(std::unique_ptr<LogicalNode> &oper, bool &change_made) = 0;
};

/**
 * @brief 表达式的重写规则
 * @ingroup Rewriter
 */
class ExpressionRewriteRule {
 public:
  virtual ~ExpressionRewriteRule() = default;

  virtual RC rewrite(std::unique_ptr<Expression> &expr, bool &change_made) = 0;
};
