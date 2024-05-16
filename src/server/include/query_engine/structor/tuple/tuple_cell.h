#pragma once

#include <iostream>

#include "include/query_engine/structor/expression/expression.h"
#include "include/storage_engine/recorder/table.h"
#include "include/storage_engine/recorder/field_meta.h"

class TupleCellSpec
{
public:
  explicit TupleCellSpec(const Expression *expression) : expression_(expression->copy()) {
    alias_ = expression->name();
  }

  TupleCellSpec(const char *table_name, const char *field_name, const char *alias = nullptr);
  explicit TupleCellSpec(const char *alias);

  ~TupleCellSpec();

  const char *table_name() const {
    return table_name_.c_str();
  }

  const char *field_name() const {
    return field_name_.c_str();
  }

  const char *alias() const {
    return alias_.c_str();
  }

  const Expression *expression() const {
      return expression_;
  }

private:
  Expression* expression_ = nullptr;
  std::string alias_;

  std::string table_name_;
  std::string field_name_;
};
