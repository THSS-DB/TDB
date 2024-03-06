#pragma once

#include <string>

#include "stmt.h"

struct CreateIndexSqlNode;
class Table;
class FieldMeta;

/**
 * @brief 创建索引的语句
 * @ingroup Statement
 */
class CreateIndexStmt : public Stmt
{
public:
  CreateIndexStmt(Table *table, std::vector<const FieldMeta*> &multi_field_metas, const std::string &index_name, bool is_unique)
        : table_(table),
          multi_field_metas_(multi_field_metas),
          index_name_(index_name),
          is_unique_(is_unique)
  {}

  virtual ~CreateIndexStmt() = default;

  StmtType type() const override { return StmtType::CREATE_INDEX; }

  Table *table() const { return table_; }
  const FieldMeta *field_meta(int idx) { return multi_field_metas_[idx]; }
  std::vector<const FieldMeta*> &multi_field_metas()  { return multi_field_metas_; }
  const std::string &index_name() const { return index_name_; }
  const bool is_unique() const { return is_unique_; }

public:
  static RC create(Db *db, const CreateIndexSqlNode &create_index, Stmt *&stmt);

private:
  Table *table_ = nullptr;
  std::vector<const FieldMeta*> multi_field_metas_;
  std::string index_name_;
  bool is_unique_;
};
