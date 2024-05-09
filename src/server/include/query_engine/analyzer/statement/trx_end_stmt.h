#pragma once

#include <string>
#include <vector>

#include "stmt.h"

/**
 * @brief 事务的 Commit/Rollback 语句
 * @ingroup Statement
 */
class TrxEndStmt : public Stmt
{
 public:
  TrxEndStmt(StmtType type)
      : type_(type)
  {}
  virtual ~TrxEndStmt() = default;

  StmtType type() const override { return type_; }

  static RC create(SqlCommandFlag flag, Stmt *&stmt)
  {
    StmtType type = flag == SqlCommandFlag::SCF_COMMIT ? StmtType::COMMIT : StmtType::ROLLBACK;
    stmt = new TrxEndStmt(type);
    return RC::SUCCESS;
  }

 private:
  StmtType type_;
};