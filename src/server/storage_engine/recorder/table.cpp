#include "include/storage_engine/recorder/table.h"
#include "include/storage_engine/recorder/record_manager.h"
#include "include/storage_engine/schema/schema_util.h"
#include "include/storage_engine/index/bplus_tree_index.h"
#include <random>


Table::~Table()
{
  if (record_handler_ != nullptr) {
    delete record_handler_;
    record_handler_ = nullptr;
  }

  if (data_buffer_pool_ != nullptr) {
    data_buffer_pool_->close_file();
    data_buffer_pool_ = nullptr;
  }

  for (std::vector<Index *>::iterator it = indexes_.begin(); it != indexes_.end(); ++it) {
    Index *index = *it;
    delete index;
  }
  indexes_.clear();

  LOG_INFO("Table has been closed: %s", name());
}

RC Table::create(int32_t table_id, 
    const char *path,
    const char *name,
    const char *base_dir,
    int attribute_count,
    const AttrInfoSqlNode attributes[])
{
  if (table_id < 0) {
    LOG_WARN("invalid table id. table_id=%d, table_name=%s", table_id, name);
    return RC::INVALID_ARGUMENT;
  }
  if (common::is_blank(name)) {
    LOG_WARN("Name cannot be empty");
    return RC::INVALID_ARGUMENT;
  }
  LOG_INFO("Begin to create table %s:%s", base_dir, name);

  if (attribute_count <= 0 || nullptr == attributes) {
    LOG_WARN("Invalid arguments. table_name=%s, attribute_count=%d, attributes=%p", name, attribute_count, attributes);
    return RC::INVALID_ARGUMENT;
  }

  RC rc = RC::SUCCESS;

  // 使用 table_name.table记录一个表的元数据
  // 判断表文件是否已经存在
  int fd = ::open(path, O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC, 0600);
  if (fd < 0) {
    if (EEXIST == errno) {
      LOG_ERROR("Failed to create table file, it has been created. %s, EEXIST, %s", path, strerror(errno));
      return RC::SCHEMA_TABLE_EXIST;
    }
    LOG_ERROR("Create table file failed. filename=%s, errmsg=%d:%s", path, errno, strerror(errno));
    return RC::IOERR_OPEN;
  }

  close(fd);

  // 创建文件
  if ((rc = table_meta_.init(table_id, name, attribute_count, attributes)) != RC::SUCCESS) {
    LOG_ERROR("Failed to init table meta. name:%s, ret:%d", name, rc);
    return rc;  // delete table file
  }

  std::fstream fs;
  fs.open(path, std::ios_base::out | std::ios_base::binary);
  if (!fs.is_open()) {
    LOG_ERROR("Failed to open file for write. file name=%s, errmsg=%s", path, strerror(errno));
    return RC::IOERR_OPEN;
  }

  // 记录元数据到文件中
  table_meta_.serialize(fs);
  fs.close();

  std::string data_file = table_data_file(base_dir, name);
  BufferPoolManager &bpm = BufferPoolManager::instance();
  rc = bpm.create_file(data_file.c_str());
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to create disk buffer pool of data file. file name=%s", data_file.c_str());
    return rc;
  }

  rc = init_record_handler(base_dir);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to create table %s due to init record handler failed.", data_file.c_str());
    // don't need to remove the data_file
    return rc;
  }

  base_dir_ = base_dir;
  LOG_INFO("Successfully create table %s:%s", base_dir, name);
  return rc;
}

RC Table::create(int32_t table_id,
    const char *name,
    const char *origin_table_name,
    SelectStmt *select_stmt,
    int attribute_count,
    const AttrInfoSqlNode attributes[]) {

  if (table_id < 0) {
    LOG_WARN("invalid table id. table_id=%d, view_name=%s", table_id, name);
    return RC::INVALID_ARGUMENT;
  }
  if (common::is_blank(name)) {
    LOG_WARN("Name cannot be empty");
    return RC::INVALID_ARGUMENT;
  }
  LOG_INFO("Begin to create view %s", name);

  RC rc = RC::SUCCESS;

  // 记录 meta
  if ((rc = table_meta_.init(table_id, name, origin_table_name, select_stmt, attribute_count, attributes)) != RC::SUCCESS) {
    LOG_ERROR("Failed to init table meta. name:%s, ret:%d", name, rc);
    return rc;  // delete table file
  }

  LOG_INFO("Successfully create view %s", name);
  return rc;
}

