#pragma once

#include <string>
#include <vector>

#include "stmt.h"

class Db;

/**
 * @brief 描述表的语句
 * @ingroup Statement
 * @details 虽然解析成了stmt，但是与原始的SQL解析后的数据也差不多
 */
class ShowTablesStmt : public Stmt {
 public:
  ShowTablesStmt() = default;
  virtual ~ShowTablesStmt() = default;

  StmtType type() const override { return StmtType::SHOW_TABLES; }

  static RC create(Db *db, Stmt *&stmt) {
    stmt = new ShowTablesStmt();
    return RC::SUCCESS;
  }
};