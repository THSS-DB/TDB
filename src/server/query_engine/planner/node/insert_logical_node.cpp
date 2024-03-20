#include "include/query_engine/planner/node/insert_logical_node.h"

InsertLogicalNode::InsertLogicalNode(Table *table, const std::vector<std::vector<Value>> &multi_values)
    : table_(table), multi_values_(multi_values)
{
}
