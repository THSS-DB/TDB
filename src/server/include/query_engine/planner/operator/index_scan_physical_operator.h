#pragma once

#include "include/query_engine/structor/tuple/row_tuple.h"
#include "include/storage_engine/recorder/record_manager.h"
#include "physical_operator.h"

/**
 * TODO [Lab3]
 * 通过索引来扫描文件,与TableScanOperator扮演同等的角色.
 * 需要修改Rewrite逻辑,在存在索引的时候将TableScanNode更换为IndexScanNode
 * 同时补充physical_operator_generator逻辑,基于IndexScanNode生成IndexScanOperator
 */
class [[maybe_unused]] IndexScanPhysicalOperator : public PhysicalOperator {
 public:
  IndexScanPhysicalOperator() = default;
  ~IndexScanPhysicalOperator() override = default;
};
