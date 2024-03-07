#pragma once

class BufferPoolManager;
class DefaultHandler;
class TrxManager;

/**
 * @brief 放一些全局对象
 * @details 为了更好的管理全局对象，这里将其封装到一个类中。初始化的过程可以参考 init_global_objects
 */
struct GlobalContext
{
  BufferPoolManager *buffer_pool_manager_ = nullptr;
  DefaultHandler *handler_ = nullptr;
  TrxManager *trx_manager_ = nullptr;

  static GlobalContext &instance();
};

#define GCTX GlobalContext::instance()