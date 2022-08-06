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

#ifndef POLL_H
#define POLL_H

#include <map>
#include <vector>
#include <chrono>
#include <mutex>
#include "response.h"

// A Poll is a single multiple choice poll.
// Collects responses, organized by RemoteID.
// Each response is recorded with a Timestamp value based
// on
//
//    - the wall time when the Poll was started (milliseconds
//      since epoch), and
//
//    - the number of milliseconds since the poll start time
//      when the response was recorded
//
// A std::mutex is used so that the object is safe to access from
// multiple threads.
class Poll {
private:
  std::map<RemoteID, std::vector<Response>> m_responses;
  Timestamp m_start_wall;
  std::chrono::time_point<std::chrono::steady_clock> m_start_mono;
  mutable std::mutex m_lock;

public:
  Poll();

  // call this when the poll starts so that the start time is recorded
  void start();

  // record one response, which will be timestamped based on the current
  // (computed) wall clock time
  void record_response(RemoteID remote_id, Option option);

  // Get the poll's start time (as milliseconds since the epoch)
  Timestamp get_start_time() const;

  // Get just the "effective" responses, i.e., each student's
  // final response before the poll was close
  std::map<RemoteID, Option> get_final_responses() const;

  // Get all responses (ordered by timestamp) for each student
  const std::map<RemoteID, std::vector<Response>> get_all_responses() const;
};

#endif // POLL_H
