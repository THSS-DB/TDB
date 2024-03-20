#pragma once

#include <string>
#include <vector>

#include "stmt.h"

class Db;

/**
 * @brief 表示创建表的语句
 * @ingroup Statement
 * @details 虽然解析成了stmt，但是与原始的SQL解析后的数据也差不多
 */
class CreateTableStmt : public Stmt {
 public:
  CreateTableStmt(const std::string &table_name,
                  const std::vector<AttrInfoSqlNode> &attr_infos)
      : table_name_(table_name), attr_infos_(attr_infos) {}
  virtual ~CreateTableStmt() = default;

  StmtType type() const override { return StmtType::CREATE_TABLE; }

  const std::string &table_name() const { return table_name_; }
  const std::vector<AttrInfoSqlNode> &attr_infos() const { return attr_infos_; }

  static RC create(Db *db, const CreateTableSqlNode &create_table, Stmt *&stmt);

 private:
  std::string table_name_;
  std::vector<AttrInfoSqlNode> attr_infos_;
};