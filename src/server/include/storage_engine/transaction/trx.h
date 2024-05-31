#pragma once

#include <cstddef>
#include <mutex>
#include <unordered_set>
#include <utility>

#include "common/lang/string.h"
#include "common/log/log.h"
#include "include/storage_engine/recorder/field.h"
#include "include/storage_engine/recorder/table.h"
#include "include/storage_engine/recover/log_entry.h"
#include "include/storage_engine/recover/log_manager.h"

class RID;
class Record;


/**
 * @brief 描述一个操作，比如插入、删除行等
 * @details 通常包含一个操作的类型，以及操作的对象和具体的数据
 */
class Operation 
{
public:
  /**
   * @brief 操作的类型
   */
  enum class Type : int 
  {
    INSERT,
    UPDATE,
    DELETE,
    UNDEFINED,
  };

public:
  Operation(Type type, Table *table, const RID &rid) 
      : type_(type), 
        table_(table),
        page_num_(rid.page_num), 
        slot_num_(rid.slot_num)
  {}

  Type    type() const { return type_; }
  int32_t table_id() const { return table_->table_id(); }
  Table * table() const { return table_; }
  PageNum page_num() const { return page_num_; }
  SlotNum slot_num() const { return slot_num_; }

private:
  Type type_;
  Table * table_ = nullptr;  // 操作的哪张表
  PageNum page_num_;
  SlotNum slot_num_;
};

class OperationHasher 
{
public:
  size_t operator()(const Operation &op) const
  {
    // 哈希时考虑操作类型
    return (((size_t)op.page_num()) << 32) | ((size_t)(op.slot_num()) << 2) | (size_t)op.type();
  }
};

class OperationEqualer 
{
public:
  bool operator()(const Operation &op1, const Operation &op2) const
  {
    // 判断重复时考虑操作类型
    return op1.table_id() == op2.table_id() &&
        op1.page_num() == op2.page_num() && op1.slot_num() == op2.slot_num()
        && op1.type() == op2.type();
  }
};

/**
 * @brief 事务管理器的类型
 * @details 进程启动时根据事务管理器的类型来创建具体的对象
 */
enum TrxType
{
  VACUOUS,  // 空的事务管理器，不做任何事情
  MVCC,  // 支持MVCC的事务管理器
};

/**
 * @brief 事务接口
 */
class Trx
{
public:
  Trx() = default;
  virtual ~Trx() = default;

  virtual TrxType type() = 0;

  virtual RC insert_record(Table *table, Record &record) = 0;
  virtual RC delete_record(Table *table, Record &record) = 0;
  virtual RC visit_record(Table *table, Record &record, bool readonly) = 0;

  virtual RC start_if_need() = 0;
  virtual RC commit() = 0;
  virtual RC rollback() = 0;

  virtual RC redo(Db *db, const LogEntry &log_entry) = 0;

  virtual int32_t id() const = 0;
};


/**
 * @brief 事务管理器
 */
class TrxManager
{
public:
  TrxManager() = default;
  virtual ~TrxManager() = default;

  virtual RC init() = 0;
  virtual const std::vector<FieldMeta> *trx_fields() const = 0;
  virtual Trx *create_trx(LogManager *log_manager) = 0;
  virtual Trx *create_trx(int32_t trx_id) = 0;
  virtual Trx *find_trx(int32_t trx_id) = 0;
  virtual void all_trxes(std::vector<Trx *> &trxes) = 0;
  virtual void destroy_trx(Trx *trx) = 0;

public:
  static TrxManager *create(const char *name);
  static RC init_global(const char *name);
  static TrxManager *instance();
};

