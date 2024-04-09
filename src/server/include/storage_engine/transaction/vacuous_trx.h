#pragma once

#include "include/storage_engine/transaction/trx.h"

/**
* @brief Vacuous(真空的)，顾名思义就是没有实现事务功能
*/
class VacuousTrxManager : public TrxManager
{
public:
 VacuousTrxManager() = default;
 virtual ~VacuousTrxManager() = default;

 RC init() override;
 const std::vector<FieldMeta> *trx_fields() const override;
 Trx *create_trx(RedoLogManager *log_manager) override;
 Trx *create_trx(int32_t trx_id) override;
 Trx *find_trx(int32_t trx_id) override;
 void all_trxes(std::vector<Trx *> &trxes) override;
 void destroy_trx(Trx *trx) override;
};

class VacuousTrx : public Trx
{
public:
 VacuousTrx() = default;
 virtual ~VacuousTrx() = default;

 TrxType type() override { return VACUOUS; }

 RC insert_record(Table *table, Record &record) override;
 RC delete_record(Table *table, Record &record) override;
 RC visit_record(Table *table, Record &record, bool readonly) override;

 RC start_if_need() override;
 RC commit() override;
 RC rollback() override;

 int32_t id() const override { return 0; }
};
