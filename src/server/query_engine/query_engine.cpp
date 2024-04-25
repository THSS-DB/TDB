#include "include/query_engine/query_engine.h"
#include "common/lang/string.h"
#include "include/session/session.h"
#include "include/query_engine/parser/parser.h"
#include "include/query_engine/analyzer/analyzer.h"
#include "include/session/communicator.h"

#include <chrono>
#include <memory>

// 处理从session传来的请求, 包含sql执行与结果写回
bool QueryEngine::process_session_request(SessionRequest *request) {
  RC rc;
  char *time_str = new char[64];
  bool need_disconnect = true;

  std::string sql = request->query();
  if (common::is_blank(sql.c_str())) {
    return true;
  }

  Session::set_current_session(request->session());
  request->session()->set_current_request(request);

  QueryInfo query_info(request, sql);

  auto start_time = std::chrono::high_resolution_clock::now();
  rc = planQuery(&query_info);
  if(RC_FAIL(rc) && rc != RC::UNIMPLENMENT){
    request->get_communicator()->write_state(request->sql_result(), need_disconnect);
    request->get_communicator()->flush();
    return need_disconnect;
  }else{
    //执行引擎入口
    executor_.execute(request, &query_info, need_disconnect);
  }
  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
  snprintf(time_str, 64, "Cost time: %ld ns\n", duration.count());
  
  request->get_communicator()->write_result(time_str, strlen(time_str));
  request->get_communicator()->send_message_delimiter();
  request->get_communicator()->flush();
  request->session()->set_current_request(nullptr);

  Session::set_current_session(nullptr);
  delete[] time_str;
  return need_disconnect;
}

// 查询的前端解析阶段，对输入的sql进行解析，并构建QueryInfo
RC QueryEngine::planQuery(QueryInfo *query_info) {

  // 1. 语法解析：将sql转为语法树
  RC rc = Parser::parse(query_info);
  if (RC_FAIL(rc)) {
    LOG_TRACE("failed to do parse. rc=%s", strrc(rc));
    return rc;
  }

  // 2. 分析预处理：解析抽象语法树并进行预处理，生成statement结构
  rc = Analyzer::analyze(query_info);
  if (RC_FAIL(rc)) {
    LOG_TRACE("failed to do resolve. rc=%s", strrc(rc));
    return rc;
  }

  // 3. 逻辑计划生成：参照statement结构生成逻辑计划树
  std::unique_ptr<LogicalNode> logical_nodes;
  rc = planner_.plan_logical_tree(query_info, logical_nodes);
  if (rc != RC::SUCCESS) {
    LOG_TRACE("failed to create logical nodes. rc=%s", strrc(rc));
    return rc;
  }

  // 4. 查询优化：对逻辑计划树进行优化，生成优化后的逻辑计划树，目前仅基于RBO进行优化
  rc = optimizer_.rewrite(logical_nodes);
  if (rc != RC::UNIMPLENMENT && rc != RC::SUCCESS) {
    LOG_TRACE("failed to do optimize. rc=%s", strrc(rc));
    return rc;
  }

  // 5. 物理计划生成：根据优化后的逻辑计划树生成物理计划树，描述了查询的具体执行逻辑
  rc = planner_.plan_physical_operator(logical_nodes, query_info);
  if(RC_FAIL(rc)) {
    LOG_TRACE("failed to create physical operator. rc=%s", strrc(rc));
    return rc;
  }

  return rc;
}

