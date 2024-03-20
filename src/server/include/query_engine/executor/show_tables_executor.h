#pragma once

#include "include/common/rc.h"
#include "include/query_engine/planner/operator/string_list_physical_operator.h"
#include "include/query_engine/structor/query_info.h"
#include "include/session/session.h"
#include "include/session/session_request.h"
#include "include/storage_engine/schema/database.h"
#include "sql_result.h"

/**
 * @brief 显示所有表的执行器
 * @ingroup Executor
 * @note 与CreateIndex类似，不处理并发
 */
class ShowTablesExecutor {
 public:
  ShowTablesExecutor() = default;
  virtual ~ShowTablesExecutor() = default;

  RC execute(QueryInfo *query_info) {
    SqlResult *sql_result = query_info->session_event()->sql_result();
    SessionRequest *session_event = query_info->session_event();

    Db *db = session_event->session()->get_current_db();

    std::vector<std::string> all_tables;
    db->all_tables(all_tables);

    TupleSchema tuple_schema;
    tuple_schema.append_cell(
        TupleCellSpec("", "Tables_in_SYS", "Tables_in_SYS"));
    sql_result->set_tuple_schema(tuple_schema);

    auto oper = new StringListPhysicalOperator;
    for (const std::string &s : all_tables) {
      oper->append(s);
    }

    sql_result->set_operator(std::unique_ptr<PhysicalOperator>(oper));
    return RC::SUCCESS;
  }
};