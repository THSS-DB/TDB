#pragma once

#include "include/common/setting.h"
#include <cstring>
#include <sstream>

using TrxID = int32_t;

static constexpr int BP_INVALID_PAGE_NUM = -1;
static constexpr PageNum BP_HEADER_PAGE = 0;
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

/**
 * @brief 文件第一个页面，存放一些元数据信息，包括了后面每页的分配信息。
 */
struct FileHeader
{
  int32_t page_count;       // 当前文件一共有多少个页面
  int32_t allocated_pages;  // 已经分配了多少个页面
  char bitmap[0];           // 页面分配位图, 第0个页面(就是当前页面)，总是1

  // 能够分配的最大的页面个数，即bitmap的字节数 乘以8
  static const int MAX_PAGE_NUM = (BP_PAGE_DATA_SIZE - sizeof(page_count) - sizeof(allocated_pages)) * 8;

  std::string to_string() const
  {
    std::stringstream ss;
    ss << "pageCount:" << page_count
       << ", allocatedCount:" << allocated_pages;
    return ss.str();
  }
};