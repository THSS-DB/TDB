#include "include/query_engine/planner/operator/insert_physical_operator.h"

#include "include/query_engine/analyzer/statement/insert_stmt.h"
#include "include/storage_engine/recorder/table.h"
#include "include/storage_engine/transaction/trx.h"

using namespace std;

InsertPhysicalOperator::InsertPhysicalOperator(
    Table *table, std::vector<std::vector<Value>> &&multi_values)
    : table_(table), multi_values_(std::move(multi_values)) {}

RC InsertPhysicalOperator::open(Trx *trx) {
  RC rc;
  for (int i = 0; i < multi_values_.size(); i++) {
    Record record;
    rc = table_->make_record(static_cast<int>(multi_values_[i].size()),
                             multi_values_[i].data(), record);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to make record. rc=%s", strrc(rc));
      return rc;
    }

    rc = trx->insert_record(table_, record);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to insert record by transaction. rc=%s", strrc(rc));
      return rc;
    }
  }
  return rc;
}

RC InsertPhysicalOperator::next() { return RC::RECORD_EOF; }

RC InsertPhysicalOperator::close() { return RC::SUCCESS; }
