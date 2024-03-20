#pragma once

#include <cstring>
#include <vector>

#include "communicator.h"

/**
 * @brief 与客户端进行通讯
 * @ingroup Communicator
 * @details 使用简单的文本通讯协议，每个消息使用'\0'结尾
 */
class PlainCommunicator : public Communicator {
 public:
  PlainCommunicator();
  ~PlainCommunicator() override = default;

  RC read_event(SessionRequest *&event) override;
  RC write_state(SqlResult *sql_result, bool &need_disconnect) override;
  RC write_result(const char *data, int32_t size) override;
};
