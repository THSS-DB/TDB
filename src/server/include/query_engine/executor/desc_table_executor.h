#pragma once

#include "include/common/rc.h"

class QueryInfo;

class DescTableExecutor
{
public:
  DescTableExecutor() = default;
  virtual ~DescTableExecutor() = default;

  RC execute(QueryInfo *query_info);
};