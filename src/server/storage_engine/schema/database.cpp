#include "include/storage_engine/schema/database.h"

Db::~Db()
{
  for (auto &iter : opened_tables_) {
    delete iter.second;
  }
  LOG_INFO("Db has been closed: %s", name_.c_str());
}

RC Db::init(const char *name, const char *dbpath)
{
  if (common::is_blank(name)) {
    LOG_ERROR("Failed to init DB, name cannot be empty");
    return RC::INVALID_ARGUMENT;
  }

  if (!common::is_directory(dbpath)) {
    LOG_ERROR("Failed to init DB, path is not a directory: %s", dbpath);
    return RC::INTERNAL;
  }

  log_manager_.reset(new LogManager());
  if (log_manager_ == nullptr) {
    LOG_ERROR("Failed to init LogManager.");
    return RC::NOMEM;
  }

  RC rc = log_manager_->init(dbpath);
  if (RC_FAIL(rc)) {
    LOG_WARN("failed to init redolog manager. dbpath=%s, rc=%s", dbpath, strrc(rc));
    return rc;
  }

  name_ = name;
  path_ = dbpath;

  rc = open_all_tables();
  if (RC_FAIL(rc)) {
    LOG_WARN("failed to open all tables. dbpath=%s, rc=%s", dbpath, strrc(rc));
    return rc;
  }

  rc = recover();
  if (RC_FAIL(rc)) {
    LOG_WARN("failed to recover db. dbpath=%s, rc=%s", dbpath, strrc(rc));
    return rc;
  }
  return rc;
}

RC Db::create_table(const char *table_name, int attribute_count, const AttrInfoSqlNode *attributes)
{
  RC rc = RC::SUCCESS;
  // check table_name
  if (opened_tables_.count(table_name) != 0) {
    LOG_WARN("%s has been opened before.", table_name);
    return RC::SCHEMA_TABLE_EXIST;
  }

  // 文件路径可以移到Table模块
  std::string table_file_path = table_meta_file(path_.c_str(), table_name);
  Table *table = new Table();
  int32_t table_id = next_table_id_++;
  rc = table->create(table_id, table_file_path.c_str(), table_name, path_.c_str(), attribute_count, attributes);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to create table %s.", table_name);
    delete table;
    return rc;
  }

  opened_tables_[table_name] = table;
  LOG_INFO("Create table success. table name=%s, table_id:%d", table_name, table_id);
  return RC::SUCCESS;
}

RC Db::create_view(const char *view_name, const char *origin_table_name, SelectStmt *select_stmt, int attribute_count, const AttrInfoSqlNode *attributes) {
  RC rc = RC::SUCCESS;
  // check view_name
  if (opened_tables_.count(view_name) != 0) {
    LOG_WARN("%s has been opened before.", view_name);
    return RC::SCHEMA_TABLE_EXIST;
  }

  // 文件路径可以移到Table模块
  std::string view_file_path = table_meta_file(path_.c_str(), view_name);
  auto *view = new Table();
  int32_t table_id = next_table_id_++;
  rc = view->create(table_id, view_name, origin_table_name, select_stmt, attribute_count, attributes);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to create view %s.", view_name);
    delete view;
    return rc;
  }

  opened_tables_[view_name] = view;
  LOG_INFO("Create view success. view name=%s, table_id:%d", view_name, table_id);
  return RC::SUCCESS;
}

RC Db::drop_table(const char *table_name)
{
  RC rc = RC::SUCCESS;
  // check table_name
  if(opened_tables_.count(table_name) == 0) {
    LOG_WARN("%s hasn't been opened before.", table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  Table *table = opened_tables_[table_name];
  rc = table->drop(table->table_id(),table_name,path_.c_str());
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to drop table %s.", table_name);
    delete table;
    return rc;
  }

  opened_tables_.erase(table_name);
  delete table;
  LOG_INFO("Drop table success. table name=%s", table_name);
  return RC::SUCCESS;
}

Table *Db::find_table(const char *table_name) const
{
  std::unordered_map<std::string, Table *>::const_iterator iter = opened_tables_.find(table_name);
  if (iter != opened_tables_.end()) {
    return iter->second;
  }
  return nullptr;
}

Table *Db::find_table(int32_t table_id) const
{
  for (auto pair : opened_tables_) {
    if (pair.second->table_id() == table_id) {
      return pair.second;
    }
  }
  return nullptr;
}

RC Db::open_all_tables()
{
  std::vector<std::string> table_meta_files;
  int ret = common::list_file(path_.c_str(), TABLE_META_FILE_PATTERN, table_meta_files);
  if (ret < 0) {
    LOG_ERROR("Failed to list table meta files under %s.", path_.c_str());
    return RC::IOERR_READ;
  }

  RC rc = RC::SUCCESS;
  for (const std::string &filename : table_meta_files) {
    Table *table = new Table();
    rc = table->open(filename.c_str(), path_.c_str());
    if (rc != RC::SUCCESS) {
      delete table;
      LOG_ERROR("Failed to open table. filename=%s", filename.c_str());
      return rc;
    }

    if (opened_tables_.count(table->name()) != 0) {
      delete table;
      LOG_ERROR("Duplicate table with difference file name. table=%s, the other filename=%s",
          table->name(), filename.c_str());
      return RC::INTERNAL;
    }

    if (table->table_id() >= next_table_id_) {
      next_table_id_ = table->table_id() + 1;
    }
    opened_tables_[table->name()] = table;
    LOG_INFO("Open table: %s, file: %s", table->name(), filename.c_str());
  }

  LOG_INFO("All table have been opened. num=%d", opened_tables_.size());
  return rc;
}

const char *Db::name() const
{
  return name_.c_str();
}

void Db::all_tables(std::vector<std::string> &table_names) const
{
  for (const auto &table_item : opened_tables_) {
    table_names.emplace_back(table_item.first);
  }
}

RC Db::sync()
{
  RC rc = RC::SUCCESS;
  for (const auto &table_pair : opened_tables_) {
    Table *table = table_pair.second;
    rc = table->sync();
    if (rc != RC::SUCCESS) {
      LOG_ERROR("Failed to flush table. table=%s.%s, rc=%d:%s", name_.c_str(), table->name(), rc, strrc(rc));
      return rc;
    }
    LOG_INFO("Successfully sync table db:%s, table:%s.", name_.c_str(), table->name());
  }
  LOG_INFO("Successfully sync db. db=%s", name_.c_str());
  return rc;
}

RC Db::recover()
{
  return log_manager_->recover(this);
}

LogManager *Db::log_manager()
{
  return log_manager_.get();
}