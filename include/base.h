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

#ifndef BASE_H
#define BASE_H

#include <vector>
#include <string>
#include <hidapi.h>
#include "message.h"
#include "frequency.h"
#include "response_callback.h"

// Encapsulation of iClicker base station

class Base {
private:
  Frequency m_desired_freq, m_current_freq;
  bool m_initialized;
  hid_device *m_dev;

  // value semantics are not allowed
  Base(const Base &);
  Base &operator=(const Base &);

public:
  enum PollType {
    ALPHA        = 0x66 + 0,
    NUMERIC      = 0x66 + 1,
    ALPHANUMERIC = 0x66 + 2,
  };

  Base();
  ~Base();

  void set_frequency(const Frequency &freq);
  void initialize();

  void set_screen(const std::string &s, unsigned line);
  void start_poll(PollType poll_type);
  void stop_poll();

  void collect_responses(volatile const bool &stop, ResponseCallback *response_callback);

private:
  void synchronous_send(const Message &msg, unsigned timeout_millis);
  void raw_send(const Message &msg);
  void receive(Message &msg, unsigned timeout_millis);
  void sleep(unsigned millis);
  void send_command_sequence(const std::vector<Message> &cmd_seq);

  void send_set_frequency();
  void send_set_protocol_version();
  void send_set_poll_type(PollType poll_type);
};

#endif // BASE_H
