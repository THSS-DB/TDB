#pragma once

#include "rewrite_rule.h"

class LogicalNode;

/**
 * @brief 简化多个表达式联结的运算
 * @ingroup Rewriter
 * @details 比如只有一个表达式，或者表达式可以直接出来
 */
class ConjunctionSimplificationRule : public ExpressionRewriteRule 
{
public:
  ConjunctionSimplificationRule() = default;
  virtual ~ConjunctionSimplificationRule() = default;

  RC rewrite(std::unique_ptr<Expression> &expr, bool &change_made) override;

private:
};
