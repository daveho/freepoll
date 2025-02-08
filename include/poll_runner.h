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

#ifndef POLL_RUNNER_H
#define POLL_RUNNER_H

#include <thread>
class Base;
class Poll;
class ResponseCallback;

// A PollRunner object takes care of executing a thread
// in which to run a poll and collect the responses

class PollRunner {
private:
  Base *m_base;
  Poll *m_poll;
  ResponseCallback *m_response_callback;
  volatile bool m_stop;
  std::thread *m_thread;

public:
  PollRunner(Base *base, Poll *poll);
  ~PollRunner();

  void start_poll();
  void stop_poll();

  bool is_poll_started() const { return m_thread != nullptr; }

private:
  static void run_poll(PollRunner *runner);
};

#endif // POLL_RUNNER_H
