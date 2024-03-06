#pragma once

#include <vector>
#include "rewrite_rule.h"

/**
 * @brief 将一些谓词表达式下推到表数据扫描中
 * @ingroup Rewriter
 * @details 这样可以提前过滤一些数据
 */
class PredicatePushdownRewriter : public RewriteRule 
{
public:
  PredicatePushdownRewriter() = default;
  virtual ~PredicatePushdownRewriter() = default;

  RC rewrite(std::unique_ptr<LogicalNode> &oper, bool &change_made) override;

private:
  RC get_exprs_can_pushdown(
      std::unique_ptr<Expression> &expr, std::vector<std::unique_ptr<Expression>> &pushdown_exprs);
};
