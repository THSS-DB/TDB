#include "include/storage_engine/recover/redo_log.h"

using namespace std;
using namespace common;

/**
 * TODO [Lab5] 需要同学们完成重做日志的实现
 */

RC RedoLogManager::init(const char *path) { return RC::SUCCESS; }

RedoLogManager::~RedoLogManager() {}
