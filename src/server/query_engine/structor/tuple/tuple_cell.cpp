#include <sstream>
#include "include/query_engine/structor/tuple/tuple_cell.h"

TupleCellSpec::TupleCellSpec(const char *table_name, const char *field_name, const char *alias)
{
  if (table_name) {
    table_name_ = table_name;
  }
  if (field_name) {
    field_name_ = field_name;
  }
  if (alias) {
    alias_ = alias;
  } else {
    if (table_name_.empty()) {
      alias_ = field_name_;
    } else {
      alias_ = table_name_ + "." + field_name_;
    }
  }
}

TupleCellSpec::TupleCellSpec(const char *alias)
{
  if (alias) {
    alias_ = alias;
  }
}

TupleCellSpec::~TupleCellSpec()
{
  // 构造时 copy 了一份 expression，这里需要释放
  delete expression_;
}
