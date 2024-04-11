#include "include/query_engine/executor/execution_engine.h"
#include "include/query_engine/executor/command_executor.h"
#include "include/query_engine/planner/node/logical_node.h"
#include "include/query_engine/planner/operator/physical_operator.h"
#include "include/query_engine/analyzer/statement/select_stmt.h"
#include "include/session/communicator.h"
#include "include/query_engine/structor/query_info.h"
#include "include/storage_engine/transaction/trx.h"
#include "include/session/session.h"
#include <algorithm>
#include <cstddef>
#include <cstring>

const char *DELIM = " | ";
const char END_DELIM = '\0';
const char NEW_LINE = '\n';

RC write_to_communicator(const char* data, int32_t size, Communicator* communicator, const size_t &min_width){
  if(size < min_width){
    int diff = min_width - size;
    char* padding = new char[diff];
    memset(padding, ' ', diff);
    RC rc = communicator->write_result(padding, diff);
    if(RC_FAIL(rc)){
      LOG_WARN("failed to send data to client. err=%s", strerror(errno));
      return rc;
    }
  }
  RC rc = communicator->write_result(data, size);
  if(RC_FAIL(rc)){
    LOG_WARN("failed to send data to client. err=%s", strerror(errno));
    return rc;
  }
  return RC::SUCCESS;
}

void set_operator_schema(QueryInfo *query_info, size_t& min_width)
{
  std::unique_ptr<PhysicalOperator> &physical_operator = query_info->physical_operator();
  Stmt *stmt = query_info->stmt();
  ASSERT(stmt != nullptr, "SQL Statement shouldn't be empty!");
  ASSERT(physical_operator != nullptr, "physical operator should not be null");

  TupleSchema schema;
  switch (stmt->type()) {
    case StmtType::SELECT: {
      auto *select_stmt = dynamic_cast<SelectStmt *>(stmt);
      if (schema.cell_num() != 0) {
        break;
      }
      for (const auto *expr : select_stmt->projects()) {
        std::string alias_str = expr->alias().empty() ? expr->name() : expr->alias();
        const char* alias = alias_str.c_str();
        schema.append_cell(alias);
        min_width = min_width < strlen(alias) ? strlen(alias) : min_width;
      }
    } break;

    case StmtType::EXPLAIN: {
      schema.append_cell("Query Plan");
    } break;
    default: {
    } break;
  }

  SqlResult *sql_result = query_info->session_event()->sql_result();
  sql_result->set_tuple_schema(schema);
  sql_result->set_operator(std::move(physical_operator));
}

RC send_schema(TupleSchema schema, int cell_num, Communicator* communicator, const size_t &min_width){
  RC rc = RC::SUCCESS;
  for (int i = 0; i < cell_num; i++) {
    const TupleCellSpec &spec = schema.cell_at(i);
    const char *alias = spec.alias();
    if (nullptr != alias || alias[0] != 0) {
        if (0 != i) {
          rc = communicator->write_result(DELIM, strlen(DELIM));
          if (RC_FAIL(rc)) {
            return rc;
          }
        }

        int len = strlen(alias);
        rc = write_to_communicator(alias, len, communicator, min_width);
        if (RC_FAIL(rc)) {
          return rc;
        }
    }
  }

  if (cell_num > 0) {
    char newline = '\n';
    rc = communicator->write_result(&newline, 1);
    if (RC_FAIL(rc)) {
        return rc;
    }
  }

  return rc;
}

RC value_to_string(Value &value, std::string &cell_str){
  if (value.attr_type() == TEXTS) {
    std::string text_file_name = value.to_string();
    std::ifstream inputFile(text_file_name);
    if (!inputFile.is_open()) {
        LOG_WARN("failed to init file named %s", text_file_name.c_str());
        return RC::RECORD_NOT_EXIST;
    }
    // 获取文件大小
    inputFile.seekg(0, std::ios::end);
    std::streampos fileSize = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);
    // 分配足够的内存来存储文件数据
    std::vector<char> fileData(fileSize);
    // 一次性读取整个文件数据
    inputFile.read(fileData.data(), fileSize);
    // 关闭文件
    inputFile.close();
    fileData.push_back('\0');
    cell_str = std::string(fileData.data());
  } else {
    cell_str = value.to_string();
  }
  return RC::SUCCESS;
}

