#ifndef POLL_H
#define POLL_H

#include <map>
#include <vector>
#include "response.h"

// A Poll is a single multiple choice poll.
// Collects responses, organized by RemoteID.
// Each response is recorded with a Timestamp value based
// on
//
//    - the wall time when the Poll was started, and
//
//    - the number of milliseconds since the poll start time
//      when the response was recorded
class Poll {
private:
  std::map<RemoteID, std::vector<Response>> m_responses;
  Timestamp m_start_wall, m_start_mono;

public:
  Poll();

  void start();
  void record_response(RemoteID remote_id, Option option);
};

#endif // POLL_H
