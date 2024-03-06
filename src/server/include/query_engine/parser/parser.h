#pragma once

#include "include/query_engine/parser/parse_defs.h"
#include "include/query_engine/structor/query_info.h"
#include "include/query_engine/structor/expression/value_expression.h"
#include "include/session/session_request.h"

namespace Parser {
    RC parse(QueryInfo *query_info);
}