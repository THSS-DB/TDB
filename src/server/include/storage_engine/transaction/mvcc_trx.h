#pragma once

#include "include/storage_engine/transaction/trx.h"

/**
* @brief MVCC(多版本并发控制)事务管理器
 */
class MvccTrxManager : public TrxManager
{
public:
  MvccTrxManager() = default;
  virtual ~MvccTrxManager();

  RC init() override;
  const std::vector<FieldMeta> *trx_fields() const override;
  Trx *create_trx(LogManager *log_manager) override;
  Trx *create_trx(int32_t trx_id) override;
  // 找到对应事务号的事务，当前仅在recover场景下使用
  Trx *find_trx(int32_t trx_id) override;
  void all_trxes(std::vector<Trx *> &trxes) override;
  void destroy_trx(Trx *trx) override;

  int32_t next_trx_id();
  int32_t max_trx_id() const;

  // 在 recover 场景下使用，确保当前事务 id 不小于 trx_id
  void update_trx_id(int32_t trx_id);

private:
  std::vector<FieldMeta> fields_; // 存储事务数据需要用到的字段元数据，所有表结构都需要带
  std::atomic<int32_t> current_trx_id_{0};
  common::Mutex      lock_;
  std::vector<Trx *> trxes_;
};

class MvccTrx : public Trx
{
 public:
  MvccTrx(MvccTrxManager &trx_kit, LogManager *log_manager);
  MvccTrx(MvccTrxManager &trx_kit, int32_t trx_id); // used for recover
  virtual ~MvccTrx() = default;

  TrxType type() override { return MVCC; }

  RC insert_record(Table *table, Record &record) override;
  RC delete_record(Table *table, Record &record) override;

  /**
   * @brief 当访问到某条数据时，使用此函数来判断是否可见，或者是否有访问冲突
   * @param table    要访问的数据属于哪张表
   * @param record   要访问哪条数据
   * @param readonly 是否只读访问
   * @return RC      - SUCCESS 成功
   *                 - RECORD_INVISIBLE 此数据对当前事务不可见，应该跳过
   *                 - LOCKED_CONCURRENCY_CONFLICT 与其它事务有冲突
   */
  RC visit_record(Table *table, Record &record, bool readonly) override;

  RC start_if_need() override;
  RC commit() override;
  RC rollback() override;

  RC redo(Db *db, const LogEntry &log_entry) override;

  int32_t id() const override { return trx_id_; }

 private:
  /**
   * @brief 获取指定表上的与版本号相关的字段
   * @param table 指定的表
   * @param begin_xid_field 返回begin_xid的字段
   * @param end_xid_field   返回end_xid的字段
   */
  void trx_fields(Table *table, Field &begin_xid_field, Field &end_xid_field) const;

  /**
   * @brief 使用指定的版本号来提交事务
   * @param commit_xid
   * @return
   */
  RC commit_with_trx_id(int32_t commit_xid);

 private:
  static const int32_t MAX_TRX_ID = std::numeric_limits<int32_t>::max();

 private:
  using OperationSet = std::unordered_set<Operation, OperationHasher, OperationEqualer>;
  MvccTrxManager & trx_kit_;
  LogManager *log_manager_ = nullptr;
  int32_t      trx_id_ = -1;
  bool         started_ = false;
  bool         recovering_ = false;
  OperationSet operations_;
};
