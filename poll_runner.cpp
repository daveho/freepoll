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
#include "base.h"
#include "poll.h"
#include "poll_response_collector.h"
#include "poll_runner.h"

PollRunner::PollRunner(Base *base, Poll *poll)
  : m_base(base)
  , m_poll(poll)
  , m_response_callback(new PollResponseCollector(*poll))
  , m_stop(false)
  , m_thread(nullptr) {
}

PollRunner::~PollRunner() {
  delete m_response_callback;
}

void PollRunner::start_poll() {
  assert(m_thread == nullptr);
  m_poll->start();
  m_thread = new std::thread(run_poll, this);
}

void PollRunner::stop_poll() {
  assert(m_thread != nullptr);
  m_stop = true;
  m_thread->join();
  m_poll->stop();
}

void PollRunner::run_poll(PollRunner *runner) {
  runner->m_base->start_poll(Base::ALPHA);
  runner->m_base->collect_responses(runner->m_stop, runner->m_response_callback);
  runner->m_base->stop_poll();
}