RC Table::drop(int32_t table_id,
    const char *name,
    const char *base_dir) {
  if (common::is_blank(name)) {
    LOG_WARN("Name cannot be empty");
    return RC::INVALID_ARGUMENT;
  }
  LOG_INFO("Begin to drop table %s:%s", base_dir, name);

  RC rc = RC::SUCCESS;

  if((rc = sync()) != RC::SUCCESS) {
    LOG_ERROR("Failed to sync errno=%d", errno);
    return rc;
  }

  std::string meta_file = table_meta_file(base_dir, name);

  if(unlink(meta_file.c_str()) != 0) {
    LOG_ERROR("Failed to remove meta file=%s, errno=%d", meta_file.c_str(), errno);
    return RC::FILE_REMOVE;
  }

  std::string data_file = table_data_file(base_dir, name);
  if(unlink(data_file.c_str()) != 0) {
    LOG_ERROR("Failed to remove data file=%s, errno=%d", data_file.c_str(), errno);
    return RC::FILE_REMOVE;
  }

  const int index_num = table_meta_.index_num();
  for (int i = 0; i < index_num; i ++) {
    ((BplusTreeIndex*)indexes_[i])->close();
    const IndexMeta *index_meta = table_meta_.index(i);
    std::string index_file = table_index_file(base_dir, name, index_meta->name());
    if(unlink(index_file.c_str()) != 0) {
      LOG_ERROR("Failed to remove index file=%s, errno=%d", index_file.c_str(), errno);
      return RC::FILE_REMOVE;
    }
  }

  LOG_INFO("Successfully drop table %s:%s", base_dir, name);
  return rc;
}

RC Table::open(const char *meta_file, const char *base_dir)
{
  // 加载元数据文件
  std::fstream fs;
  std::string meta_file_path = std::string(base_dir) + common::FILE_PATH_SPLIT_STR + meta_file;
  fs.open(meta_file_path, std::ios_base::in | std::ios_base::binary);
  if (!fs.is_open()) {
    LOG_ERROR("Failed to open meta file for read. file name=%s, errmsg=%s", meta_file_path.c_str(), strerror(errno));
    return RC::IOERR_OPEN;
  }
  if (table_meta_.deserialize(fs) < 0) {
    LOG_ERROR("Failed to deserialize table meta. file name=%s", meta_file_path.c_str());
    fs.close();
    return RC::INTERNAL;
  }
  fs.close();

  // 加载数据文件
  RC rc = init_record_handler(base_dir);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to open table %s due to init record handler failed.", base_dir);
    // don't need to remove the data_file
    return rc;
  }

  base_dir_ = base_dir;

  const int index_num = table_meta_.index_num();
  std::vector<FieldMeta> multi_field_metas;
  for (int i = 0; i < index_num; i++) {
    const IndexMeta *index_meta = table_meta_.index(i);
    std::vector<FieldMeta> multi_field_metas;
    for (int j = 0; j < index_meta->field_amount(); j++) {
      const FieldMeta *field_meta = table_meta_.field(index_meta->field(j));
      if (field_meta == nullptr) {
        LOG_ERROR("Found invalid index meta info which has a non-exists field. table=%s, index=%s, field=%s",
              name(), index_meta->name(), index_meta->field(j));
        // skip cleanup here, do all cleanup action in destructive Table function
        return RC::INTERNAL;
      }
      multi_field_metas.emplace_back(*field_meta);
    }

    BplusTreeIndex *index = new BplusTreeIndex(this);
    std::string index_file = table_index_file(base_dir, name(), index_meta->name());
    rc = index->open(index_file.c_str(), *index_meta, multi_field_metas);
    if (rc != RC::SUCCESS) {
      delete index;
      LOG_ERROR("Failed to open index. table=%s, index=%s, file=%s, rc=%s",
              name(), index_meta->name(), index_file.c_str(), strrc(rc));
      // skip cleanup here, do all cleanup action in destructive Table function
      return rc;
    }
    indexes_.push_back(index);
  }

  return rc;
}

