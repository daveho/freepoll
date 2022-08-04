#ifndef MESSAGE_H
#define MESSAGE_H

#include <vector>
#include <string>

// Class to represent message sent to or from the base station
class Message {
private:
  std::vector<unsigned char> m_bytes;

public:
  Message(unsigned size);
  Message(std::initializer_list<unsigned char> init);
  Message(const std::string &data);
  Message(const Message &other);

  Message &operator=(const Message &rhs);

  unsigned char *data()             { return m_bytes.data(); }
  const unsigned char *data() const { return m_bytes.data(); }
};

#endif // MESSAGE_H
