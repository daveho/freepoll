#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <ctime>
#include <stdexcept>
#include "datatypes.h"

namespace ts {

inline Timestamp timespec_to_timestamp(const struct timespec &tspec) {
  Timestamp ts = tspec.tv_sec * 1000;
  ts += (tspec.tv_nsec / 1000);
  return ts;
}

inline Timestamp get_timestamp(clockid_t clk_id) {
  struct timespec tspec;
  if (clock_gettime(clk_id, &tspec) != 0) {
    throw std::runtime_error("clock_gettime failed");
  }
  return timespec_to_timestamp(tspec);
}

}

#endif // TIMESTAMP_H
