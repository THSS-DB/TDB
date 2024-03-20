#include "include/session/thread_data.h"

#include "include/session/session.h"

thread_local ThreadData* ThreadData::thread_data_;

Trx* ThreadData::trx() const {
  return (session_ == nullptr) ? nullptr : session_->current_trx();
}