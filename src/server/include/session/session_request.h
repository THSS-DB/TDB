#pragma once

#include <string.h>

#include <string>

#include "include/query_engine/executor/sql_result.h"

class Session;
class Communicator;

/**
 * @brief 表示一个SQL请求
 */
class SessionRequest {
 public:
  SessionRequest(Communicator *client);
  virtual ~SessionRequest();

  Communicator *get_communicator() const;
  Session *session() const;

  void set_query(const std::string &query) { query_ = query; }

  const std::string &query() const { return query_; }

  SqlResult *sql_result() { return &sql_result_; }

 private:
  Communicator *communicator_ = nullptr;  ///< 与客户端通讯的对象
  SqlResult sql_result_;                  ///< SQL执行结果
  std::string query_;                     ///< SQL语句
};
