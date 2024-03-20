#pragma once

#include <string>
#include <vector>

#include "stmt.h"

class Table;

class LoadDataStmt : public Stmt
{
public:
  LoadDataStmt(Table *table, const char *filename) : table_(table), filename_(filename)
  {}
  virtual ~LoadDataStmt() = default;

  StmtType type() const override { return StmtType::LOAD_DATA; }

  Table *table() const { return table_; }
  const char *filename() const { return filename_.c_str(); }

  static RC create(Db *db, const LoadDataSqlNode &load_data, Stmt *&stmt);

private:
  Table *table_ = nullptr;
  std::string filename_;
};
