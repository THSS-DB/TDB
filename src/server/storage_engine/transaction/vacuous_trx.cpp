#include "include/storage_engine/transaction/vacuous_trx.h"

using namespace std;

RC VacuousTrxManager::init()
{
  return RC::SUCCESS;
}

const vector<FieldMeta> *VacuousTrxManager::trx_fields() const
{
  return nullptr;
}

Trx *VacuousTrxManager::create_trx(LogManager *)
{
  return new VacuousTrx;
}

Trx *VacuousTrxManager::create_trx(int32_t /*trx_id*/)
{
  return nullptr;
}

void VacuousTrxManager::destroy_trx(Trx *trx)
{
  delete trx;
}

Trx *VacuousTrxManager::find_trx(int32_t /* trx_id */)
{
  return nullptr;
}

void VacuousTrxManager::all_trxes(std::vector<Trx *> &trxes)
{
  return;
}

////////////////////////////////////////////////////////////////////////////////

RC VacuousTrx::insert_record(Table *table, Record &record)
{
  return table->insert_record(record);
}

RC VacuousTrx::delete_record(Table *table, Record &record)
{
  return table->delete_record(record);
}

RC VacuousTrx::visit_record(Table *table, Record &record, bool readonly)
{
  return RC::SUCCESS;
}

RC VacuousTrx::start_if_need()
{
  return RC::SUCCESS;
}

RC VacuousTrx::commit()
{
  return RC::SUCCESS;
}

RC VacuousTrx::rollback()
{
  return RC::SUCCESS;
}

RC VacuousTrx::redo(Db *db, const LogEntry &log_entry)
{
  return RC::SUCCESS;
}