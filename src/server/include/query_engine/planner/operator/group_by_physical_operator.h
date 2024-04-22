#include "physical_operator.h"
#include "include/query_engine/planner/node/group_by_logical_node.h"
#include "include/query_engine/structor/tuple/aggregation_tuple.h"
#include "include/query_engine/structor/tuple/join_tuple.h"

/**
 * TODO GroupBy算子的具体实现
 */
class GroupByPhysicalOperator : public PhysicalOperator {

public:

  GroupByPhysicalOperator(GroupByLogicalNode *logical_oper);
  virtual ~GroupByPhysicalOperator() = default;

  PhysicalOperatorType type() const override {
    return PhysicalOperatorType::GROUP_BY;
  }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  Tuple *current_tuple() override;
};