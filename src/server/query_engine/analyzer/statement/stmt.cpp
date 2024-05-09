#include "common/log/log.h"
#include "include/query_engine/analyzer/statement/stmt.h"
#include "include/query_engine/analyzer/statement/insert_stmt.h"
#include "include/query_engine/analyzer/statement/update_stmt.h"
#include "include/query_engine/analyzer/statement/delete_stmt.h"
#include "include/query_engine/analyzer/statement/select_stmt.h"
#include "include/query_engine/analyzer/statement/explain_stmt.h"
#include "include/query_engine/analyzer/statement/create_index_stmt.h"
#include "include/query_engine/analyzer/statement/create_table_stmt.h"
#include "include/query_engine/analyzer/statement/drop_table_stmt.h"
#include "include/query_engine/analyzer/statement/desc_table_stmt.h"
#include "include/query_engine/analyzer/statement/help_stmt.h"
#include "include/query_engine/analyzer/statement/show_tables_stmt.h"
#include "include/query_engine/analyzer/statement/exit_stmt.h"
#include "include/query_engine/analyzer/statement/load_data_stmt.h"
#include "include/query_engine/analyzer/statement/trx_begin_stmt.h"
#include "include/query_engine/analyzer/statement/trx_end_stmt.h"

RC Stmt::create_stmt(Db *db, ParsedSqlNode &sql_node, Stmt *&stmt)
{
  stmt = nullptr;

  switch (sql_node.flag) {
    case SCF_INSERT: {
      return InsertStmt::create(db, sql_node.insertion, stmt);
    }
    case SCF_DELETE: {
      return DeleteStmt::create(db, sql_node.deletion, stmt);
    }
    case SCF_UPDATE: {
      return UpdateStmt::create(db, sql_node.update,stmt);
    }
    case SCF_SELECT: {
      return SelectStmt::create(db, sql_node.selection, stmt);
    }
    case SCF_EXPLAIN: {
      return ExplainStmt::create(db, sql_node.explain, stmt);
    }
    case SCF_CREATE_INDEX: {
      return CreateIndexStmt::create(db, sql_node.create_index, stmt);
    }
    case SCF_CREATE_TABLE: {
      return CreateTableStmt::create(db, sql_node.create_table, stmt);
    }
    case SCF_DROP_TABLE: {
      return DropTableStmt::create(db, sql_node.drop_table, stmt);
    }
    case SCF_DESC_TABLE: {
      return DescTableStmt::create(db, sql_node.desc_table, stmt);
    }
    case SCF_HELP: {
      return HelpStmt::create(stmt);
    }

    case SCF_SHOW_TABLES: {
      return ShowTablesStmt::create(db, stmt);
    }

    case SCF_EXIT: {
      return ExitStmt::create(stmt);
    }

    case SCF_LOAD_DATA: {
      return LoadDataStmt::create(db, sql_node.load_data, stmt);
    }

    case SCF_BEGIN: {
      return TrxBeginStmt::create(stmt);
    }

    case SCF_COMMIT:
    case SCF_ROLLBACK: {
      return TrxEndStmt::create(sql_node.flag, stmt);
    }

    default: {
      LOG_INFO("Command::type %d doesn't need to create statement.", sql_node.flag);
    } break;
  }
  return RC::UNIMPLENMENT;
}
