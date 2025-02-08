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
#include <sstream>
#include "message.h"

Message::Message() {
}

Message::Message(unsigned size)
  : m_bytes(size) {
}

Message::Message(std::initializer_list<unsigned char> init)
  : m_bytes(init) {
}

Message::Message(const std::string &data) {
  std::stringstream ss(data);
  unsigned val;
  ss >> std::hex;
  while (ss >> val) {
    m_bytes.push_back(static_cast<unsigned char>(val));
  }
}

Message::Message(const Message &other)
  : m_bytes(other.m_bytes) {
}

Message &Message::operator=(const Message &rhs) {
  if (this != &rhs) {
    m_bytes = rhs.m_bytes;
  }
  return *this;
}

Message &Message::operator+=(unsigned char byte) {
  m_bytes.push_back(byte);
  return *this;
}

Message &Message::operator+=(const Message &rhs) {
  assert(this != &rhs);
  m_bytes.insert(m_bytes.begin(), rhs.m_bytes.begin(), rhs.m_bytes.end());
  return *this;
}

void Message::set_data(const unsigned char *data, unsigned num_bytes) {
  m_bytes.clear();
  m_bytes.insert(m_bytes.begin(), data, data + num_bytes);
}

std::string Message::str() const {
  std::stringstream ss;

  ss << std::hex;

  ss << "{";
  for (unsigned i = 0; i < m_bytes.size(); i++) {
    if (i > 0) {
      ss << ", ";
    }
    ss << "0x";
    ss << unsigned(m_bytes[i]);
  }
  ss << "}";

  return ss.str();
}
