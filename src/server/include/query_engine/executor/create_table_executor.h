#pragma once

#include "include/common/rc.h"

class QueryInfo;

class CreateTableExecutor {
 public:
  CreateTableExecutor() = default;
  virtual ~CreateTableExecutor() = default;

  RC execute(QueryInfo *query_info);
};