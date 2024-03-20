#include "include/query_engine/planner/operator/table_scan_physical_operator.h"

#include "include/storage_engine/recorder/table.h"

using namespace std;

RC TableScanPhysicalOperator::open(Trx *trx) {
  RC rc = table_->get_record_scanner(record_scanner_, trx, readonly_);
  if (rc == RC::SUCCESS) {
    tuple_.set_schema(table_, table_alias_, table_->table_meta().field_metas());
  }
  trx_ = trx;
  return rc;
}

RC TableScanPhysicalOperator::next() {
  if (!record_scanner_.has_next()) {
    return RC::RECORD_EOF;
  }

  RC rc = RC::SUCCESS;
  bool filter_result = false;
  while (record_scanner_.has_next()) {
    rc = record_scanner_.next(current_record_);
    if (rc != RC::SUCCESS) {
      return rc;
    }

    tuple_._set_record(&current_record_);
    rc = filter(tuple_, filter_result);
    if (rc != RC::SUCCESS) {
      return rc;
    }

    if (filter_result) {
      break;
    } else {
      rc = RC::RECORD_EOF;
    }
  }
  return rc;
}

RC TableScanPhysicalOperator::close() { return record_scanner_.close_scan(); }

Tuple *TableScanPhysicalOperator::current_tuple() {
  if (tuple_.order_set()) {
    tuple_.remove_order_set();
    return &tuple_;
  }
  tuple_._set_record(&current_record_);
  return &tuple_;
}

string TableScanPhysicalOperator::param() const { return table_->name(); }

void TableScanPhysicalOperator::set_predicates(
    vector<unique_ptr<Expression>> &&exprs) {
  predicates_ = std::move(exprs);
}

RC TableScanPhysicalOperator::filter(RowTuple &tuple, bool &result) {
  RC rc = RC::SUCCESS;
  Value value;
  for (unique_ptr<Expression> &expr : predicates_) {
    rc = expr->get_value(tuple, value);
    if (rc != RC::SUCCESS) {
      return rc;
    }

    bool tmp_result = value.get_boolean();
    if (!tmp_result) {
      result = false;
      return rc;
    }
  }

  result = true;
  return rc;
}
