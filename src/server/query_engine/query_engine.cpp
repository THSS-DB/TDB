#include "include/query_engine/query_engine.h"
#include "common/lang/string.h"
#include "include/storage_engine/recorder/record.h"
#include "include/session/session.h"
#include "include/query_engine/parser/parser.h"
#include "include/query_engine/analyzer/analyzer.h"
#include "include/session/communicator.h"

#include <memory>

// 处理从session传来的请求, 包含sql执行与结果写回
bool QueryEngine::process_session_request(SessionRequest *request) {
  RC rc;
  bool need_disconnect = true;

  std::string sql = request->query();
  if (common::is_blank(sql.c_str())) {
    return true;
  }

  Session::set_current_session(request->session());
  request->session()->set_current_request(request);

  QueryInfo query_info(request, sql);
  rc = executeQuery(&query_info);
  if(RC_FAIL(rc) && rc != RC::UNIMPLENMENT){
    request->get_communicator()->write_state(request->sql_result(), need_disconnect);
    request->get_communicator()->flush();
    return need_disconnect;
  }else{
    executor_.execute(request, &query_info, need_disconnect);
  }

  request->session()->set_current_request(nullptr);
  Session::set_current_session(nullptr);

  return need_disconnect;
}

RC QueryEngine::executeQuery(QueryInfo *query_info) {
  RC rc = Parser::parse(query_info);
  if (RC_FAIL(rc)) {
    LOG_TRACE("failed to do parse. rc=%s", strrc(rc));
    return rc;
  }

  rc = Analyzer::analyze(query_info);
  if (RC_FAIL(rc)) {
    LOG_TRACE("failed to do resolve. rc=%s", strrc(rc));
    return rc;
  }

  // Create logical node trees
  std::unique_ptr<LogicalNode> logical_nodes;
  rc = planner_.plan_logical_tree(query_info, logical_nodes);
  if (rc != RC::SUCCESS) {
    LOG_TRACE("failed to create logical nodes. rc=%s", strrc(rc));
    return rc;
  }

  // A RBO(Ruler-based-optimizer) to optimze the logical operator by some rules
  rc = optimizer_.rewrite(logical_nodes);
  if (rc != RC::UNIMPLENMENT && rc != RC::SUCCESS) {
    LOG_TRACE("failed to do optimize. rc=%s", strrc(rc));
    return rc;
  }

  rc = planner_.plan_physical_operator(logical_nodes, query_info);
  if(RC_FAIL(rc)) {
    LOG_TRACE("failed to create physical operator. rc=%s", strrc(rc));
    return rc;
  }

  return rc;
}

