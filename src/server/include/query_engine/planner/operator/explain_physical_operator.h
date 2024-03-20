#pragma once

#include "include/query_engine/structor/tuple/values_tuple.h"
#include "physical_operator.h"

/**
 * @brief Explain物理算子
 * @ingroup PhysicalOperator
 */
class ExplainPhysicalOperator : public PhysicalOperator {
 public:
  ExplainPhysicalOperator() = default;
  virtual ~ExplainPhysicalOperator() = default;

  PhysicalOperatorType type() const override {
    return PhysicalOperatorType::EXPLAIN;
  }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;
  Tuple *current_tuple() override;

 private:
  void to_string(std::ostream &os, PhysicalOperator *oper, int level,
                 bool last_child, std::vector<bool> &ends);

 private:
  std::string physical_plan_;
  ValueListTuple tuple_;
};
