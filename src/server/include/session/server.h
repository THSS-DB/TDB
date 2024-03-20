// Session部分内容参考了miniOB的CS架构实现，在这里感谢他们开源代码~

#pragma once

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <event2/thread.h>

#include "common/defs.h"
#include "common/lang/mutex.h"
#include "common/log/log.h"
#include "common/io/io.h"

#include "include/common/setting.h"
#include "include/query_engine/query_engine.h"
#include "server_param.h"
#include "session_request.h"
#include "session.h"
#include "communicator.h"

class Communicator;
class QueryEngine;

/**
 * @brief 负责接收客户端消息并创建任务
 * @ingroup Communicator
 * @details 当前支持网络连接，有TCP和Unix Socket两种方式。通过命令行参数来指定使用哪种方式。
 * 启动后监听端口或unix socket，使用libevent来监听事件，当有新的连接到达时，创建一个Communicator对象进行处理。
 */
class Server 
{
public:
  Server(ServerParam input_server_param);
  ~Server();

public:
  static void init();
  static void close_connection(Communicator *comm);

public:
  int serve();
  void shutdown();

private:
  /**
   * @brief 接收到新的连接时，调用此函数创建Communicator对象
   * @details 此函数作为libevent中监听套接字对应的回调函数
   * @param fd libevent回调函数传入的参数，即监听套接字
   * @param ev 本次触发的事件，通常是EV_READ
   * @param arg 在注册libevent回调函数时，传入的参数，即Server对象
   */
  static void accept(int fd, short ev, void *arg);
  /**
   * @brief 接收到客户端消息时，调用此函数创建任务
   * @details 此函数作为libevent中客户端套接字对应的回调函数。当有新的消息到达时，调用此函数创建任务。
   * @param fd libevent回调函数传入的参数，即客户端套接字
   * @param ev 本次触发的事件，通常是EV_READ
   * @param arg 在注册libevent回调函数时，传入的参数，即Communicator对象
   */
  static void recv(int fd, short ev, void *arg);

private:
  /**
   * @brief 将socket描述符设置为非阻塞模式
   * 
   * @param fd 指定的描述符
   */
  int set_non_block(int fd);

  int start();

  /**
   * @brief 启动TCP服务
   */
  int start_tcp_server();

  /**
   * @brief 启动Unix Socket服务
   */
  int start_unix_socket_server();

  /**
   * @brief 启动标准终端输入服务
   */
  int start_stdin_server();

private:
  volatile bool started_ = false;

  int server_socket_ = -1;  ///< 监听套接字，是一个描述符
  struct event_base *event_base_ = nullptr; ///< libevent对象
  struct event *listen_ev_ = nullptr;  ///< libevent监听套接字事件

  ServerParam server_param_;  ///< 服务启动参数

  CommunicatorFactory communicator_factory_; ///< 通过这个对象创建新的Communicator对象

  static QueryEngine query_engine_;  ///< 通过这个对象处理查询请求
};
