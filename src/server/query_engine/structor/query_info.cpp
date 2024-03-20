#include "include/query_engine/structor/query_info.h"

#include "include/session/session_request.h"
#include "include/query_engine/parser/parse_defs.h"
#include "include/query_engine/analyzer/statement/stmt.h"

QueryInfo::QueryInfo(SessionRequest *event, const std::string &sql)
    : session_event_(event), sql_(sql)
{}

QueryInfo::~QueryInfo() noexcept
{
  if (session_event_ != nullptr) {
    session_event_ = nullptr;
  }

  if (stmt_ != nullptr) {
    delete stmt_;
    stmt_ = nullptr;
  }
}

