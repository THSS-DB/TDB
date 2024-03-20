#include "include/query_engine/planner/node/table_get_logical_node.h"

TableGetLogicalNode::TableGetLogicalNode(Table *table,
                                         const std::string &table_alias,
                                         const std::vector<Field> &fields,
                                         bool readonly)
    : table_(table),
      table_alias_(table_alias),
      fields_(fields),
      readonly_(readonly) {}

void TableGetLogicalNode::set_predicates(
    std::vector<std::unique_ptr<Expression>> &&exprs) {
  predicates_ = std::move(exprs);
}
