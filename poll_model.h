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

#ifndef POLL_MODEL_H
#define POLL_MODEL_H

#include "observer.h"
#include "observable.h"

class Base;
class Poll;
class Timer;

// PollModel is a unified model object for PollView.
// It aggregates Base, Poll, and Timer, and unifies their notifications.
// It also has responsibility for creating and deleting the Base,
// Poll, and Timer objects, and generally managing their lifecycles.

class PollModel : public Observer, public Observable {
private:
  Base *m_base;
  Poll *m_poll;
  Timer *m_timer;

public:
  // note that the notification hints sent by this object
  // just use the hint values from Poll and Timer,
  // which are distinct

  PollModel();
  virtual ~PollModel();

  // FIXME: there should be some state/lifecycle functions here

  bool is_poll_running();
  bool can_start_poll();

  Base *get_base();
  Poll *get_poll();
  Timer *get_timer();

  virtual void on_update(Observable *observable, int hint);
};

#endif // POLL_MODEL_H
