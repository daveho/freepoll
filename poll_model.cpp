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
#include "datastore.h"
#include "base.h"
#include "poll.h"
#include "timer.h"
#include "poll_model.h"

PollModel::PollModel(DataStore *datastore)
  : m_datastore(datastore)
  , m_base(new Base())
  , m_poll(new Poll())
  , m_timer(new Timer())
  , m_selected_course(0) {
  m_poll->add_observer(this);
  m_timer->add_observer(this);
}

PollModel::~PollModel() {
  m_poll->remove_observer(this);
  m_timer->remove_observer(this);
  delete m_poll;
  delete m_timer;

  delete m_base;
}

bool PollModel::is_poll_running() const {
  return m_poll->is_started() && !m_poll->is_stopped();
}

bool PollModel::can_start_poll() const {
  // TODO: could check whether the base station is connected
  return !is_poll_running();
}

Course *PollModel::get_current_course() const {
  return m_datastore->get_courses().at(m_selected_course);
}

void PollModel::set_current_course(unsigned selected_course) {
  assert(selected_course < m_datastore->get_courses().size());
  m_selected_course = selected_course;
  notify_observers(POLL_MODEL_SELECTED_COURSE_CHANGED);
}

void PollModel::set_poll_data_dir(const std::string &poll_data_dir) {
  m_poll_data_dir = poll_data_dir;
}

std::string PollModel::get_poll_data_dir() const {
  return m_poll_data_dir;
}

DataStore *PollModel::get_datastore() const {
  return m_datastore;
}

Base *PollModel::get_base() const {
  return m_base;
}

Poll *PollModel::get_poll() const {
  return m_poll;
}

Timer *PollModel::get_timer() const {
  return m_timer;
}

void PollModel::on_update(Observable *observable, int hint) {
  // Poll and Timer have non-overlapping hint values,
  // so no processing is necessary.
  notify_observers(hint);
}
