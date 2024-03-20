#pragma once

#include <string>

#include "common/lang/string.h"
#include "common/log/log.h"
#include "include/common/rc.h"
#include "include/query_engine/parser/value.h"
#include "json/json.h"

namespace Json {
class Value;
}  // namespace Json

/**
 * @brief 字段元数据
 */
class FieldMeta {
 public:
  FieldMeta();
  FieldMeta(const char *name, AttrType attr_type, int attr_offset, int attr_len,
            bool visible);
  ~FieldMeta() = default;

  RC init(const char *name, AttrType attr_type, int attr_offset, int attr_len,
          bool nullable, bool visible);

 public:
  const char *name() const;
  AttrType type() const;
  int offset() const;
  int len() const;
  bool nullable() const;
  bool visible() const;

 public:
  void desc(std::ostream &os) const;

 public:
  void to_json(Json::Value &json_value) const;
  static RC from_json(const Json::Value &json_value, FieldMeta &field);

 protected:
  std::string name_;
  AttrType attr_type_;
  int attr_offset_;
  int attr_len_;
  bool nullable_;
  bool visible_;
};