/**
 * 创建索引，然后遍历所有数据，插入索引, 最后将索引放到表的元数据中, 并且将元数据写入文件
 * @param trx 事务
 * @param multi_field_metas 多个字段的元数据
 * @param index_name 索引名称
 * @param is_unique 是否是唯一索引
 */
RC Table::create_index(Trx *trx, std::vector<const FieldMeta *> &multi_field_metas, const char *index_name, bool is_unique)
{
  if (common::is_blank(index_name) || multi_field_metas.empty()) {
    LOG_INFO("Invalid input arguments, table name is %s, index_name is blank or attribute_name is blank", name());
    return RC::INVALID_ARGUMENT;
  }

  IndexMeta new_index_meta;
  RC rc = new_index_meta.init(is_unique, index_name, multi_field_metas);
  if (rc != RC::SUCCESS) {
    LOG_INFO("Failed to init IndexMeta in table:%s, index_name:%s, field_amount:%d",
             name(), index_name, multi_field_metas.size());
    return rc;
  }

  // 创建索引相关数据
  BplusTreeIndex *index = new BplusTreeIndex(this);
  std::string index_file = table_index_file(base_dir_.c_str(), name(), index_name);
  std::vector<FieldMeta> new_multi_field_metas;
  for (int i = 0; i < multi_field_metas.size(); i++) {
    new_multi_field_metas.emplace_back(*(multi_field_metas[i]));
  }
  rc = index->create(index_file.c_str(), new_index_meta, new_multi_field_metas);
  if (rc != RC::SUCCESS) {
    delete index;
    LOG_ERROR("Failed to create bplus tree index. file name=%s, rc=%d:%s", index_file.c_str(), rc, strrc(rc));
    return rc;
  }

  // 遍历当前的所有数据，插入这个索引
  RecordFileScanner scanner;
  rc = get_record_scanner(scanner, trx, true/*readonly*/);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create scanner while creating index. table=%s, index=%s, rc=%s",
             name(), index_name, strrc(rc));
    return rc;
  }

  Record record;
  while (scanner.has_next()) {
    rc = scanner.next(record);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to scan records while creating index. table=%s, index=%s, rc=%s",
               name(), index_name, strrc(rc));
      return rc;
    }
    rc = index->insert_entry(record.data(), &record.rid());
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to insert record into index while creating index. table=%s, index=%s, rc=%s",
               name(), index_name, strrc(rc));
      return rc;
    }
  }
  scanner.close_scan();
  LOG_INFO("inserted all records into new index. table=%s, index=%s", name(), index_name);

  indexes_.push_back(index);

  /// 接下来将这个索引放到表的元数据中
  TableMeta new_table_meta(table_meta_);
  rc = new_table_meta.add_index(new_index_meta);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to add index (%s) on table (%s). error=%d:%s", index_name, name(), rc, strrc(rc));
    return rc;
  }

  /// 内存中有一份元数据，磁盘文件也有一份元数据。修改磁盘文件时，先创建一个临时文件，写入完成后再rename为正式文件
  /// 这样可以防止文件内容不完整
  // 创建元数据临时文件
  std::string tmp_file = table_meta_file(base_dir_.c_str(), name()) + ".tmp";
  std::fstream fs;
  fs.open(tmp_file, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
  if (!fs.is_open()) {
    LOG_ERROR("Failed to open file for write. file name=%s, errmsg=%s", tmp_file.c_str(), strerror(errno));
    return RC::IOERR_OPEN;  // 创建索引中途出错，要做还原操作
  }
  if (new_table_meta.serialize(fs) < 0) {
    LOG_ERROR("Failed to dump new table meta to file: %s. sys err=%d:%s", tmp_file.c_str(), errno, strerror(errno));
    return RC::IOERR_WRITE;
  }
  fs.close();

  // 覆盖原始元数据文件
  std::string meta_file = table_meta_file(base_dir_.c_str(), name());
  int ret = rename(tmp_file.c_str(), meta_file.c_str());
  if (ret != 0) {
    LOG_ERROR("Failed to rename tmp meta file (%s) to normal meta file (%s) while creating index (%s) on table (%s). "
        "system error=%d:%s",
        tmp_file.c_str(), meta_file.c_str(), index_name, name(), errno, strerror(errno));
    return RC::IOERR_WRITE;
  }

  table_meta_.swap(new_table_meta);

  LOG_INFO("Successfully added a new index (%s) on the table (%s)", index_name, name());
  return rc;
}

