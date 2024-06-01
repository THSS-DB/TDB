#pragma once

#include <functional>
#include <vector>

#include "include/storage_engine/recorder/table_meta.h"
#include "include/query_engine/parser/value.h"
#include "include/storage_engine/buffer/buffer_pool.h"
#include "include/storage_engine/recorder/record.h"

class RecordFileScanner;
class RecordFileHandler;
class Index;

/**
 * @brief 表
 */
class Table 
{
public:
  Table() = default;
  ~Table();

  /**
   * 创建一个表
   * @param path 元数据保存的文件(完整路径)
   * @param name 表名
   * @param base_dir 表数据存放的路径
   * @param attribute_count 字段个数
   * @param attributes 字段
   */
  RC create(int32_t table_id, 
      const char *path,
      const char *name,
      const char *base_dir,
      int attribute_count,
      const AttrInfoSqlNode attributes[]);

  /**
   * 创建一个视图
   * @param path 视图元数据保存的文件(完整路径)
   * @param name 视图名
   * @param select_stmt 视图的 select 语句
   */
  RC create(int32_t table_id,
      const char *name,
      const char *origin_table_name,
      SelectStmt *select_stmt,
      int attribute_count,
      const AttrInfoSqlNode attributes[]);

  /**
   * 删除一个表
   * @param name 表名
   * @param base_dir 表数据存放的路径
   */
  RC drop(int32_t table_id,
      const char *name,
      const char *base_dir);

  /**
   * 打开一个表
   * @param meta_file 保存表元数据的文件完整路径
   * @param base_dir 表所在的文件夹，表记录数据文件、索引数据文件存放位置
   */
  RC open(const char *meta_file, const char *base_dir);

  /**
   * @brief 根据给定的字段生成一个记录/行
   * @details 通常是由用户传过来的字段，按照schema信息组装成一个record。
   * @param value_num 字段的个数
   * @param values    每个字段的值
   * @param record    生成的记录数据
   */
  RC make_record(int value_num, const Value *values, Record &record);

  /**
   * @brief 在当前的表中插入一条记录
   * @details 在表文件和索引中插入关联数据。这里只管在表中插入数据，不关心事务相关操作。
   * @param record[in/out] 传入的数据包含具体的数据，插入成功会通过此字段返回RID
   */
  RC insert_record(Record &record);
  RC delete_record(const Record &record);
  RC visit_record(const RID &rid, bool readonly, std::function<void(Record &)> visitor);
  RC get_record(const RID &rid, Record &record);

  RC recover_insert_record(Record &record);

  RC create_index(Trx *trx, std::vector<const FieldMeta *> &multi_field_metas, const char *index_name, bool is_unique);

  RC get_record_scanner(RecordFileScanner &scanner, Trx *trx, bool readonly);

  RC get_record_scanner(RecordFileScanner &scanner, Trx *trx, bool readonly, std::vector<std::unique_ptr<Expression>> predicate_exprs);

  RecordFileHandler *record_handler() const
  {
    return record_handler_;
  }

public:
  int32_t table_id() const { return table_meta_.table_id(); }
  const char *name() const;
  const char *dir() const;

  const TableMeta &table_meta() const;

  const bool is_view() const { return table_meta_.is_view(); }
  const char *origin_table_name() const { return table_meta_.origin_table_name(); }
  SelectStmt *select_stmt() { return table_meta_.select_stmt(); }

  RC sync();

private:
  RC insert_entry_of_indexes(const char *record, const RID &rid);
  RC delete_entry_of_indexes(const char *record, const RID &rid, bool error_on_not_exists);

private:
  RC init_record_handler(const char *base_dir);
  RC change_record_value(char *&record, int idx, const Value &value) const;

public:
  Index *find_index(const char *index_name) const;
  Index *find_index_by_field(const char *field_name) const;

private:
  std::string base_dir_;
  TableMeta   table_meta_;
  FileBufferPool *data_buffer_pool_ = nullptr;   /// 数据文件关联的buffer pool
  RecordFileHandler *record_handler_ = nullptr;  /// 记录操作
  std::vector<Index *> indexes_;
};
