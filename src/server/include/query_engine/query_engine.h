#pragma once

#include "include/common/rc.h"
#include "include/query_engine/planner/planner.h"
#include "include/query_engine/optimizer/optimizer.h"
#include "include/query_engine/executor/execution_engine.h"

class SessionRequest;
class QueryInfo;

class QueryEngine
{
public:
  QueryEngine() = default;
  ~QueryEngine() = default;

  bool process_session_request(SessionRequest *request);
  RC planQuery(QueryInfo *query_info);
private:
  Planner planner_;
  Optimizer optimizer_;
  Executor executor_;
};