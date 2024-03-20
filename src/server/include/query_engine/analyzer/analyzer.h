#pragma once

#include "include/query_engine/structor/query_info.h"
#include "include/common/rc.h"

namespace Analyzer {
    RC analyze(QueryInfo *query_info);
}