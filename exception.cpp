#include "exception.h"

PollException::PollException(const std::string &msg)
  : std::runtime_error(msg) {
}

PollException::~PollException() {
}
