#include <cassert>
#include <iostream>
#include <vector>
#include <memory>
#include "exception.h"
#include "message.h"
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

namespace {

bool is_valid_freq_char(char c) {
  return c >= 'a' && c <= 'e';
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
}

// Send a message and expect a response containing the first two
// bytes of the data sent.
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
    throw PollException("timeout reading data from device");
  }
  std::cout << "received " << n << " byte(s)\n";

  msg.set_data(buf.get(), unsigned(n));
}

void Base::sleep(unsigned millis) {
  // TODO
}

void Base::send_command_sequence(const std::vector<Message> &cmd_seq) {
  for (auto i = cmd_seq.begin(); i != cmd_seq.end(); ++i) {
    raw_send(*i);
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
