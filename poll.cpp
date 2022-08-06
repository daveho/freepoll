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

#include "timestamp.h"
#include "poll.h"

Poll::Poll() {
}

void Poll::start() {
  std::lock_guard<std::mutex> guard(m_lock);
  m_start_wall = ts::millis_since_epoch();
  m_start_mono = std::chrono::steady_clock::now();
}

void Poll::record_response(RemoteID remote_id, Option option) {
  // We use a monotonically-increasing clock to determine how long
  // after the poll started this response was submitted.
  auto response_time_mono = std::chrono::steady_clock::now();
  auto elapsed_millis = std::chrono::duration_cast<std::chrono::milliseconds>(response_time_mono - m_start_mono);

  // Package the response data
  Response resp(remote_id, option, m_start_wall + elapsed_millis.count());

  // Find vector of Response objects for the specified RemoteID
  std::vector<Response> &responses_for_remote_id = m_responses[remote_id];

  // Store the Response
  responses_for_remote_id.push_back(resp);
}

Timestamp Poll::get_start_time() const {
  std::lock_guard<std::mutex> guard(m_lock);
  return m_start_wall;
}

std::map<RemoteID, Option> Poll::get_final_responses() const {
  std::lock_guard<std::mutex> guard(m_lock);

  std::map<RemoteID, Option> result;

  for (auto i = m_responses.begin(); i != m_responses.end(); ++i) {
    result[i->first] = i->second.back().get_option();
  }

  return result;
}

const std::map<RemoteID, std::vector<Response>> Poll::get_all_responses() const {
  std::lock_guard<std::mutex> guard(m_lock);

  auto result = m_responses;

  return result;
}
