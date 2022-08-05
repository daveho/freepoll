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
#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <thread>
#include <utility>
#include <set>
#include "exception.h"
#include "message.h"
#include "datatypes.h"
#include "base.h"

// This code is a derived from iclickerpoll.py:
//   https://github.com/siefkenj/iclickerpoll/blob/master/iclickerpoll.py
//
// In fact, some of the code is more or less cut-and-pasted, with
// some minimal changes to allow it to compile as C++.
//
// So, this should be considered a derived work, which is fine,
// because the original is licensed under GPL 3.0 (copyright Jason Siefken),
// and this code is also licensed under GPL 3.0.

typedef unsigned char UC;

struct RawResponse {
  RemoteID remote_id;
  Option option;
  int seq_num;

  RawResponse()
    { }
  RawResponse(RemoteID remote_id, Option option, int seq_num)
    : remote_id(remote_id), option(option), seq_num(seq_num)
    { }

  bool operator==(const RawResponse &rhs) const {
    return remote_id == rhs.remote_id
        && option == rhs.option
        && seq_num == rhs.seq_num;
  }

  bool operator!=(const RawResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator<(const RawResponse &rhs) const {
    if (remote_id < rhs.remote_id) return true;
    if (remote_id > rhs.remote_id) return false;
    if (option < rhs.option) return true;
    if (option > rhs.option) return false;
    return seq_num < rhs.seq_num;
  }
};

namespace {

bool is_valid_freq_char(char c) {
  return c >= 'a' && c <= 'e';
}

void decode_response(const unsigned char *data, std::vector<RawResponse> &responses) {
  if (data[0] == 0x02 && data[1] == 0x13) {
    // this is an alpha clicker response
#if 0
    Message as_msg;
    as_msg.set_data(data, 32);
    std::cout << "decoding response: " << as_msg.str() << "\n";
#endif
    // decode the remote ID
    RemoteID remote_id = 0;
    for (unsigned i = 0; i < 3; i++) {
      remote_id |= data[i + 3];
      remote_id <<= 8;
    }
    // LSB of remote ID is the exclusive OR of the three data bytes
    remote_id |= UC(data[3] ^ data[4] ^ data[5]);

    // decode the selected option
    // A=0x81, B=0x82, etc.
    Option option = Option(data[2] - 0x81);

    // get the sequence number
    int seq_num = data[6];

    responses.push_back({ remote_id, option, seq_num });
  }
}

void unpack_responses(const Message &data, std::vector<RawResponse> &responses) {
  std::cout << "Unpacking: " << data.str() << "\n";

  // there could be up to two responses
  if (data.size() >= 32) {
    decode_response(data.data() + 0, responses);
  }
  if (data.size() >= 64) {
    decode_response(data.data() + 32, responses);
  }
}

const unsigned VENDOR_ID = 0x1881;
const unsigned PRODUCT_ID = 0x0150;

const std::vector<Message> INIT_COMMAND_SEQUENCE_A = {
  Message("01 2a 21 41 05"),
  Message("01 12"),
  Message("01 15"),
  Message("01 16"),
};

const std::vector<Message> INIT_COMMAND_SEQUENCE_B = {
  Message("01 29 a1 8f 96 8d 99 97 8f"),
  Message("01 17 04"),
  Message("01 17 03"),
  Message("01 16"),
};

const std::vector<Message> START_POLL_COMMAND_SEQUENCE_A = {
  Message("01 17 03"),
  Message("01 17 05"),
};

const std::vector<Message> START_POLL_COMMAND_SEQUENCE_B = {
  Message("01 11"),
};

const std::vector<Message> STOP_POLL_COMMAND_SEQUENCE_A = {
  Message("01 12"),
  Message("01 16"),
  Message("01 17 01"),
  Message("01 17 03"),
  Message("01 17 04"),
};

#define FREEPOLL_VERSION "0.00"

const char *greeting_text[] = {
  "    FreePoll    ",
  "     v " FREEPOLL_VERSION "     ",
};

} // end anonymous namespace

Base::Base()
  : m_freq1('a')
  , m_freq2('a')
  , m_initialized(false)
  , m_dev(nullptr) {
}

Base::~Base() {
  if (m_dev != nullptr) {
    hid_close(m_dev);
  }

  if (m_initialized) {
    hid_exit();
  }
}

void Base::set_frequency(char freq1, char freq2) {
  if (!is_valid_freq_char(freq1) || !is_valid_freq_char(freq2)) {
    throw PollException("invalid frequency specification");
  }
  m_freq1 = freq1;
  m_freq2 = freq2;
}

void Base::initialize() {
  assert(!m_initialized);
  assert(m_dev == nullptr);

  if (hid_init() != 0) {
    throw PollException("hid_init() failed");
  }

  m_initialized = true;

  m_dev = hid_open(VENDOR_ID, PRODUCT_ID, nullptr);
  if (m_dev == nullptr) {
    throw PollException("could not open base station HID device");
  }

  send_set_frequency();
  send_command_sequence(INIT_COMMAND_SEQUENCE_A);
  send_set_protocol_version();
  send_command_sequence(INIT_COMMAND_SEQUENCE_B);

  // display greeting text
  for (int i = 0; i < 2; i++) {
    sleep(100);
    set_screen(greeting_text[i], i);
  }
  sleep(100);
}

void Base::set_screen(const std::string &s, unsigned line) {
  assert(line < 2);
  assert(s.size() <= 16);
  Message cmd = {0x01, (line == 0) ? UC(0x13) : UC(0x14)};
  for (unsigned i = 0; i < s.size(); i++) { // append characters
    cmd += UC(s[i]);
  }
  for (unsigned i = 0; i < 16 - s.size(); i++) { // pad to length 16
    cmd += UC(' ');
  }
  raw_send(cmd);
}

void Base::start_poll(PollType poll_type) {
  send_command_sequence(START_POLL_COMMAND_SEQUENCE_A);
  send_set_poll_type(poll_type);
  send_command_sequence(START_POLL_COMMAND_SEQUENCE_B);

  // TODO: clear the display in preparation for summarizing responses?
}

void Base::stop_poll() {
  send_command_sequence(STOP_POLL_COMMAND_SEQUENCE_A);
}

// Poll for received responses until stop is true.
// Obviously, this should be run in its own thread
// (because stop will be set to true asynchronously.)
void Base::collect_responses(volatile const bool &stop, ResponseCallback *response_callback) {
  // For whatever reason, the base station sends repeats of
  // previously-sent responses. Only send new unique responses
  // to the callback.

  std::set<RawResponse> received;

  while (!stop) {
    Message data;
    receive(data, 100);
    if (!data.empty()) {
      std::vector<RawResponse> responses;
      unpack_responses(data, responses);
      for (auto i = responses.begin(); i != responses.end(); ++i) {
        const RawResponse &response = *i;

        // check whether this response is new
        if (received.count(response) == 0) {
          // response is new: deliver it to the callback, and
          // note that we've received it
          response_callback->on_response(response.remote_id, response.option);
          received.insert(response);
        }
      }
    }

    // TODO: update display to summarize responses?
  }
}

// Send a message and expect a response containing the first two
// bytes of the data sent. This only seems to be necessary when
// setting the frequency.
void Base::synchronous_send(const Message &msg, unsigned timeout_millis) {
  assert(msg.size() >= 2);

  // send the message
  raw_send(msg);

  // receive the response and check whether it contains the expected data
  Message response;
  receive(response, timeout_millis);

  // expected response will be the first two bytes of
  // the message being sent, followed by 0xAA
  if (   response.size() < 3
      || response.at(0) != msg.at(0)
      || response.at(1) != msg.at(1)
      || response.at(2) != 0xAA) {
    throw PollException("did not receive expected response data (received " + response.str() + ")");
  }
}

void Base::raw_send(const Message &msg) {
  assert(m_dev != nullptr);

  // From the hid_write documentation:
  //
  //  "The first byte of @p data[] must contain the Report ID. For
  //  devices which only support a single report, this must be set
  //  to 0x0."

  Message msg_to_send;
  msg_to_send += UC(0);
  msg_to_send += msg;

  assert(msg_to_send.size() == msg.size() + 1);

  // TODO: should think about how to make this nonblocking?
  int n = hid_write(m_dev, msg_to_send.data(), msg_to_send.size());

  if (n != int(msg_to_send.size())) {
    throw PollException("failed to write all data");
  }
}

void Base::receive(Message &msg, unsigned timeout_millis) {
  assert(m_dev != nullptr);

  // iclickerpoll.py always attempts to read 64 bytes,
  // presumably we will get just one packet?
  // (The hid_read_timeout() documentation says it will read
  // "an Input report".)
  std::unique_ptr<UC> buf(new UC[64]);

  int n = hid_read_timeout(m_dev, buf.get(), 64, int(timeout_millis));
  if (n < 0) {
    throw PollException("failed to read data from device");
  }
  if (n == 0) {
    // timeout: this is signified by clearing the message data
    msg.clear();
    return;
  }
  //std::cout << "received " << n << " byte(s)\n";

  msg.set_data(buf.get(), unsigned(n));
}

void Base::sleep(unsigned millis) {
  std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}

void Base::send_command_sequence(const std::vector<Message> &cmd_seq) {
  for (auto i = cmd_seq.begin(); i != cmd_seq.end(); ++i) {
    raw_send(*i);
    Message response;
    receive(response, 100);
  }
}

void Base::send_set_frequency() {
  // set the base station frequency

  Message msg1 = {0x01, 0x10, UC(0x21 + (m_freq1 - 'a')), UC(0x41 + (m_freq2 - 'a'))};
  sleep(200);
  synchronous_send(msg1, 100);

  Message msg2 = {0x01, 0x16};
  sleep(200);
  synchronous_send(msg2, 100);

  sleep(200);
}

void Base::send_set_protocol_version() {
  Message msg = {0x01, 0x2d};
  raw_send(msg);
  sleep(200);
}

void Base::send_set_poll_type(PollType poll_type) {
  assert(   poll_type == ALPHA
         || poll_type == NUMERIC
         || poll_type == ALPHANUMERIC);
  Message cmd = {0x01, 0x19, UC(poll_type), 0x0a, 0x01};
  raw_send(cmd);
  sleep(200);
}
