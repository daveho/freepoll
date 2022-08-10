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

#ifndef FREQUENCY_H
#define FREQUENCY_H

#include <string>

class Frequency {
private:
  std::string m_freq;

public:
  Frequency();
  Frequency(const std::string &freq_str);
  ~Frequency();

  bool is_valid() const { return is_valid_freq_char(m_freq[0]) && is_valid_freq_char(m_freq[1]); }

  std::string get_freq() const { return m_freq; }

  char get_freq1() const { return m_freq[0]; }
  char get_freq2() const { return m_freq[1]; }

  std::string str() const { return is_valid() ? m_freq : "--"; }

  bool operator==(const Frequency &rhs) const { return m_freq == rhs.m_freq; }
  bool operator!=(const Frequency &rhs) const { return !(*this == rhs); }

  // check whether give character is a valid frequency code
  static bool is_valid_freq_char(char c) {
    return (c >= 'A' && c <= 'D');
  }
};

#endif
