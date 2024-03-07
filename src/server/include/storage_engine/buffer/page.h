#pragma once

#include "include/common/setting.h"

using TrxID = int32_t;

static constexpr int BP_INVALID_PAGE_NUM = -1;
static constexpr PageNum BP_HEADER_PAGE   = 0;
static constexpr const int BP_PAGE_SIZE = (1 << 13);  // 8192字节
static constexpr const int BP_PAGE_DATA_SIZE = (BP_PAGE_SIZE - sizeof(PageNum) - sizeof(LSN));

/**
 * @brief 表示一个页面，可能放在内存或磁盘上
 */
struct Page
{
  PageNum page_num;
  LSN     lsn;
  char data[BP_PAGE_DATA_SIZE];
};
