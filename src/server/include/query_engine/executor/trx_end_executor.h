#pragma once

#include "include/common/rc.h"
#include "include/storage_engine/transaction/trx.h"

/**
 * @brief 事务结束的执行器，可以是提交或回滚
 * @ingroup Executor
 */
class TrxEndExecutor
{
 public:
  TrxEndExecutor() = default;
  virtual ~TrxEndExecutor() = default;

  RC execute(QueryInfo *query_info)
  {
    Stmt *stmt = query_info->stmt();
    Session *session = query_info->session_event()->session();
    session->set_trx_multi_operation_mode(false);
    Trx *trx = session->current_trx();

    if (stmt->type() == StmtType::COMMIT) {
      return trx->commit();
    }
    else {
      return trx->rollback();
    }
  }
};