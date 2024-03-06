#pragma once

#include <memory>
#include <vector>

#include "include/query_engine/structor/expression/expression.h"

enum class LogicalNodeType
{
  TABLE_GET,  ///< 从表中获取数据
  PREDICATE,  ///< 过滤，就是谓词
  PROJECTION, ///< 投影，就是select
  AGGR,       ///< 聚合
  JOIN,       ///< 连接
  ORDER,      ///< 排序
  INSERT,     ///< 插入
  DELETE,     ///< 删除，删除可能会有子查询
  UPDATE,     ///< 更新
  EXPLAIN,    ///< 查看执行计划
  GROUP_BY    ///< Group By
};

class LogicalNode
{
public:
  LogicalNode() = default;
  virtual ~LogicalNode();

  virtual LogicalNodeType type() const = 0;

  void add_child(std::unique_ptr<LogicalNode> oper);
  std::vector<std::unique_ptr<LogicalNode>> &children()
  {
    return children_;
  }
  std::vector<std::unique_ptr<Expression>> &expressions()
  {
    return expressions_;
  }

protected:
  std::vector<std::unique_ptr<LogicalNode>> children_;  ///< 子算子
  ///< 表达式，比如select中的列，where中的谓词等等，都可以使用表达式来表示
  ///< 表达式能是一个常量，也可以是一个函数，也可以是一个列，也可以是一个子查询等等
  std::vector<std::unique_ptr<Expression>> expressions_;    
};
