#pragma once

#include "include/common/rc.h"
#include "include/query_engine/structor/query_info.h"

namespace Analyzer {
RC analyze(QueryInfo *query_info);
}