#include "common/log/log.h"
#include "include/query_engine/planner/operator/aggr_physical_operator.h"
#include "storage/table/table.h"

RC AggrPhysicalOperator::open(Trx *trx)
{
  if (children_.empty()) {
    return RC::SUCCESS;
  }

  PhysicalOperator *child = children_[0].get();
  RC rc = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to init child operator: %s", strrc(rc));
    return rc;
  }

  aggr_init();

  return RC::SUCCESS;
}

RC AggrPhysicalOperator::next()
{
  RC rc;
  if (children_.empty()) {
    return RC::RECORD_EOF;
  }

  PhysicalOperator *child = children_[0].get();
  bool aggr_flag = false;
  while (RC::SUCCESS == (rc = child->next())) {
    aggr_flag = true;
    Tuple *tuple = child->current_tuple();
    if (nullptr == tuple) {
      LOG_WARN("failed to get current record: %s", strrc(rc));
      return rc;
    }

    for (int i = 0; i < aggr_fields_.size(); i ++) {
      const auto& aggr_field = aggr_fields_[i];
      if (0 == strcmp(aggr_field.field_name(), "*")) {
        all_null_[i] = false;
        counts_[i] ++;
        continue;
      }
      Value value;
      const TupleCellSpec *spec =
          new TupleCellSpec(aggr_field.table_name(), aggr_field.field_name(), aggr_field.table_alias());
      rc = tuple->find_cell(*spec, value);
      if(rc != RC::SUCCESS) {
        return rc;
      }
      if (value.is_null()) {
        continue;
      }
      all_null_[i] = false;
      counts_[i] ++;
      aggr_update(aggr_types_[i], aggr_results_[i], value);
    }
    is_first_called_ = false;
  }
  if (aggr_flag || is_first_called_) {
    aggr_done();
    is_first_called_ = false;
    return RC::SUCCESS;
  } else {
    return RC::RECORD_EOF;
  }

}

RC AggrPhysicalOperator::close()
{
  if (!children_.empty()) {
    children_[0]->close();
  }
  return RC::SUCCESS;
}

Tuple *AggrPhysicalOperator::current_tuple()
{
  return &tuple_;
}

void AggrPhysicalOperator::aggr_init() {
  is_first_called_ = true;
  counts_.resize(aggr_fields_.size());
  all_null_.resize(aggr_fields_.size());
  aggr_results_.resize(aggr_fields_.size());
  for (size_t i = 0; i < all_null_.size(); ++i) {
    all_null_[i] = true;
    counts_[i]   = 0;
    aggr_results_[i].set_null();
  }
}
void AggrPhysicalOperator::aggr_update(AggrType aggr_type, Value& aggr_result, Value& value) {
  if(aggr_result.is_null()) {
    aggr_result = value;
    return;
  }
  switch (aggr_type) {
    case AGGR_MIN: {
      int cmp_result = aggr_result.compare(value);
      if (cmp_result > 0) {
        aggr_result = value;
      }
      break;
    }
    case AGGR_MAX: {
      int cmp_result = aggr_result.compare(value);
      if (cmp_result < 0) {
        aggr_result = value;
      }
      break;
    }
    case AGGR_AVG:
    case AGGR_SUM:{
      switch (value.attr_type()) {
        case INTS: {
          aggr_result.set_int(aggr_result.get_int() + value.get_int());
          break;
        }
        case FLOATS: {
          aggr_result.set_float(aggr_result.get_float() + value.get_float());
          break;
        }
        default: break;
      }
    } break;
    default:
      LOG_ERROR("Unsupported AggrFuncType");
  }

}
void AggrPhysicalOperator::aggr_done() {
  for (int i = 0; i < aggr_fields_.size(); i ++) {
    if (all_null_[i] && AGGR_COUNT != aggr_types_[i]) {
      aggr_results_[i].set_null();
      continue;
    }
    switch (aggr_types_[i]) {
      case AGGR_COUNT:{
        aggr_results_[i].set_type(AttrType::INTS);
        aggr_results_[i].set_int(counts_[i]);
      } break;
      case AGGR_AVG: {
        float sum = aggr_results_[i].get_float();
        if (counts_[i] != 0) {
          aggr_results_[i].set_float(sum / (float)counts_[i]);
        }
        break;
      }
      default: break;
    }
  }
  tuple_.set_tuple(alias_, aggr_results_);
}