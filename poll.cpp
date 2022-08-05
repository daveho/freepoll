// Copyright (c) 2022, David Hovemeyer <david.hovemeyer@gmail.com>

// This file is part of FreePoll.
//
// FreePoll is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// FreePoll is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with FreePoll. If not, see <https://www.gnu.org/licenses/>.

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
  std::vector<Response> &responses_for_remote_id = m_responses[remote_id];

  // Store the Response
  responses_for_remote_id.push_back(resp);
}
