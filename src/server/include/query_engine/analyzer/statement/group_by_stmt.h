#pragma once

#include "src/server/include/common/rc.h"
#include "stmt.h"
#include "src/server/include/query_engine/structor/expression/expression.h"

class GroupByStmt : public Stmt {
public:

  GroupByStmt() = default;
  ~GroupByStmt() override;

  StmtType type() const override {
    return StmtType::GROUP_BY;
  }

  static RC create(
      Db *db,
      Table *default_table,
      std::unordered_map<std::string, Table *> *tables,
      const std::vector<RelAttrSqlNode>,
      GroupByStmt *&stmt);

  static RC create_group_by_unit(
      Db *db,
      Table *default_table,
      std::unordered_map<std::string, Table *> *tables,
      const RelAttrSqlNode &unit,
      Expression *&res_expr);

  const std::vector<Expression *> &group_by_exprs() const {
    return exprs_;
  }

private:
  std::vector<Expression *> exprs_;
};

