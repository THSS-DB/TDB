#pragma once

#include "include/common/rc.h"
#include "stmt.h"

class Table;
class FieldMeta;
class FilterStmt;

/**
 * @brief 更新语句
 * @ingroup Statement
 */
class UpdateStmt : public Stmt
{
public:
  UpdateStmt(Table *table, std::vector<UpdateUnit> update_units, FilterStmt *filter_stmt);
  ~UpdateStmt();

public:
  static RC create(Db *db, const UpdateSqlNode &update_sql, Stmt *&stmt);

public:
  Table *table() const
  {
    return table_;
  }

  std::vector<UpdateUnit> update_units() {
    return update_units_;
  }

  FilterStmt *filter_stmt() const {
    return filter_stmt_;
  }

  StmtType type() const override
  {
    return StmtType::UPDATE;
  }

private:
  Table *table_ = nullptr;
  std::vector<UpdateUnit> update_units_;
  FilterStmt *filter_stmt_ = nullptr;
};
