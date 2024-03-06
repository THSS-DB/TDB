#include <regex>

#include "include/query_engine/structor/expression/comparison_expression.h"
#include "include/query_engine/structor/expression/value_expression.h"

static void replace_all(std::string &str, const std::string &from, const std::string &to)
{
  if (from.empty()) {
    return;
  }
  size_t pos = 0;
  while (std::string::npos != (pos = str.find(from, pos))) {
    str.replace(pos, from.length(), to);
    pos += to.length();  // in case 'to' contains 'from'
  }
}

RC ComparisonExpr::set_trx(Trx *trx) const {
  return RC::SUCCESS;
}

RC ComparisonExpr::compare_value(const Value &left, const Value &right, bool &result) const
{
  RC rc = RC::SUCCESS;
  // (NOT) EXISTS
  if (comp_ == CompOp::EXISTS || comp_ == CompOp::NOT_EXISTS) {
    Value exists;
    if (left_->type() == ExprType::VALUES) {
      auto *values_expr = dynamic_cast<ValuesExpr *>(left_.get());
      rc = values_expr->value_exists(exists);

    } else {
      return RC::INVALID_ARGUMENT;
    }

    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value from sub query. rc=%s", strrc(rc));
      return rc;
    }
    result = CompOp::EXISTS == comp_ ? exists.get_boolean() : !exists.get_boolean();
    return RC::SUCCESS;
  }

  // (NOT) IN
  if (comp_ == CompOp::IN || comp_ == CompOp::NOT_IN) {
    Value in;
    if (right_->type() == ExprType::VALUES) {
      auto *values_expr = dynamic_cast<ValuesExpr *>(right_.get());
      rc = values_expr->value_in(left, in);
    } else {
      return RC::INVALID_ARGUMENT;
    }

    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value from sub query. rc=%s", strrc(rc));
      return rc;
    }
    result = CompOp::IN == comp_ ? in.get_boolean() : !in.get_boolean();
    return RC::SUCCESS;
  }

  if (comp_ == CompOp::IS_NULL) {
    //assert(right.is_null());
    result = left.is_null();
    return RC::SUCCESS;
  }

  if (comp_ == CompOp::IS_NOT_NULL) {
    //assert(right.is_null());
    result = !left.is_null();
    return RC::SUCCESS;
  }

  if (left.is_null() || right.is_null()) {
    result = false;
    return RC::SUCCESS;
  }

  int cmp_result = left.compare(right);
  result = false;
  switch (comp_) {
    case EQUAL_TO: {
      result = (0 == cmp_result);
    } break;
    case LESS_EQUAL: {
      result = (cmp_result <= 0);
    } break;
    case NOT_EQUAL: {
      result = (cmp_result != 0);
    } break;
    case LESS_THAN: {
      result = (cmp_result < 0);
    } break;
    case GREAT_EQUAL: {
      result = (cmp_result >= 0);
    } break;
    case GREAT_THAN: {
      result = (cmp_result > 0);
    } break;
    case LIKE_OP:
    case NOT_LIKE_OP: {
      std::string raw_reg((const char *)right.data());
      replace_all(raw_reg, "_", "[^']");
      replace_all(raw_reg, "%", "[^']*");
      std::regex reg(raw_reg.c_str(), std::regex_constants::ECMAScript | std::regex_constants::icase);
      result = std::regex_match((const char *)left.data(), reg);
      if (NOT_LIKE_OP == comp_) {
        result = !result;
      }
    } break;
    default: {
      LOG_WARN("unsupported comparison. %d", comp_);
      rc = RC::INTERNAL;
    } break;
  }

  return rc;
}

RC ComparisonExpr::try_get_value(Value &cell) const
{
  if (left_->type() == ExprType::VALUE &&
      right_ != nullptr && right_->type() == ExprType::VALUE) {
    auto *left_value_expr = dynamic_cast<ValueExpr *>(left_.get());
    auto *right_value_expr = dynamic_cast<ValueExpr *>(right_.get());
    const Value &left_cell = left_value_expr->get_value();
    const Value &right_cell = right_value_expr->get_value();

    bool value = false;
    RC rc = compare_value(left_cell, right_cell, value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to compare tuple cells. rc=%s", strrc(rc));
    } else {
      cell.set_boolean(value);
    }
    return rc;
  }

  return RC::INVALID_ARGUMENT;
}

RC ComparisonExpr::get_value(const Tuple &tuple, Value &value) const
{
  Value left_value;
  Value right_value;

  RC rc = left_->get_value(tuple, left_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }
  if (right_ != nullptr) {
    rc = right_->get_value(tuple, right_value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
      return rc;
    }
  }

  bool bool_value = false;
  rc = compare_value(left_value, right_value, bool_value);
  if (rc == RC::SUCCESS) {
    value.set_boolean(bool_value);
  }
  return rc;
}