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
class DescTableStmt : public Stmt
{
public:
  DescTableStmt(const std::string &table_name)
        : table_name_(table_name)
  {}
  virtual ~DescTableStmt() = default;

  StmtType type() const override { return StmtType::DESC_TABLE; }

  const std::string &table_name() const { return table_name_; }

  static RC create(Db *db, const DescTableSqlNode &desc_table, Stmt *&stmt);

private:
  std::string table_name_;
};