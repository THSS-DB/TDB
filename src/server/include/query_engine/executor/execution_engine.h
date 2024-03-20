#pragma once

#include <cstddef>
#include <memory>

#include "include/common/rc.h"
#include "include/query_engine/analyzer/statement/stmt.h"
#include "include/query_engine/planner/node/logical_node.h"
#include "include/query_engine/planner/operator/physical_operator.h"
#include "include/query_engine/planner/operator/physical_operator_generator.h"
#include "include/query_engine/structor/query_info.h"

class Executor {
 public:
  RC execute(SessionRequest *request, QueryInfo *queryInfo,
             bool &need_disconnect);

 private:
  size_t min_width = 0;
};