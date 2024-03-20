#pragma once

#include <memory>
#include <vector>
#include "rewrite_rule.h"

class LogicalNode;

/**
 * @defgroup Rewriter 
 * @brief 根据规则对逻辑计划进行重写
 */

/**
 * @brief 根据一些规则对逻辑计划进行重写
 * @ingroup Rewriter
 * @details 当前仅实现了一两个非常简单的规则。
 * 重写包括对逻辑计划和计划中包含的表达式。
 */
class Rewriter 
{
public:
  Rewriter();
  virtual ~Rewriter() = default;

  /**
   * @brief 对逻辑计划进行重写
   * @details 如果重写发生，change_made为true，否则为false。
   * 通常情况下如果改写发生改变，就会继续重写，直到没有改变为止。
   * @param oper 逻辑计划
   * @param change_made 当前是否有重写发生
   */
  RC rewrite(std::unique_ptr<LogicalNode> &oper, bool &change_made);

private:
  std::vector<std::unique_ptr<RewriteRule>> rewrite_rules_;
};
