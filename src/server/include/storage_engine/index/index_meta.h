#pragma once

#include <string>
#include <vector>

#include "include/common/rc.h"
#include "include/storage_engine/recorder/field_meta.h"
#include "include/storage_engine/recorder/table_meta.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "json/json.h"

class TableMeta;
class FieldMeta;

namespace Json {
class Value;
}  // namespace Json

/**
 * @brief 描述一个索引
 * @ingroup Index
 * @details 一个索引包含了表的哪些字段，索引的名称等。
 */
class IndexMeta 
{
public:
  IndexMeta() = default;

  RC init(bool unique, const char *name, std::vector<const FieldMeta *> &multi_fields);

public:
  const char *name() const;
  const char *field(int i) const;
  const char *multi_fields() const;
  const size_t field_amount() const;
  const bool is_unique() const;

  void desc(std::ostream &os) const;

public:
  void to_json(Json::Value &json_value) const;
  static RC from_json(const TableMeta &table, const Json::Value &json_value, IndexMeta &index);

protected:
  bool is_unique_;  // 是否是唯一索引
  std::string name_;  // index's name
  std::vector<std::string> multi_fields_;
};
