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

class DataStore;
class Base;
class Poll;
class Timer;
class Course;

// PollModel is a unified model object for PollView.
// It aggregates Base, Poll, and Timer, and unifies their notifications.
// It also has responsibility for creating and deleting the Base,
// Poll, and Timer objects, and generally managing their lifecycles.
// It uses a DataStore, but is not responsible for creating or
// deleting it.

class PollModel : public Observer, public Observable {
private:
  DataStore *m_datastore;
  Base *m_base;
  Poll *m_poll;
  Timer *m_timer;
  unsigned m_selected_course;
  std::string m_poll_data_dir;

public:
  // note that each observable object has a distinct range of hint
  // values, so the hint values for "rebroadcast" notifications from
  // Poll and Timer are used as-is

  // these are the hints that are specific to PollModel
  enum {
    POLL_MODEL_SELECTED_COURSE_CHANGED = 3000,
  };

  PollModel(DataStore *datastore);
  virtual ~PollModel();

  // FIXME: there should be some state/lifecycle functions here

  bool is_poll_running() const;
  bool can_start_poll() const;

  Course *get_current_course() const;
  void set_current_course(unsigned selected_course);

  void set_poll_data_dir(const std::string &poll_data_dir);
  std::string get_poll_data_dir() const;

  DataStore *get_datastore() const;
  Base *get_base() const;
  Poll *get_poll() const;
  Timer *get_timer() const;

  virtual void on_update(Observable *observable, int hint);
};

#endif // POLL_MODEL_H
