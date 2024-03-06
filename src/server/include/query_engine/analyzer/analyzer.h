#pragma once

#include "include/query_engine/structor/query_info.h"
#include "include/session/session.h"
#include "include/query_engine/analyzer/statement/stmt.h"
#include "include/common/rc.h"

namespace Analyzer {
    RC analyze(QueryInfo *query_info);
}