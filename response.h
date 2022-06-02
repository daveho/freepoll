#ifndef RESPONSE_H
#define RESPONSE_H

#include "datatypes.h"

// A Response is the result of a student clicking on a multiple choice
// option (A-E) in a poll.
class Response {
private:
  RemoteID m_remote_id;
  Option m_option;
  Timestamp m_timestamp;

public:
  Response(RemoteID remote_id, Option option, Timestamp timestamp)
    : m_remote_id(remote_id), m_option(option), m_timestamp(timestamp) { }

  RemoteID get_remote_id() const  { return m_remote_id; }
  Option get_option() const       { return m_option; }
  Timestamp get_timestamp() const { return m_timestamp; }
};

#endif // RESPONSE_H
