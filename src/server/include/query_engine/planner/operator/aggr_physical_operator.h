#pragma once

#include "physical_operator.h"
#include "include/query_engine/planner/node/aggr_logical_node.h"
#include "include/query_engine/structor/tuple/aggregation_tuple.h"

class AggrPhysicalOperator : public PhysicalOperator
{
public:
  AggrPhysicalOperator(AggrLogicalNode *logical_operator) {
    for (int i = 0; i < logical_operator->_alias_().size(); i++) {
      alias_.emplace_back(logical_operator->_alias_()[i]);
      aggr_types_.emplace_back(logical_operator->_aggr_types_()[i]);
      aggr_fields_.emplace_back(logical_operator->_aggr_fields_()[i]);
    }
  }


  ~AggrPhysicalOperator() override = default;

  PhysicalOperatorType type() const override
  {
    return PhysicalOperatorType::AGGREGATION;
  }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  int cell_num() const
  {
    return tuple_.cell_num();
  }

  Tuple *current_tuple() override;

private:
  std::vector<std::string> alias_;
  std::vector<AggrType> aggr_types_;
  std::vector<Field> aggr_fields_;

  std::vector<Value> aggr_results_;
  std::vector<bool> all_null_;
  std::vector<int> counts_;
  bool is_first_called_;
  AggrTuple tuple_;
  void aggr_init();
  void aggr_update(AggrType aggr_type, Value& aggr_result, Value& value);
  void aggr_done();
};