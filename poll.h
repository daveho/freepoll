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
