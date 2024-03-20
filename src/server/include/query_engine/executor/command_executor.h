#pragma once

#include "include/common/rc.h"

class QueryInfo;

class CommandExecutor
{
public:
  CommandExecutor() = default;
  virtual ~CommandExecutor() = default;

  RC execute(QueryInfo *query_info);
};
