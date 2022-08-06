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

#ifndef POLL_RESPONSE_COLLECTOR_H
#define POLL_RESPONSE_COLLECTOR_H

#include "response_callback.h"
class Poll;

// ResponseCallback implementation which records the responses
// in a Poll object
class PollResponseCollector : public ResponseCallback {
private:
  Poll &m_poll;

  // value semantics are prohibited
  PollResponseCollector(const PollResponseCollector &);
  PollResponseCollector &operator=(const PollResponseCollector &);

public:
  PollResponseCollector(Poll &poll);
  virtual ~PollResponseCollector();

  virtual void on_response(RemoteID remote_id, Option option);
};

#endif // POLL_RESPONSE_COLLECTOR_H
