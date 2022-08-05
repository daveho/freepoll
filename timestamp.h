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

#include <chrono>
#include <stdexcept>
#include "datatypes.h"

namespace ts {

// Get milliseconds since epoch. Note that until C++20, the
// epoch isn't specified. We just assume it is the Unix epoch,
// but this assumption might need to be revisited on non-Linux
// platforms.
inline Timestamp millis_since_epoch() {
  const auto now    = std::chrono::system_clock::now();
  const auto epoch  = now.time_since_epoch();
  const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
  return Timestamp(millis.count());
}

}

#endif // TIMESTAMP_H
