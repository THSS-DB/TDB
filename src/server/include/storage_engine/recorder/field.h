#pragma once

#include <cstring>
#include <string>

#include "include/storage_engine/recorder/field_meta.h"
#include "include/storage_engine/recorder/table.h"

/**
 * @brief 字段
 */
class Field {
 public:
  Field() = default;
  Field(const Table *table, const FieldMeta *field)
      : table_(table), field_(field) {}
  Field(const Field &) = default;

  const Table *table() const { return table_; }
  const FieldMeta *meta() const { return field_; }
  AttrType attr_type() const { return field_->type(); }
  const char *table_name() const { return table_->name(); }
  void set_table_alias(const std::string &s) { table_alias_ = s; }
  const char *table_alias() const { return table_alias_.c_str(); }
  const char *field_name() const { return field_->name(); }
  void set_table(const Table *table) { this->table_ = table; }
  void set_field(const FieldMeta *field) { this->field_ = field; }
  void set_int(Record &record, int value);
  int get_int(const Record &record);
  int len() { return field_->len(); }
  bool nullable() { return field_->nullable(); }

  const char *get_data(const Record &record);

  bool operator==(const Field &other) const {
    return !strcmp(table_->name(), other.table_->name()) &&
           !strcmp(field_->name(), other.field_->name()) &&
           !strcmp(table_alias_.c_str(), other.table_alias());
  }

 private:
  const Table *table_ = nullptr;
  const FieldMeta *field_ = nullptr;
  std::string table_alias_;
};
