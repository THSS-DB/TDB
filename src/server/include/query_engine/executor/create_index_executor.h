#pragma once

#include "include/common/rc.h"

class QueryInfo;

class CreateIndexExecutor
{
public:
  CreateIndexExecutor() = default;
  virtual ~CreateIndexExecutor() = default;

  RC execute(QueryInfo *query_info);
};