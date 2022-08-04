#include <sstream>
#include "message.h"

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
