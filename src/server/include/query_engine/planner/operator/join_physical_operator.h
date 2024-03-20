#pragma once

#include "include/query_engine/structor/tuple/join_tuple.h"
#include "physical_operator.h"

class NestedLoopJoinPhysicalOperator : public PhysicalOperator {
 public:
  NestedLoopJoinPhysicalOperator();
  ~NestedLoopJoinPhysicalOperator() override = default;

  PhysicalOperatorType type() const override {
    return PhysicalOperatorType::NESTED_LOOP_JOIN;
  }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;
  Tuple *current_tuple() override;

 private:
  Trx *trx_ = nullptr;

  //! 左表右表的真实对象是在PhysicalOperator::children_中，这里是为了写的时候更简单
  JoinedTuple joined_tuple_;  //! 当前关联的左右两个tuple
};
