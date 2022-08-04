#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>

class PollException : public std::runtime_error {
public:
  PollException(const std::string &msg);
  ~PollException();
};

#endif // EXCEPTION_H
