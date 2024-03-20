#include "include/common/global_context.h"

static GlobalContext global_context;

GlobalContext &GlobalContext::instance()
{
  return global_context;
}
