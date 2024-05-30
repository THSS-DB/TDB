#pragma once

#include "include/common/rc.h"
#include "include/storage_engine/transaction/trx.h"

/**
 * @brief 事务开始语句的执行器
 * @ingroup Executor
 */
class TrxBeginExecutor
{
 public:
  TrxBeginExecutor() = default;
  virtual ~TrxBeginExecutor() = default;

  RC execute(QueryInfo *query_info)
  {
    Session *session = query_info->session_event()->session();
    Trx *trx = session->current_trx();
    session->set_trx_multi_operation_mode(true);
    return trx->start_if_need();
  }
};