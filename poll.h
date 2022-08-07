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
#include "observable.h"

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
class Poll : public Observable {
private:
  bool m_started;
  bool m_stopped;
  std::map<RemoteID, std::vector<Response>> m_responses;
  Timestamp m_start_wall;
  std::chrono::time_point<std::chrono::steady_clock> m_start_mono;
  mutable std::mutex m_lock;

public:
  // notification hints
  enum {
    POLL_STARTED = 1000,
    POLL_RESPONSE_RECORDED,
    POLL_STOPPED,
    POLL_RESET,

    POLL_LAST,
  };

  static bool is_valid_hint(int hint) {
    return hint >= POLL_STARTED && hint < POLL_LAST;
  }

  Poll();

  // call this when the poll starts so that the start time is recorded
  void start();

  // record one response, which will be timestamped based on the current
  // (computed) wall clock time
  void record_response(RemoteID remote_id, Option option);

  // call this when the poll stops
  void stop();

  // reset the poll to clear out the data (so it can be restarted
  // as a new poll)
  void reset();

  // return true if the Poll has been started
  bool is_started() const;

  // return true if the Poll has been stopped
  bool is_stopped() const;

  // Get the poll's start time (as milliseconds since the epoch)
  Timestamp get_start_time() const;

  // Get just the "effective" responses, i.e., each student's
  // final response before the poll was close
  std::map<RemoteID, Option> get_final_responses() const;

  // Get all responses (ordered by timestamp) for each student
  std::map<RemoteID, std::vector<Response>> get_all_responses() const;

  // Get the number of "effective" responses
  unsigned get_num_final_responses() const { return m_responses.size(); }
};

#endif // POLL_H
