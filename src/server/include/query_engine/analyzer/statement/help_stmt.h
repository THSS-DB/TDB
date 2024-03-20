#pragma once

#include <string>
#include <vector>

#include "stmt.h"

/**
 * @brief Help 语句，现在什么成员都没有
 * @ingroup Statement
 */
class HelpStmt : public Stmt {
 public:
  HelpStmt() {}
  virtual ~HelpStmt() = default;

  StmtType type() const override { return StmtType::HELP; }

  static RC create(Stmt *&stmt) {
    stmt = new HelpStmt();
    return RC::SUCCESS;
  }
};