#include "common/log/log.h"
#include "include/query_engine/planner/operator/order_physical_operator.h"
#include "include/storage_engine/recorder/record.h"
#include "include/query_engine/analyzer/statement/filter_stmt.h"
#include "include/storage_engine/recorder/field.h"
#include <algorithm>

OrderPhysicalOperator::OrderPhysicalOperator(std::vector<OrderByUnit *> order_units) : order_units_(std::move(order_units))
{}

RC OrderPhysicalOperator::open(Trx *trx)
{
  if (children_.size() != 1) {
    LOG_WARN("order operator must has one child");
    return RC::INTERNAL;
  }

  return children_[0]->open(trx);
}

RC OrderPhysicalOperator::next()
{
  RC rc = RC::SUCCESS;
  if (is_init_) {
    is_init_ = false;
    rc = sort_table();
    if (rc != RC::SUCCESS) {
      return rc;
    }
  }

  if (it_ != st_idx_.end()) {
    children_[0]->current_tuple()->set_record(st_[*it_]);
    it_ ++;
    return RC::SUCCESS;
  }
  return RC::RECORD_EOF;
}

RC OrderPhysicalOperator::close()
{
  children_[0]->close();
  return RC::SUCCESS;
}

Tuple *OrderPhysicalOperator::current_tuple()
{
  return children_[0]->current_tuple();
}

RC OrderPhysicalOperator::sort_table() {
  RC rc = RC::SUCCESS;

  std::vector<std::pair<std::vector<Value>, int>> tuple_table;
  std::vector<Value> tuple_values;

  int index = 0;
  while (RC::SUCCESS == (rc = children_[0]->next())) {
    tuple_values.clear();
    for (const OrderByUnit *unit : order_units_) {
      Expression *expr = unit->expr();
      Value value;
      expr->get_value(*children_[0]->current_tuple(), value);
      tuple_values.emplace_back(value);
    }
    tuple_table.emplace_back(tuple_values, index ++);
    std::vector<Record *> records;
    children_[0]->current_tuple()->get_record(records);
    for (auto &rcd_ptr : records) {
      rcd_ptr = new Record(*rcd_ptr);
    }
    st_.emplace_back(records);
  }
  if (RC::RECORD_EOF != rc) {
    LOG_ERROR("Fetch Table Error In SortOperator. RC: %d", rc);
    return rc;
  }

  bool is_asc[order_units_.size()];
  for (int i = 0; i < order_units_.size(); ++i) {
    is_asc[i] = order_units_[i]->sort_type();
  }

  auto cmp = [&is_asc](const std::pair<std::vector<Value>, int> &a, const std::pair<std::vector<Value>, int> &b) {
    auto &cells_a = a.first;
    auto &cells_b = b.first;
    assert(cells_a.size() == cells_b.size());
    for (int i = 0; i < cells_a.size(); i ++) {
      auto &cell_a = cells_a[i];
      auto &cell_b = cells_b[i];
      if (cell_a.is_null() && cell_b.is_null()) {
        continue;
      }
      if (cell_a.is_null()) {
        return is_asc[i];
      }
      if (cell_b.is_null()) {
        return !is_asc[i];
      }
      int cmp = cell_a.compare(cell_b);
      if (cmp != 0) {
        return is_asc[i] ? cmp < 0: cmp > 0;
      }
    }
    return false;  // completely same
  };
  std::sort(tuple_table.begin(), tuple_table.end(), cmp);

  for (auto & i : tuple_table) {
    st_idx_.emplace_back(i.second);
  }
  it_ = st_idx_.begin();
  return RC::SUCCESS;
}
