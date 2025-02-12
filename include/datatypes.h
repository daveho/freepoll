// Copyright (c) 2022-2025, David Hovemeyer <david.hovemeyer@gmail.com>

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

#ifndef DATATYPES_H
#define DATATYPES_H

#include <cstdint>

// remote ID
typedef uint32_t RemoteID;

// multiple choice option
enum class Option {
  A, B, C, D, E,
};

// timestamp: milliseconds since the Unix epoch
typedef uint64_t Timestamp;

// Version string
#define FREEPOLL_VERSION "0.06"

#endif // DATATYPES_H
