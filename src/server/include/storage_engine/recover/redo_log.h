#pragma once

#include <cstdint>
#include <atomic>
#include <deque>
#include <memory>
#include <string>

#include "include/storage_engine/recorder/record.h"
#include "common/lang/mutex.h"

/**
 * TODO [Lab5] 需要同学们完成重做日志的实现
 */

class RedoLogManager 
{
public:
  RedoLogManager() = default;
  ~RedoLogManager();

  RC init(const char *path);

};
