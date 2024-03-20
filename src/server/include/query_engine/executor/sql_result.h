#pragma once

#include <memory>
#include <string>

#include "include/query_engine/planner/operator/physical_operator.h"
#include "include/query_engine/structor/tuple/tuple.h"

class Session;

/**
 * @brief SQL执行结果
 * @details
 * 如果当前SQL生成了执行计划，那么在返回客户端时，调用执行计划返回结果。
 * 否则返回的结果就是当前SQL的执行结果，比如DDL语句，通过return_code和state_string来描述。
 * 如果出现了一些错误，也可以通过return_code和state_string来获取信息。
 */
class SqlResult {
 public:
  SqlResult(Session *session);
  ~SqlResult() {}

  void set_tuple_schema(const TupleSchema &schema);
  void set_return_code(RC rc) { return_code_ = rc; }
  void set_state_string(const std::string &state_string) {
    state_string_ = state_string;
  }

  void set_operator(std::unique_ptr<PhysicalOperator> oper);

  bool has_operator() const { return operator_ != nullptr; }
  const TupleSchema &tuple_schema() const { return tuple_schema_; }
  RC return_code() const { return return_code_; }
  const std::string &state_string() const { return state_string_; }

  RC init();
  RC close();
  RC next_tuple(Tuple *&tuple);

 public:
  std::vector<FuncResult> function_results_;

 private:
  Session *session_ = nullptr;                  ///< 当前所属会话
  std::unique_ptr<PhysicalOperator> operator_;  ///< 执行计划
  TupleSchema tuple_schema_;  ///< 返回的表头信息。可能有也可能没有
  RC return_code_ = RC::SUCCESS;
  std::string state_string_;
};
