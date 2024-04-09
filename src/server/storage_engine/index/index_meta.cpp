#include "include/storage_engine/index/index_meta.h"

const static Json::StaticString FIELD_NAME("name");
const static Json::StaticString FIELD_AMOUNT("field_amount");
const static Json::StaticString FIELD_FIELD_NAME("field_name");
const static Json::StaticString UNIQUE_FLAG("is_unique");

RC IndexMeta::init(bool is_unique, const char *name, std::vector<const FieldMeta *> &multi_fields)
{
  if (common::is_blank(name)) {
    LOG_ERROR("Failed to init index, name is empty.");
    return RC::INVALID_ARGUMENT;
  }
  is_unique_ = is_unique;
  name_ = name;
  for (int i = 0; i < multi_fields.size(); i++) {
    multi_fields_.emplace_back(multi_fields[i]->name());
  }
  return RC::SUCCESS;
}

void IndexMeta::to_json(Json::Value &json_value) const
{
  json_value[UNIQUE_FLAG] = is_unique_;
  json_value[FIELD_NAME] = name_;
  json_value[FIELD_AMOUNT] = std::to_string(multi_fields_.size());
  std::string multi_fields_names = "";
  for (int i = 0; i < multi_fields_.size() - 1; i++) {
    multi_fields_names += multi_fields_[i];
    multi_fields_names += " ";
  }
  multi_fields_names += multi_fields_[multi_fields_.size() - 1];
  json_value[FIELD_FIELD_NAME] = multi_fields_names;
}

RC IndexMeta::from_json(const TableMeta &table, const Json::Value &json_value, IndexMeta &index)
{
  const Json::Value &unique_value = json_value[UNIQUE_FLAG];
  const Json::Value &name_value = json_value[FIELD_NAME];
  const Json::Value &field_amount = json_value[FIELD_AMOUNT];
  const Json::Value &field_values = json_value[FIELD_FIELD_NAME];
  if (!name_value.isString()) {
    LOG_ERROR("Index name is not a string. json value=%s", name_value.toStyledString().c_str());
    return RC::INTERNAL;
  }
  if (!field_values.isString()) {
    LOG_ERROR("Field names of index [%s] is not a string. json value=%s",
        name_value.asCString(),
        field_values.toStyledString().c_str());
    return RC::INTERNAL;
  }

  std::vector<const FieldMeta *> multi_fields;

  std::istringstream iss(field_values.asCString());
  std::vector<std::string> tokens;
  std::string token;
  while (iss >> token) {
    tokens.push_back(token);
  }

  if (std::stoi(field_amount.asString()) != tokens.size()) {
    LOG_ERROR("Field Amount of Index %s is wrong.", name_value.asCString());
    return RC::INTERNAL;
  }
  for (auto e : tokens) {
    const FieldMeta *field = table.field(e.c_str());
    if (nullptr == field) {
      LOG_ERROR("Deserialize index [%s]: no such field: %s", name_value.asCString(), e.c_str());
      return RC::SCHEMA_FIELD_MISSING;
    }
    multi_fields.emplace_back(field);
  }
  return index.init(unique_value.asBool(), name_value.asCString(), multi_fields);
}

const char *IndexMeta::name() const
{
  return name_.c_str();
}
const char *IndexMeta::field(int i) const
{
  return multi_fields_[i].c_str();
}
const char *IndexMeta::multi_fields() const
{
  static std::string multi_field_str;
  multi_field_str.clear();
  for (auto str: multi_fields_) {
    multi_field_str += str;
  }
  return multi_field_str.c_str();
}
const size_t IndexMeta::field_amount() const
{
  return multi_fields_.size();
}

void IndexMeta::desc(std::ostream &os) const
{
  os << "index name=" << name_ << ", field amount=" << multi_fields_.size();
  for (int i = 0; i < multi_fields_.size(); i++) {
    os << ", field no."<< i <<"=" << multi_fields_[i];
  }
}

const bool IndexMeta::is_unique() const
{
  return is_unique_;
}