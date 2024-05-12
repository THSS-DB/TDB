#pragma once

#include "logical_node.h"

/**
 * @brief 逻辑算子，用于执行update语句
 * @ingroup LogicalNode
 */
class UpdateLogicalNode : public LogicalNode
{
public:
  UpdateLogicalNode(Table *table, std::vector<UpdateUnit> update_units);
  ~UpdateLogicalNode() override;

  LogicalNodeType type() const override
  {
    return LogicalNodeType::UPDATE;
  }
  Table *table() const {
    return table_;
  }
  std::vector<UpdateUnit> &update_units() {
    return update_units_;
  }
private:
  Table *table_ = nullptr;
  std::vector<UpdateUnit> update_units_;
};
