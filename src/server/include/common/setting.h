#pragma once

#include <cstdint>
#include <ctime>

#define CLIENT_ADDRESS "CLIENT_ADDRESS"
#define MAX_CONNECTION_NUM "MAX_CONNECTION_NUM"
#define MAX_CONNECTION_NUM_DEFAULT (65535*2)
#define PORT "PORT"
#define PORT_DEFAULT 6789

#define SESSION_STAGE_NAME "SessionStage"

/* 磁盘文件，包括存放数据的文件和索引(B+Tree)文件，都按照页来组织。每一页都有一个编号，称为PageNum */
using PageNum = int32_t;

/* 数据文件中按照页来组织，每一页会存放一些行数据(row)，或称为记录(record)。每一行(row/record)，都占用一个槽位(slot)，这些槽有一个编号，称为SlotNum */
using SlotNum = int32_t;

/* LSN for log sequence number */
using LSN = int32_t;