RC Table::insert_record(Record &record)
{
  RC rc = RC::SUCCESS;
  rc = record_handler_->insert_record(record.data(), table_meta_.record_size(), &record.rid());
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Insert record failed. table name=%s, rc=%s", table_meta_.name(), strrc(rc));
    return rc;
  }

  // TODO [Lab2] 增加索引的处理逻辑

  return rc;
}

RC Table::delete_record(const Record &record)
{
  RC rc = RC::SUCCESS;

  // TODO [Lab2] 增加索引的处理逻辑

  rc = record_handler_->delete_record(&record.rid());
  return rc;
}

RC Table::visit_record(const RID &rid, bool readonly, std::function<void(Record &)> visitor)
{
  return record_handler_->visit_record(rid, readonly, visitor);
}

RC Table::get_record(const RID &rid, Record &record)
{
  const int record_size = table_meta_.record_size();
  char *record_data = (char *)malloc(record_size);
  ASSERT(nullptr != record_data, "failed to malloc memory. record data size=%d", record_size);

  auto copier = [&record, record_data, record_size](Record &record_src) {
    memcpy(record_data, record_src.data(), record_size);
    record.set_rid(record_src.rid());
  };
  RC rc = record_handler_->visit_record(rid, true/*readonly*/, copier);
  if (rc != RC::SUCCESS) {
    free(record_data);
    LOG_WARN("failed to visit record. rid=%s, table=%s, rc=%s", rid.to_string().c_str(), name(), strrc(rc));
    return rc;
  }

  record.set_data_owner(record_data, record_size);
  return rc;
}

RC Table::make_record(int value_num, const Value *values, Record &record)
{
  // 检查字段类型是否一致
  if (value_num + table_meta_.sys_field_num() + table_meta_.null_filed_num() != table_meta_.field_num()) {
    LOG_WARN("Input values don't match the table's schema, table name:%s", table_meta_.name());
    return RC::SCHEMA_FIELD_MISSING;
  }

  const int normal_field_start_index = table_meta_.sys_field_num();
  for (int i = 0; i < value_num; i++) {
    const FieldMeta *field = table_meta_.field(i + normal_field_start_index);
    const Value &value = values[i];
    if (AttrType::NULLS == value.attr_type()) {
      if (!field->nullable()) {
        LOG_ERROR("Invalid value type. table name =%s, field name=%s, type=%d, but given=%d",
                  table_meta_.name(), field->name(), field->type(), value.attr_type());
        return RC::SCHEMA_FIELD_TYPE_MISMATCH;
      }
      continue;
    }
    if (field->type() != value.attr_type() && type_cast_not_support(field->type(), value.attr_type())) {
      LOG_ERROR("Invalid value type. table name =%s, field name=%s, type=%d, but given=%d",
                table_meta_.name(), field->name(), field->type(), value.attr_type());
      return RC::SCHEMA_FIELD_TYPE_MISMATCH;
    }
  }

  // 复制所有字段的值
  int record_size = table_meta_.record_size();
  char *record_data = (char *)malloc(record_size);

  RC rc = RC::SUCCESS;
  for (int i = 0; i < value_num; i++) {
    rc = change_record_value(record_data, i + normal_field_start_index, values[i]);
    if (RC::SUCCESS != rc) {
      LOG_ERROR("Change Record Value Failed. RC = %d", rc);
      return rc;
    }
  }
  record.set_data_owner(record_data, record_size);
  return RC::SUCCESS;
}