RC send_result(SessionRequest *request, bool &need_disconnect, const size_t &min_width){
  RC rc;
  need_disconnect = true;

  SqlResult *sql_result = request->sql_result();
  Communicator* communicator = request->get_communicator();

  if (RC::SUCCESS != sql_result->return_code() || !sql_result->has_operator()) {
    return communicator->write_state(sql_result, need_disconnect);
  }

  rc = sql_result->init();
  if(RC_FAIL(rc)){
    sql_result->close();
    sql_result->set_return_code(rc);
    return communicator->write_state(sql_result, need_disconnect);
  }

  const TupleSchema &schema = sql_result->tuple_schema();
  const int cell_num = schema.cell_num();

  rc = send_schema(schema, cell_num, communicator, min_width);
  if(RC_FAIL(rc)){
    LOG_WARN("failed to send data to client. err=%s", strerror(errno));
    sql_result->close();
    return rc;
  }

  Tuple *tuple = nullptr;
  while(RC::SUCCESS == (rc = sql_result->next_tuple(tuple))){
    int column_num;
    column_num = tuple == nullptr?0:tuple->cell_num();
    for(int i = 0; i < column_num; i++){
        if(i != 0){
          rc = communicator->write_result(DELIM, strlen(DELIM));
          if(RC_FAIL(rc)){
            LOG_WARN("failed to send data to client. err=%s", strerror(errno));
            sql_result->close();
            return rc;
          }
        }

        Value value;
        rc = tuple->cell_at(i, value);
        if(rc != RC::SUCCESS){
          sql_result->close();
          return rc;
        }

        std::string value_str;
        rc = value_to_string(value, value_str);
        if(rc != RC::SUCCESS){
          sql_result->close();
          return rc;
        }
        rc = write_to_communicator(value_str.data(), value_str.size(), communicator, min_width);
        if(RC_FAIL(rc)){
          LOG_WARN("failed to send data to client. err=%s", strerror(errno));
          sql_result->close();
          return rc;
        }
    }

    rc = communicator->write_result(&NEW_LINE, 1);
    if (RC_FAIL(rc)) {
        LOG_WARN("failed to send data to client. err=%s", strerror(errno));
        sql_result->close();
        return rc;
    }
  }

  if (rc == RC::RECORD_EOF) {
    rc = RC::SUCCESS;
  } else if (RC_FAIL(rc)) {
    LOG_TRACE("failed to get next tuple. rc=%s", strrc(rc));
    sql_result->set_return_code(rc);
    return communicator->write_state(sql_result, need_disconnect);
  }

  if (cell_num == 0) {
    // 除了select之外，其它的消息通常不会通过operator来返回结果，表头和行数据都是空的
    // 这里针对这种情况做特殊处理，当表头和行数据都是空的时候，就返回处理的结果
    // 可能是insert/delete等操作，不直接返回给客户端数据，这里把处理结果返回给客户端
    RC rc_close = sql_result->close();
    if (rc == RC::SUCCESS) {
        rc = rc_close;
    }
    sql_result->set_return_code(rc);
    return communicator->write_state(sql_result, need_disconnect);
  } else {
    need_disconnect = false;
  }

  RC rc_close = sql_result->close();
  if (RC_SUCC(rc)) {
    rc = rc_close;
  }

  return rc;
}

RC Executor::execute(SessionRequest *request, QueryInfo *query_info, bool &need_disconnect)
{
  RC rc;
  min_width = 0;
  if(query_info->physical_operator() != nullptr){
    set_operator_schema(query_info, min_width);
  }else{
    // Query doesn't have physical operator, such as: insert, update
    Stmt *stmt = query_info->stmt();
    if (stmt != nullptr) {
        CommandExecutor command_executor;
        rc = command_executor.execute(query_info);
        query_info->session_event()->sql_result()->set_return_code(rc);
    } else {
        return RC::INTERNAL;
    }
  }

  rc = send_result(request, need_disconnect, min_width);

  request->get_communicator()->flush();
  return rc;
}