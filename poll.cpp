#include <cassert>
#include "timestamp.h"
#include "poll.h"

Poll::Poll() {
}

void Poll::start() {
  m_start_wall = ts::get_timestamp(CLOCK_REALTIME);
  m_start_mono = ts::get_timestamp(CLOCK_MONOTONIC);
}

void Poll::record_response(RemoteID remote_id, Option option) {
  // We use a monotonically-increasing clock to determine how long
  // after the poll started this response was submitted.
  Timestamp response_time_mono = ts::get_timestamp(CLOCK_MONOTONIC);
  assert(response_time_mono >= m_start_mono);
  Timestamp offset = m_start_mono - response_time_mono;

  // Package the response data
  Response resp(remote_id, option, m_start_wall + offset);

  // Find vector of Response objects for the specified RemoteID
  auto i = m_responses.find(remote_id);
  std::vector<Response> &responses_for_remote_id =
    (i == m_responses.end()) ? m_responses[remote_id] : i->second;

  // Store the Response
  responses_for_remote_id.push_back(resp);
}
