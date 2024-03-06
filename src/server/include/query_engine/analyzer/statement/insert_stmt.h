#pragma once

#include "src/server/include/common/rc.h"
#include "stmt.h"

class Table;
class Db;

/**
 * @brief 插入语句
 * @ingroup Statement
 */
class InsertStmt : public Stmt 
{
public:
  InsertStmt() = default;
  InsertStmt(Table *table, const std::vector<std::vector<Value>> &multi_values_, int record_amount_, int value_amount);

  StmtType type() const override
  {
    return StmtType::INSERT;
  }

public:
  static RC create(Db *db, const InsertSqlNode &insert_sql, Stmt *&stmt);

public:
  Table *table() const
  {
    return table_;
  }
  const std::vector<std::vector<Value>> & multi_values()
  {
    return multi_values_;
  }
  const Value *values(int record_idx)
  {
    return multi_values_[record_idx].data();
  }
  int record_amount()
  {
    return record_amount_;
  }
  int value_amount()
  {
    return value_amount_;
  }

private:
  Table *table_ = nullptr;
  const std::vector<std::vector<Value>> multi_values_;
  int record_amount_ = 0;
  int value_amount_ = 0;
};
