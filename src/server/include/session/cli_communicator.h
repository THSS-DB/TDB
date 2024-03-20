// Session部分内容参考以及复用了miniOB的Client-Server架构，在这里感谢他们开源代码~

#pragma once

#include "plain_communicator.h"

/**
 * @brief 用于命令行模式的通讯器
 * @ingroup Communicator
 * @details
 * 直接通过终端/标准输入输出进行通讯。从这里的实现来看，是不需要libevent的一些实现的，
 * 因此communicator需要重构，或者需要重构server中的各个通讯启动模式。
 */
class CliCommunicator : public PlainCommunicator {
 public:
  CliCommunicator() = default;
  ~CliCommunicator() override = default;

  RC init(int fd, Session *session, const std::string &addr) override;
  RC read_event(SessionRequest *&event) override;
  RC write_state(SqlResult *sql_result, bool &need_disconnect) override;
  RC write_result(const char *data, int32_t size) override;

 private:
  int write_fd_ =
      -1;  ///< 与使用远程通讯模式不同，如果读数据使用标准输入，那么输出应该是标准输出
};
