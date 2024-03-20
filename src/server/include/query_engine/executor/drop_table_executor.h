#pragma once

#include "include/common/rc.h"

class QueryInfo;

class DropTableExecutor {
 public:
  DropTableExecutor() = default;
  virtual ~DropTableExecutor() = default;

  RC execute(QueryInfo *query_info);
};