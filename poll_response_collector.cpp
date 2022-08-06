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

#include "poll.h"
#include "poll_response_collector.h"

PollResponseCollector::PollResponseCollector(Poll &poll)
  : m_poll(poll) {
}

PollResponseCollector::~PollResponseCollector() {
}

void PollResponseCollector::on_response(RemoteID remote_id, Option option) {
  m_poll.record_response(remote_id, option);
}
