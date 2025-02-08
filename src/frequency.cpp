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

#include <cctype>
#include "exception.h"
#include "frequency.h"

// note that this constructor creates a Frequency value
// that is not valid
Frequency::Frequency() {
}

// initialize a Frequency from a two-letter frequency code:
// throws an exception if the code is not valid
Frequency::Frequency(const std::string &freq_str) {
  if (freq_str.size() != 2) {
    throw PollException("Invalid frequency code: " + freq_str);
  }

  char freq1 = ::toupper(freq_str[0]);
  char freq2 = ::toupper(freq_str[1]);

  if (!is_valid_freq_char(freq1) || !is_valid_freq_char(freq2)) {
    throw PollException("Invalid frequency code: " + freq_str);
  }

  m_freq += freq1;
  m_freq += freq2;
}

Frequency::~Frequency() {
}
