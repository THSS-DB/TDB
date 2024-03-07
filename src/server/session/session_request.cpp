#include "include/session/session_request.h"
#include "include/session/communicator.h"

SessionRequest::SessionRequest(Communicator *comm)
    : communicator_(comm),
    sql_result_(communicator_->session())
{}

SessionRequest::~SessionRequest()
{
}

Communicator *SessionRequest::get_communicator() const
{
  return communicator_;
}

Session *SessionRequest::session() const
{
  return communicator_->session();
}
