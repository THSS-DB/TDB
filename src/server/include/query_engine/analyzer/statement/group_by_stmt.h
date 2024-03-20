#pragma once

#include "include/common/rc.h"
#include "include/query_engine/structor/expression/expression.h"
#include "stmt.h"

class GroupByStmt : public Stmt {
 public:
  GroupByStmt() = default;
  ~GroupByStmt() override;

  StmtType type() const override { return StmtType::GROUP_BY; }

  static RC create(Db *db, Table *default_table,
                   std::unordered_map<std::string, Table *> *tables,
                   const std::vector<RelAttrSqlNode>, GroupByStmt *&stmt);

  static RC create_group_by_unit(
      Db *db, Table *default_table,
      std::unordered_map<std::string, Table *> *tables,
      const RelAttrSqlNode &unit, Expression *&res_expr);

  const std::vector<Expression *> &group_by_exprs() const { return exprs_; }

 private:
  std::vector<Expression *> exprs_;
};
