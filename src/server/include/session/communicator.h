// Session部分内容参考以及复用了miniOB的Client-Server架构，在这里感谢他们开源代码~

#pragma once

#include <string>
#include <event.h>
#include "include/common/rc.h"
#include "include/query_engine/executor/sql_result.h"
#include "include/session/buffered_writer.h"

class SessionRequest;
class Session;


/**
 * @defgroup Communicator
 * @brief 负责处理与客户端的通讯
 * @details 当前有两种通讯协议，一种是普通的文本协议，以'\0'作为结尾，一种是mysql协议。
 */

/**
 * @brief 负责与客户端通讯
 * @ingroup Communicator
 *
 * @details 在listener接收到一个新的连接(参考 server.cpp::accept), 就创建一个Communicator对象。
 * 并调用init进行初始化。
 * 在server中监听到某个连接有新的消息，就通过Communicator::read_event接收消息。

 */
class Communicator 
{
public:
  virtual ~Communicator();

  /**
   * @brief 接收到一个新的连接时，进行初始化
   */
  virtual RC init(int fd, Session *session, const std::string &addr);

  /**
   * @brief 监听到有新的数据到达，调用此函数进行接收消息
   * 如果需要创建新的任务来处理，那么就创建一个SessionEvent 对象并通过event参数返回。
   */
  virtual RC read_event(SessionRequest *&event) = 0;

  /**
   * @brief 关联的会话信息
   */
  Session *session() const
  {
    return session_;
  }

  /**
   * @brief libevent使用的数据，参考server.cpp
   */
  struct event &read_event()
  {
    return read_event_;
  }

  /**
   * @brief 对端地址
   * 如果是unix socket，可能没有意义
   */
  const char *addr() const
  {
    return addr_.c_str();
  }

  virtual RC write_state(SqlResult *sql_result, bool &need_disconnect) = 0;

  virtual RC write_result(const char *data, int32_t size) = 0;

  RC send_message_delimiter(){
    return writer_->writen(send_message_delimiter_.data(), send_message_delimiter_.size());
  }

  void flush(){
    writer_->flush();
  }

protected:
  Session *session_ = nullptr;
  struct event read_event_;
  std::string addr_;
  BufferedWriter *writer_ = nullptr;
  std::vector<char> send_message_delimiter_; ///< 发送消息分隔符
  int fd_ = -1;
};

/**
 * @brief 当前支持的通讯协议
 * @ingroup Communicator
 */
enum class CommunicateProtocol 
{
  PLAIN,  ///< 以'\0'结尾的协议
  CLI,    ///< 与客户端进行交互的协议
  MYSQL,  ///< mysql通讯协议。具体实现参考 MysqlCommunicator
};

/**
 * @brief 通讯协议工厂
 * @ingroup Communicator
 */
class CommunicatorFactory 
{
public:
  Communicator *create(CommunicateProtocol protocol);
};
