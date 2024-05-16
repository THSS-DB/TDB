#include "include/query_engine/planner/node/update_logical_node.h"

UpdateLogicalNode::UpdateLogicalNode(Table *table, std::vector<UpdateUnit> update_units) : table_(table), update_units_(std::move(update_units))
{}

UpdateLogicalNode::~UpdateLogicalNode()
{
  // update_units_ 在 plan_node 中 copy 了一份，这里需要释放
  for (auto &unit : update_units_) {
    delete unit.value;
  }
}
