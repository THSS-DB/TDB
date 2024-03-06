#pragma once

#include "rewrite_rule.h"

/**
 * @brief 谓词重写规则
 * @ingroup Rewriter
 * @details 有些谓词可以在真正运行之前就知道结果，那么就可以提前运算出来，比如1=1,1=0。
 */
class PredicateRewriteRule : public RewriteRule 
{
public:
  PredicateRewriteRule() = default;
  virtual ~PredicateRewriteRule() = default;

  RC rewrite(std::unique_ptr<LogicalNode> &oper, bool &change_made) override;
};
