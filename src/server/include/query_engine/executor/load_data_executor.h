#pragma once

#include "include/common/rc.h"
#include "include/query_engine/structor/query_info.h"

class Table;
class SqlResult;

/**
 * @brief 导入数据的执行器
 * @ingroup Executor
 */
class LoadDataExecutor
{
public:
  LoadDataExecutor() = default;
  virtual ~LoadDataExecutor() = default;

  RC execute(QueryInfo *query_info);
  
private:
  void load_data(Table *table, const char *file_name, SqlResult *sql_result);
};
