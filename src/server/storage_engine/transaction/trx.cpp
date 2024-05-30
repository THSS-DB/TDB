#include "include/storage_engine/transaction/trx.h"
#include "include/storage_engine/transaction/vacuous_trx.h"
#include "include/storage_engine/transaction/mvcc_trx.h"
#include "include/storage_engine/recorder/record.h"
#include "include/storage_engine/recorder/record_manager.h"

static TrxManager *global_trx_manager = nullptr;

TrxManager *TrxManager::create(const char *name)
{
  if (common::is_blank(name) || 0 == strcasecmp(name, "vacuous")) {
    return dynamic_cast<TrxManager*>(new VacuousTrxManager());
  }
  if (0 == strcasecmp(name, "mvcc")) {
    return dynamic_cast<TrxManager*>(new MvccTrxManager());
  }
  LOG_ERROR("unknown trx kit name. name=%s", name);
  return nullptr;
}

RC TrxManager::init_global(const char *name)
{
  ASSERT(global_trx_manager == nullptr, "init global trx kit twice");
  TrxManager *trx_manager = create(name);
  if (nullptr == trx_manager) {
    LOG_ERROR("failed to create trx kit by name. name=%s", name);
    return RC::INTERNAL;
  }

  RC rc = trx_manager->init();
  if (rc == RC::SUCCESS) {
    global_trx_manager = trx_manager;
  }
  return rc;
}

TrxManager *TrxManager::instance()
{
  return global_trx_manager;
}
