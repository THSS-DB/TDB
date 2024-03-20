#pragma once

#include "common/conf/ini.h"
#include "common/os/process_param.h"

int init(common::ProcessParam *processParam);
void cleanup();
