#include "include/query_engine/planner/operator/physical_operator.h"

std::string physical_operator_type_name(PhysicalOperatorType type)
{
  switch (type) {
    case PhysicalOperatorType::TABLE_SCAN:
      return "TABLE_SCAN";
    case PhysicalOperatorType::INDEX_SCAN:
      return "INDEX_SCAN";
    case PhysicalOperatorType::JOIN:
      return "JOIN";
    case PhysicalOperatorType::EXPLAIN:
      return "EXPLAIN";
    case PhysicalOperatorType::PREDICATE:
      return "PREDICATE";
    case PhysicalOperatorType::INSERT:
      return "INSERT";
    case PhysicalOperatorType::DELETE:
      return "DELETE";
    case PhysicalOperatorType::PROJECT:
      return "PROJECT";
    case PhysicalOperatorType::STRING_LIST:
      return "STRING_LIST";
    case PhysicalOperatorType::AGGREGATION:
      return "AGGREGATION";
    case PhysicalOperatorType::GROUP_BY:
      return "GROUP_BY";
    case PhysicalOperatorType::ORDER_BY:
      return "ORDER_BY";
    default:
      return "UNKNOWN";
  }
}

PhysicalOperator::~PhysicalOperator()
{}

std::string PhysicalOperator::name() const
{
  return physical_operator_type_name(type());
}

std::string PhysicalOperator::param() const
{
  return "";
}
