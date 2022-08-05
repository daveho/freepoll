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