RC Table::init_record_handler(const char *base_dir)
{
  std::string data_file = table_data_file(base_dir, table_meta_.name());

  RC rc = BufferPoolManager::instance().open_file(data_file.c_str(), data_buffer_pool_);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to open disk buffer pool for file:%s. rc=%d:%s", data_file.c_str(), rc, strrc(rc));
    return rc;
  }

  record_handler_ = new RecordFileHandler();
  rc = record_handler_->init(data_buffer_pool_);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to init record handler. rc=%s", strrc(rc));
    data_buffer_pool_->close_file();
    data_buffer_pool_ = nullptr;
    delete record_handler_;
    record_handler_ = nullptr;
    return rc;
  }

  return rc;
}

RC Table::get_record_scanner(RecordFileScanner &scanner, Trx *trx, bool readonly)
{
  RC rc = scanner.open_scan(this, *data_buffer_pool_, trx, readonly, nullptr);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("failed to open scanner. rc=%s", strrc(rc));
  }
  return rc;
}


Index *Table::find_index(const char *index_name) const
{
  for (Index *index : indexes_) {
    if (0 == strcmp(index->index_meta().name(), index_name)) {
      return index;
    }
  }
  return nullptr;
}

Index *Table::find_index_by_field(const char *field_name) const
{
  const TableMeta &table_meta = this->table_meta();
  const IndexMeta *index_meta = table_meta.find_index_by_field(field_name);
  if (index_meta != nullptr) {
    return this->find_index(index_meta->name());
  }
  return nullptr;
}

/**
 * 将索引数据刷到磁盘
 */
RC Table::sync()
{
  RC rc = RC::SUCCESS;
  for (Index *index : indexes_) {
    rc = index->sync();
    if (rc != RC::SUCCESS) {
      LOG_ERROR("Failed to flush index's pages. table=%s, index=%s, rc=%d:%s",
                name(),
                index->index_meta().name(),
                rc,
                strrc(rc));
      return rc;
    }
  }
  LOG_INFO("Sync table over. table=%s", name());
  return rc;
}

RC Table::change_record_value(char *&record, int idx, const Value &value) const
{
  const FieldMeta *null_field = table_meta_.null_bitmap_field();
  common::Bitmap bitmap(record + null_field->offset(), null_field->len());

  const FieldMeta *field = table_meta_.field(idx);
  if (AttrType::NULLS == value.attr_type()) {
    if (!field->nullable()) {
      LOG_ERROR("Invalid value type. Cannot be null. table name =%s, field name=%s, type=%d, but given=%d",
          table_meta_.name(),
          field->name(),
          field->type(),
          value.attr_type());
      return RC::SCHEMA_FIELD_TYPE_MISMATCH;
    }
    bitmap.set_bit(idx);

    std::time_t now = std::time(nullptr);
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<std::size_t> distribution(0, std::numeric_limits<std::size_t>::max());
    std::size_t randomValue = distribution(generator);
    std::size_t combinedValue = now + randomValue;
    std::hash<std::size_t> hasher;
    std::size_t hashValue = hasher(combinedValue);
    memcpy(record + field->offset(), &hashValue, field->len());
    return RC::SUCCESS;
  }
  bitmap.clear_bit(idx);

  size_t copy_len = field->len();
  if (field->type() == CHARS  || field->type() == TEXTS) {
    Value tmp = value;
    if(value.attr_type() != CHARS && value.attr_type() != TEXTS) {
      tmp = Value(value.get_string().c_str());
    }
    const size_t data_len = tmp.length();
    if (copy_len > data_len) {
      copy_len = data_len + 1;
    }
    memcpy(record + field->offset(), tmp.data(), copy_len);
  } else if (field->type() == INTS || field->type() == DATES || field->type() == BOOLEANS) {
    int tmp = value.get_int();
    memcpy(record + field->offset(), &tmp, copy_len);
  } else if (field->type() == FLOATS){
    float tmp = value.get_float();
    memcpy(record + field->offset(), &tmp, copy_len);
  }

  return RC::SUCCESS;
}

/**
 * TODO [Lab5] 在故障恢复时，需要将记录插入到表中，其他Lab不需要处理。
 */
RC Table::recover_insert_record(Record &record)
{
  return RC::SUCCESS;
}

const char *Table::name() const
{
  return table_meta_.name();
}
const char *Table::dir() const
{
  return base_dir_.c_str();
}

const TableMeta &Table::table_meta() const
{
  return table_meta_;
}