#pragma once

#include <memory>

#include "src/server/include/common/rc.h"
#include "physical_operator.h"
#include "src/server/include/query_engine/planner/node/logical_node.h"

class TableGetLogicalNode;
class PredicateLogicalNode;
class OrderByLogicalNode;
class ProjectLogicalNode;
class AggrLogicalNode;
class InsertLogicalNode;
class DeleteLogicalNode;
class UpdateLogicalNode;
class ExplainLogicalNode;
class JoinLogicalNode;
class GroupByLogicalNode;

/**
 * @brief 物理算子树生成器
 * @ingroup PhysicalOperator
 * @details 根据逻辑节点树生成物理算子树。
 */
class PhysicalOperatorGenerator
{
public:
  PhysicalOperatorGenerator() = default;
  virtual ~PhysicalOperatorGenerator() = default;

  RC create(LogicalNode &logical_operator, std::unique_ptr<PhysicalOperator> &oper, bool is_delete = false);

private:
  RC create_plan(TableGetLogicalNode &logical_oper, std::unique_ptr<PhysicalOperator> &oper, bool is_delete = false);
  RC create_plan(PredicateLogicalNode &logical_oper, std::unique_ptr<PhysicalOperator> &oper, bool is_delete = false);
  RC create_plan(ProjectLogicalNode &logical_oper, std::unique_ptr<PhysicalOperator> &oper, bool is_delete = false);
  RC create_plan(AggrLogicalNode &logical_oper, std::unique_ptr<PhysicalOperator> &oper);
  RC create_plan(OrderByLogicalNode &logical_oper, std::unique_ptr<PhysicalOperator> &oper);
  RC create_plan(InsertLogicalNode &logical_oper, std::unique_ptr<PhysicalOperator> &oper);
  RC create_plan(DeleteLogicalNode &logical_oper, std::unique_ptr<PhysicalOperator> &oper);
  RC create_plan(UpdateLogicalNode &logical_oper, std::unique_ptr<PhysicalOperator> &oper);
  RC create_plan(ExplainLogicalNode &logical_oper, std::unique_ptr<PhysicalOperator> &oper, bool is_delete = false);
  RC create_plan(JoinLogicalNode &logical_oper, std::unique_ptr<PhysicalOperator> &oper);
};
