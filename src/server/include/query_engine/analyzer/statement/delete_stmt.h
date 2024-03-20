#pragma once

#include "stmt.h"
#include "include/query_engine/parser/parse_defs.h"

class Table;
class FilterStmt;

/**
 * @brief Delete 语句
 * @ingroup Statement
 */
class DeleteStmt : public Stmt 
{
public:
  DeleteStmt(Table *table, FilterStmt *filter_stmt);
  ~DeleteStmt() override;

  Table *table() const
  {
    return table_;
  }
  FilterStmt *filter_stmt() const
  {
    return filter_stmt_;
  }

  StmtType type() const override
  {
    return StmtType::DELETE;
  }

public:
  static RC create(Db *db, const DeleteSqlNode &delete_sql, Stmt *&stmt);

private:
  Table *table_ = nullptr;
  FilterStmt *filter_stmt_ = nullptr;
};
