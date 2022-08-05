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
