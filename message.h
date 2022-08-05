#ifndef MESSAGE_H
#define MESSAGE_H

#include <vector>
#include <string>

// Class to represent message sent to or from the base station
class Message {
private:
  std::vector<unsigned char> m_bytes;

public:
  Message();
  Message(unsigned size);
  Message(std::initializer_list<unsigned char> init);
  Message(const std::string &data);
  Message(const Message &other);

  Message &operator=(const Message &rhs);

  unsigned size() const { return unsigned(m_bytes.size()); }
  bool empty() const { return m_bytes.empty(); }

  unsigned char &at(unsigned i)      { return m_bytes.at(i); }
  unsigned char at(unsigned i) const { return m_bytes.at(i); }

  unsigned char *data()             { return m_bytes.data(); }
  const unsigned char *data() const { return m_bytes.data(); }

  Message &operator+=(unsigned char byte);
  Message &operator+=(const Message &rhs);

  void clear() { m_bytes.clear(); }

  void set_data(const unsigned char *data, unsigned num_bytes);

#if 0
  bool operator==(const Message &rhs) const { return m_bytes == rhs.m_bytes; }
  bool operator!=(const Message &rhs) const { return !(m_bytes == rhs.m_bytes); }
#endif

  std::string str() const;
};

#endif // MESSAGE_H
