#pragma once

#include <memory>
#include <string>

#include "include/query_engine/planner/operator/physical_operator.h"
#include "include/session/session_request.h"

class SessionRequest;
class Stmt;
class ParsedSqlNode;

class QueryInfo {
 public:
  QueryInfo(SessionRequest *event, const std::string &sql);
  virtual ~QueryInfo() noexcept;

  void set_session_event(SessionRequest *event) { session_event_ = event; }
  SessionRequest *session_event() const { return session_event_; }

  void set_sql(const std::string &sql) { sql_ = sql; }
  const std::string &sql() const { return sql_; }

  void set_sql_node(std::unique_ptr<ParsedSqlNode> sql_node) {
    sql_node_ = std::move(sql_node);
  }
  const std::unique_ptr<ParsedSqlNode> &sql_node() const { return sql_node_; }

  void set_stmt(Stmt *stmt) { stmt_ = stmt; }
  Stmt *stmt() const { return stmt_; }

  void set_operator(std::unique_ptr<PhysicalOperator> op) {
    operator_ = std::move(op);
  }
  std::unique_ptr<PhysicalOperator> &physical_operator() { return operator_; }

 private:
  std::unique_ptr<ParsedSqlNode> sql_node_;
  Stmt *stmt_ = nullptr;
  std::unique_ptr<PhysicalOperator> operator_;
  SessionRequest *session_event_ = nullptr;
  std::string sql_;
};
