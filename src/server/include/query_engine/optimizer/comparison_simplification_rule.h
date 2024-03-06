#pragma once

#include "include/common/rc.h"
#include "rewrite_rule.h"

class LogicalNode;

/**
 * @brief 简单比较的重写规则
 * @ingroup Rewriter
 * @details 如果有简单的比较运算，比如比较的两边都是常量，那我们就可以在运行执行计划之前就知道结果，
 * 进而直接将表达式改成结果，这样就可以减少运行时的计算量。
 */
class ComparisonSimplificationRule : public ExpressionRewriteRule 
{
public:
  ComparisonSimplificationRule() = default;
  virtual ~ComparisonSimplificationRule() = default;

  RC rewrite(std::unique_ptr<Expression> &expr, bool &change_made) override;

private:
};
