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

#ifndef TIMER_H
#define TIMER_H

#include <thread>
#include <mutex>
#include "observable.h"

class Timer : public Observable {
private:
  std::thread *m_timer_thread;
  volatile unsigned m_num_seconds;
  volatile bool m_stop;
  mutable std::mutex m_lock;

  // value semantics are prohibited
  Timer(const Timer &);
  Timer &operator=(const Timer &);

public:
  // notification hints
  enum {
    TIMER_STARTED = 2000,
    TIMER_NUM_SECONDS_UPDATED,
    TIMER_STOPPED,

    TIMER_LAST,
  };

  static bool is_valid_hint(int hint) {
    return hint >= TIMER_STARTED && hint < TIMER_LAST;
  }

  Timer();
  virtual ~Timer();

  void start();
  void stop();

  bool is_running() const;

  unsigned get_num_seconds() const;

private:
  static void run(Timer *timer);
};

#endif // TIMER_H
