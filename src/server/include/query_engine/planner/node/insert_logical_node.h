#pragma once

#include <vector>

#include "logical_node.h"
#include "include/query_engine/parser/parse_defs.h"

class InsertLogicalNode : public LogicalNode
{
public:
  InsertLogicalNode(Table *table, const std::vector<std::vector<Value>> &multi_values);
  ~InsertLogicalNode() override = default;

  LogicalNodeType type() const override
  {
    return LogicalNodeType::INSERT;
  }

  Table *table() const { return table_; }

  std::vector<std::vector<Value>> & multi_values() { return multi_values_; }
  std::vector<Value> & values(int record_idx) { return multi_values_[record_idx]; }

private:
  Table *table_ = nullptr;
  std::vector<std::vector<Value>> multi_values_;
};