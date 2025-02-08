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

#ifndef RESPONSE_H
#define RESPONSE_H

#include "datatypes.h"

// A Response is the result of a student clicking on a multiple choice
// option (A-E) in a poll.
class Response {
private:
  RemoteID m_remote_id;
  Option m_option;
  Timestamp m_timestamp;

public:
  Response(RemoteID remote_id, Option option, Timestamp timestamp)
    : m_remote_id(remote_id), m_option(option), m_timestamp(timestamp) { }

  RemoteID get_remote_id() const  { return m_remote_id; }
  Option get_option() const       { return m_option; }
  Timestamp get_timestamp() const { return m_timestamp; }
};

#endif // RESPONSE_H
