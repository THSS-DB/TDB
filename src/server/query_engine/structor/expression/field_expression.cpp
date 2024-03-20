#include "include/query_engine/structor/expression/field_expression.h"
#include "include/query_engine/structor/expression/value_expression.h"
#include "include/query_engine/structor/tuple/tuple.h"

RC FieldExpr::get_value(const Tuple &tuple, Value &value) const
{
  TupleCellSpec tmp(table_name(), field_name(), field_.table_alias());
  RC rc = tuple.find_cell(tmp, value);
  return rc;
}

RC ValueExpr::get_value(const Tuple &tuple, Value &value) const
{
  value = value_;
  return RC::SUCCESS;
}

void FieldExpr::getFields(std::vector<Field *> &query_fields) const {
  bool exist = false;
  for (auto *f : query_fields) {
    if (*f == field_) {
      exist = true;
      break;
    }
  }
  if (!exist) {
    query_fields.emplace_back(new Field(field_));
  }
}
