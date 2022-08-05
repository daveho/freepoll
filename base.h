#ifndef BASE_H
#define BASE_H

#include <vector>
#include <string>
#include <hidapi.h>
#include "response_callback.h"

// Encapsulation of iClicker base station

class Message;

class Base {
private:
  char m_freq1, m_freq2;
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

  void set_frequency(char freq1, char freq2);
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
