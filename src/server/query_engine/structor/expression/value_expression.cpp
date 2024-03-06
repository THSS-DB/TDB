#include "include/query_engine/structor/expression/value_expression.h"

RC ValuesExpr::value_in(const Value &value, Value &result) const {
  for (const Value &cache : values_) {
    if (cache.compare(value) == 0) {
      result.set_boolean(true);
      return RC::SUCCESS;
    }
  }
  result.set_boolean(false);
  return RC::SUCCESS;
}

RC ValuesExpr::value_exists(Value &value) const {
  value.set_boolean(!values_.empty());
  return RC::SUCCESS;
}