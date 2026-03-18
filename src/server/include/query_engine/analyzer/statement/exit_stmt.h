#pragma once

#include <string>
#include <vector>

#include "stmt.h"

/**
 * @brief Exit 语句，表示断开连接，现在什么成员都没有
 * @ingroup Statement
 */
class ExitStmt : public Stmt {
 public:
  ExitStmt() {}
  virtual ~ExitStmt() = default;

  StmtType type() const override { return StmtType::EXIT; }

  static RC create(Stmt *&stmt) {
    stmt = new ExitStmt();
    return RC::SUCCESS;
  }
};