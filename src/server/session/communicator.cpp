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
