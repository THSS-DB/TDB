#include "include/query_engine/executor/command_executor.h"
#include "include/query_engine/structor/query_info.h"
#include "include/query_engine/analyzer/statement/stmt.h"
#include "common/log/log.h"
#include "include/query_engine/executor/create_index_executor.h"
#include "include/query_engine/executor/create_table_executor.h"
#include "include/query_engine/executor/desc_table_executor.h"
#include "include/query_engine/executor/drop_table_executor.h"
#include "include/query_engine/executor/help_executor.h"
#include "include/query_engine/executor/show_tables_executor.h"
#include "include/query_engine/executor/load_data_executor.h"

RC CommandExecutor::execute(QueryInfo *query_info)
{
  Stmt *stmt = query_info->stmt();

  switch (stmt->type()) {
    case StmtType::CREATE_INDEX: {
      CreateIndexExecutor executor;
      return executor.execute(query_info);
    }

    case StmtType::CREATE_TABLE: {
      CreateTableExecutor executor;
      return executor.execute(query_info);
    }

    case StmtType::DROP_TABLE: {
      DropTableExecutor executor;
      return executor.execute(query_info);
    }

    case StmtType::DESC_TABLE: {
      DescTableExecutor executor;
      return executor.execute(query_info);
    }

    case StmtType::HELP: {
      HelpExecutor executor;
      return executor.execute(query_info);
    }

    case StmtType::SHOW_TABLES: {
      ShowTablesExecutor executor;
      return executor.execute(query_info);
    }

    case StmtType::LOAD_DATA: {
      LoadDataExecutor executor;
      return executor.execute(query_info);
    }

    case StmtType::EXIT: {
      return RC::SUCCESS;
    }

    default: {
      LOG_ERROR("unknown command: %d", static_cast<int>(stmt->type()));
      return RC::UNIMPLENMENT;
    }
  }
}
