#pragma once

#include "include/common/rc.h"
#include "include/query_engine/planner/operator/string_list_physical_operator.h"
#include "include/query_engine/structor/query_info.h"
#include "include/session/session_request.h"
#include "sql_result.h"
#include "include/session/session.h"

class HelpExecutor
{
public:
  HelpExecutor() = default;
  virtual ~HelpExecutor() = default;

  RC execute(QueryInfo *query_info)
  {
    const char *strings[] = {
        "show tables;",
        "desc `table name`;",
        "create table `table name` (`column name` `column type`, ...);",
        "create index `index name` on `table` (`column`);",
        "insert into `table` values(`value1`,`value2`);",
        "update `table` set column=value [where `column`=`value`];",
        "delete from `table` [where `column`=`value`];",
        "select [ * | `columns` | aggregation function ] from `table` where `condition` order by `column name`;"
      };

    auto oper = new StringListPhysicalOperator();
    for (size_t i = 0; i < sizeof(strings) / sizeof(strings[0]); i++) {
      oper->append(strings[i]);
    }

    SqlResult *sql_result = query_info->session_event()->sql_result();

    TupleSchema schema;
    schema.append_cell("Commands");

    sql_result->set_tuple_schema(schema);
    sql_result->set_operator(std::unique_ptr<PhysicalOperator>(oper));

    return RC::SUCCESS;
  }
};