#include "include/session/session.h"
#include "include/storage_engine/transaction/trx.h"
#include "include/storage_engine/schema/database.h"
#include "include/storage_engine/schema/default_handler.h"
#include "include/common/global_context.h"

Session &Session::default_session()
{
  static Session session;
  return session;
}

Session::Session(const Session &other) : db_(other.db_)
{}

Session::~Session()
{
  if (nullptr != trx_) {
    GCTX.trx_manager_->destroy_trx(trx_);
    trx_ = nullptr;
  }
}

const char *Session::get_current_db_name() const
{
  if (db_ != nullptr)
    return db_->name();
  else
    return "";
}

Db *Session::get_current_db() const
{
  return db_;
}

void Session::set_current_db(const std::string &dbname)
{
  DefaultHandler &handler = DefaultHandler::get_default();
  Db *db = handler.find_db(dbname.c_str());
  if (db == nullptr) {
    LOG_WARN("no such database: %s", dbname.c_str());
    return;
  }

  LOG_TRACE("change db to %s", dbname.c_str());
  db_ = db;
}

void Session::set_trx_multi_operation_mode(bool multi_operation_mode)
{
  trx_multi_operation_mode_ = multi_operation_mode;
}

bool Session::is_trx_multi_operation_mode() const
{
  return trx_multi_operation_mode_;
}

Trx *Session::current_trx()
{
  if (trx_ == nullptr) {
    trx_ = GCTX.trx_manager_->create_trx(db_->log_manager());
  }
  return trx_;
}

thread_local Session *thread_session = nullptr;

void Session::set_current_session(Session *session)
{
  thread_session = session;
}

Session *Session::current_session()
{
  return thread_session;
}

void Session::set_current_request(SessionRequest *request)
{
  current_request_ = request;
}

SessionRequest *Session::current_request() const
{
  return current_request_;
}
