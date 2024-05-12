#include "include/query_engine/planner/operator/update_physical_operator.h"

#include "common/log/log.h"
#include "include/storage_engine/recorder/record.h"
#include "include/storage_engine/recorder/table.h"
#include "include/storage_engine/transaction/trx.h"
#include "include/query_engine/parser/value.h"
#include "include/query_engine/analyzer/statement/update_stmt.h"
#include "include/query_engine/structor/expression/value_expression.h"
#include "include/query_engine/structor/tuple/row_tuple.h"

UpdatePhysicalOperator::~UpdatePhysicalOperator()
{
  // 这些 update_units 从逻辑算子中转移过来，独占所有权，需要释放
  for (auto &unit : update_units_) {
    delete unit.value;
  }
}

RC UpdatePhysicalOperator::open(Trx *trx)
{
  if (children_.empty()) {
    return RC::SUCCESS;
  }

  std::unique_ptr<PhysicalOperator> &child = children_[0];
  RC rc = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to init child operator: %s", strrc(rc));
    return rc;
  }

  trx_ = trx;

  Tuple *tuple = nullptr;
  const TableMeta &table_meta = table_->table_meta();
  const int field_num = table_meta.field_num() - table_meta.sys_field_num() - table_meta.null_filed_num();
  const int sys_field_num = table_meta.sys_field_num();
  std::vector<UpdateUnit> processed_update_units;
  for (const UpdateUnit& update_unit : update_units_) {
    Expression* expr = update_unit.value;
    Value value;
    expr->try_get_value(value);
    bool check = false;
    for (int i = 0; i < field_num; i++) {
      const FieldMeta *field_meta = table_meta.field(i + sys_field_num);
      if(update_unit.attribute_name == field_meta->name()) {
        const AttrType field_type = field_meta->type();
        const AttrType value_type = value.attr_type();
        if (field_type == TEXTS && value_type == CHARS) {
          if (value.get_string().size() > 65535) {
            return RC::INVALID_ARGUMENT;
          }
          std::hash<std::string> hashFunction;
          std::size_t hashValue = hashFunction(value.get_string());
          std::string value_search_for_text = std::string(table_->dir()) + "/" + std::string(table_->name()) + std::to_string(hashValue) + ".text";
          std::string value_stored_in_file = value.get_string();
          std::ofstream outputFile(value_search_for_text);
          if (outputFile.is_open()) {
            outputFile << value_stored_in_file;
            outputFile.close();
          } else {
            LOG_ERROR(" failed to write text value to file!");
          }
          value.set_text(value_search_for_text.c_str(), value_search_for_text.size());
          check = true;
          break;
        }
        if (AttrType::NULLS == value_type) {
          if ( !field_meta->nullable()) {
            LOG_WARN("field type mismatch. table=%s, field=%s, field type=%d, value_type=%d",
            table_->name(), field_meta->name(), field_type, value_type);
            return RC::SCHEMA_FIELD_TYPE_MISMATCH;
          }
        } else {
          if (field_type != value_type && type_cast_not_support(value_type, field_type)) {  // TODO try to convert the value type to field type
            LOG_WARN("field type mismatch. table=%s, field=%s, field type=%d, value_type=%d",
            table_->name(), field_meta->name(), field_type, value_type);
            return RC::SCHEMA_FIELD_TYPE_MISMATCH;
          }
        }
        check = true;
        break;
      }
    }
    if (!check) {
      LOG_WARN("schema mismatch. table name=%s, attribute name=%s", table_->name(), update_unit.attribute_name.c_str());
      return RC::SCHEMA_FIELD_MISSING;
    }
    UpdateUnit unit;
    unit.attribute_name = update_unit.attribute_name;
    unit.value = new ValueExpr(value);
    processed_update_units.emplace_back(unit);
  }
  for (auto &unit : update_units_) {
    delete unit.value;
  }
  update_units_.clear();
  update_units_ = std::move(processed_update_units);
  return RC::SUCCESS;
}

RC UpdatePhysicalOperator::next()
{
  RC rc = RC::SUCCESS;
  if (children_.empty()) {
    return RC::RECORD_EOF;
  }

  PhysicalOperator *child = children_[0].get();
  while (RC::SUCCESS == (rc = child->next())) {
    Tuple *tuple = child->current_tuple();
    if (nullptr == tuple) {
      LOG_WARN("failed to get current record: %s", strrc(rc));
      return rc;
    }
    int sys_field_num = table_->table_meta().sys_field_num();
    int cell_num = tuple->cell_num() - table_->table_meta().null_filed_num();
    std::vector<Value> values;
    for (int i = sys_field_num; i < cell_num; i ++) {
      Value value;
      bool check = false;
      for (const auto& update_unit : update_units_) {
        if (table_->table_meta().field(i)->name() == update_unit.attribute_name) {
          update_unit.value->try_get_value(value);
          values.push_back(value);
          check = true;
          break;
        }
      }
      if (!check) {
        rc = tuple->cell_at(i, value);
        if(rc != RC::SUCCESS) {
          break;
        }
        values.push_back(value);
      }
    }

    RowTuple *row_tuple = static_cast<RowTuple *>(tuple);

    Record oldRecord;
    oldRecord.set_rid(row_tuple->record().rid());
    char *tmp = (char *)malloc(row_tuple->record().len());
    ASSERT(nullptr != tmp, "failed to allocate memory. size=%d", row_tuple->record().len());
    memcpy(tmp, row_tuple->record().data(), row_tuple->record().len());
    oldRecord.set_data_owner(tmp, row_tuple->record().len());
    // Record oldRecord = row_tuple->record();
    rc = trx_->delete_record(table_, oldRecord);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to delete record: %s", strrc(rc));
      return rc;
    }

    Record updateRecord;
    RC rc = table_->make_record(static_cast<int>(values.size()), values.data(), updateRecord);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to make record. rc=%s", strrc(rc));
      return rc;
    }
    rc = trx_->insert_record(table_, updateRecord);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to insert record by transaction. rc=%s", strrc(rc));
      trx_->insert_record(table_, oldRecord);
      return rc;
    }
  }

  return RC::RECORD_EOF;
}

RC UpdatePhysicalOperator::close()
{
  if (!children_.empty()) {
    children_[0]->close();
  }
  return RC::SUCCESS;
}
