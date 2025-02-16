// Copyright (c) 2022-2025, David Hovemeyer <david.hovemeyer@gmail.com>

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
#include <chrono>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "timer.h"

Timer::Timer()
  : m_timer_thread(nullptr)
  , m_num_seconds(0)
  , m_stop(false) {
}

Timer::~Timer() {
}

void Timer::start() {
  assert(m_timer_thread == nullptr);

  {
    std::lock_guard<std::mutex> guard(m_lock);
    m_timer_thread = new std::thread(Timer::run, this);
  }

  notify_observers(TIMER_STARTED, false);
}

void Timer::stop() {
  assert(m_timer_thread != nullptr);
  m_stop = true;
  m_timer_thread->join();

  {
    std::lock_guard<std::mutex> guard(m_lock);
    delete m_timer_thread;
    m_num_seconds = 0;
    m_timer_thread = nullptr;
    m_stop = false;
  }

  notify_observers(TIMER_STOPPED, false);
}

bool Timer::is_running() const {
  std::lock_guard<std::mutex> guard(m_lock);
  return m_timer_thread != nullptr;
}

unsigned Timer::get_num_seconds() const {
  std::lock_guard<std::mutex> guard(m_lock);
  return m_num_seconds;
}

std::string Timer::get_display_time() const {
  unsigned num_seconds = get_num_seconds();
  unsigned min = num_seconds / 60;
  unsigned sec = num_seconds % 60;
  std::stringstream ss;
  ss << min << ":" << std::setfill('0') << std::setw(2) << sec;
  return ss.str();
}

void Timer::run(Timer *timer) {
  auto start = std::chrono::steady_clock::now();

  while (!timer->m_stop) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    auto current = std::chrono::steady_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(current - start);

    if (elapsed.count() > timer->m_num_seconds) {
      {
        std::lock_guard<std::mutex> guard(timer->m_lock);
        timer->m_num_seconds = elapsed.count();
      }
      std::cout << "timer: notify " << timer->get_num_observers() << " observer(s)\n";
      timer->notify_observers(TIMER_NUM_SECONDS_UPDATED, true);
    }
  }
}
