#pragma once

#include "include/query_engine/structor/expression/expression.h"
#include "storage/db/db.h"
#include "common/lang/string.h"

RC analyze_expression(
    const Expression* expr,
    Db *db,
    const std::unordered_map<std::string, Table *> &table_map,
    const std::vector<Table *> &tables,
    Expression *&res_expr,
    bool is_create_table_select = false);
