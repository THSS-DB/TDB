/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "include/session/communicator.h"
#include "include/session/plain_communicator.h"
#include "include/session/cli_communicator.h"
#include "include/session/buffered_writer.h"
#include "include/session/session.h"

#include "common/lang/mutex.h"

RC Communicator::init(int fd, Session *session, const std::string &addr)
{
  fd_ = fd;
  session_ = session;
  addr_ = addr;
  writer_ = new BufferedWriter(fd_);
  return RC::SUCCESS;
}

Communicator::~Communicator()
{
  if (fd_ >= 0) {
    close(fd_);
    fd_ = -1;
  }
  if (session_ != nullptr) {
    delete session_;
    session_ = nullptr;
  }

  if (writer_ != nullptr) {
    delete writer_;
    writer_ = nullptr;
  }
}

/////////////////////////////////////////////////////////////////////////////////

Communicator *CommunicatorFactory::create(CommunicateProtocol protocol)
{
  switch (protocol) {
    case CommunicateProtocol::PLAIN: {
      return new PlainCommunicator;
    } break;
    case CommunicateProtocol::CLI: {
      return new CliCommunicator;
    } break;
    default: {
      return nullptr;
    }
  }
}
