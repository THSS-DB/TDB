#include "include/query_engine/executor/desc_table_executor.h"

#include "include/query_engine/analyzer/statement/desc_table_stmt.h"
#include "include/query_engine/planner/operator/string_list_physical_operator.h"
#include "include/query_engine/structor/query_info.h"
#include "include/session/session.h"
#include "include/storage_engine/schema/database.h"

using namespace std;

RC DescTableExecutor::execute(QueryInfo *query_info) {
  RC rc = RC::SUCCESS;
  Stmt *stmt = query_info->stmt();
  SessionRequest *session_event = query_info->session_event();
  Session *session = session_event->session();
  ASSERT(stmt->type() == StmtType::DESC_TABLE,
         "desc table executor can not run this command: %d",
         static_cast<int>(stmt->type()));

  DescTableStmt *desc_table_stmt = static_cast<DescTableStmt *>(stmt);

  SqlResult *sql_result = session_event->sql_result();

  const char *table_name = desc_table_stmt->table_name().c_str();

  Db *db = session->get_current_db();
  Table *table = db->find_table(table_name);
  if (table != nullptr) {
    TupleSchema tuple_schema;
    tuple_schema.append_cell(TupleCellSpec("", "Field", "Field"));
    tuple_schema.append_cell(TupleCellSpec("", "Type", "Type"));
    tuple_schema.append_cell(TupleCellSpec("", "Length", "Length"));

    sql_result->set_tuple_schema(tuple_schema);

    auto oper = new StringListPhysicalOperator;
    const TableMeta &table_meta = table->table_meta();
    for (int i = table_meta.sys_field_num(); i < table_meta.field_num(); i++) {
      const FieldMeta *field_meta = table_meta.field(i);
      oper->append({field_meta->name(), attr_type_to_string(field_meta->type()),
                    std::to_string(field_meta->len())});
    }

    sql_result->set_operator(unique_ptr<PhysicalOperator>(oper));
  } else {
    sql_result->set_return_code(RC::SCHEMA_TABLE_NOT_EXIST);
    sql_result->set_state_string("Table not exists");
  }
  return rc;
}