#include <vector>
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

void Base::send(const Message &msg) {
  // TODO
}

void Base::sleep(double sec) {
  // TODO
}

void Base::send_command_sequence(const std::vector<Message> &cmd_seq) {
  for (auto i = cmd_seq.begin(); i != cmd_seq.end(); ++i) {
    send(*i);
  }
}

void Base::send_set_frequency() {
  // set the base station frequency

  Message msg1 = {0x01, 0x10, UC(0x21 + (m_freq1 - 'a')), UC(0x41 + (m_freq2 - 'a'))};
  sleep(0.2);
  send(msg1);

  Message msg2 = {0x01, 0x16};
  sleep(0.2);
  send(msg2);

  sleep(0.2);
}

void Base::send_set_protocol_version() {
  Message msg = {0x01, 0x2d};
  send(msg);
  sleep(0.2);
}
