#ifndef MINIDB_CREATE_VIEW_STMT_H
#define MINIDB_CREATE_VIEW_STMT_H

#include <string>

#include "stmt.h"

class Db;
class SelectStmt;

class CreateViewStmt : public Stmt {

public:
  CreateViewStmt(const std::string &view_name, SelectStmt *select_stmt)
      : view_name_(view_name), select_stmt_(select_stmt) {}
  virtual ~CreateViewStmt() = default;

  StmtType type() const override { return StmtType::CREATE_VIEW; }

  const std::string &view_name() const { return view_name_; }
  SelectStmt *select_stmt() { return select_stmt_; }

  static RC create(Db *db, const CreateViewSqlNode &create_view, Stmt *&stmt);

private:
  std::string view_name_;
  SelectStmt *select_stmt_;

};

#endif  // MINIDB_CREATE_VIEW_STMT_H
