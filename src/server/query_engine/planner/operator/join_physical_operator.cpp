#include "include/query_engine/planner/operator/join_physical_operator.h"

// TODO [Lab3] join SQL的算子实现
NestedLoopJoinPhysicalOperator::NestedLoopJoinPhysicalOperator()
= default;

RC NestedLoopJoinPhysicalOperator::open(Trx *trx)
{
  return RC::SUCCESS;
}

RC NestedLoopJoinPhysicalOperator::next()
{
  return RC::RECORD_EOF;
}

// TODO [Lab3] 清理左右子算子
RC NestedLoopJoinPhysicalOperator::close()
{
  return RC::SUCCESS;
}

Tuple *NestedLoopJoinPhysicalOperator::current_tuple()
{
  return nullptr;
}
