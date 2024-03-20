#pragma once

#include <atomic>
#include <cstdint>
#include <deque>
#include <memory>
#include <string>

#include "common/lang/mutex.h"
#include "include/storage_engine/recorder/record.h"

/**
 * TODO [Lab5] 需要同学们完成重做日志的实现
 */

class RedoLogManager {
 public:
  RedoLogManager() = default;
  ~RedoLogManager();

  RC init(const char *path);
};
