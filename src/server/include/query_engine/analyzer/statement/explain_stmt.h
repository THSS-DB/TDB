#pragma once

#include <memory>
#include "stmt.h"

/**
 * @brief explain语句
 * @ingroup Statement
 */
class ExplainStmt : public Stmt 
{
public:
  ExplainStmt(std::unique_ptr<Stmt> child_stmt);
  virtual ~ExplainStmt() = default;

  StmtType type() const override
  {
    return StmtType::EXPLAIN;
  }

  Stmt *child() const
  {
    return child_stmt_.get();
  }

  static RC create(Db *db, const ExplainSqlNode &query, Stmt *&stmt);

private:
  std::unique_ptr<Stmt> child_stmt_;
};
