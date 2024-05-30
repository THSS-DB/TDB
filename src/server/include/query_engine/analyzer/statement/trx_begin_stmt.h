#pragma once

#include <string>
#include <vector>

#include "stmt.h"

/**
 * @brief 事务的Begin语句
 * @ingroup Statement
 */
class TrxBeginStmt : public Stmt
{
 public:
  TrxBeginStmt()
  {}
  virtual ~TrxBeginStmt() = default;

  StmtType type() const override { return StmtType::BEGIN; }

  static RC create(Stmt *&stmt)
  {
    stmt = new TrxBeginStmt();
    return RC::SUCCESS;
  }
};