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

#ifndef COURSE_H
#define COURSE_H

#include <string>
#include "frequency.h"

class Course {
private:
  std::string m_title;         // e.g. "Computer Systems Fundamentals"
  std::string m_term_display;  // e.g. "Fall"
  int m_term_num;              // e.g. 6 (because fall is chronologically the last term of the calendar year)
  int m_section;               // e.g. 1
  int m_year;                  // e.g. 2022
  std::string m_directory;     // e.g. /home/daveho/FreePoll/CSF-Fall2022-Sec01
  Frequency m_frequency;       // e.g. AA
  bool m_active;               // true if this is an active course (and should be shown in the GUI)

public:
  Course();
  Course(const std::string &title,
         const std::string &term_display,
         int term_num,
         int section,
         int year,
         const std::string &directory,
         bool active);
  ~Course();

  std::string get_title() const        { return m_title; }
  std::string get_term_display() const { return m_term_display; }
  int get_term_num() const             { return m_term_num; }
  int get_section() const              { return m_section; }
  int get_year() const                 { return m_year; }
  std::string get_directory() const    { return m_directory; }
  Frequency get_frequency() const      { return m_frequency; }
  bool is_active() const               { return m_active; }

  std::string get_display_string() const;

  void set_title(const std::string &title)               { m_title = title; }
  void set_term_display(const std::string &term_display) { m_term_display = term_display; }
  void set_term_num(int term_num)                        { m_term_num = term_num; }
  void set_section(int section)                          { m_section = section; }
  void set_year(int year)                                { m_year = year; }
  void set_directory(const std::string &directory)       { m_directory = directory; }
  void set_frequency(const Frequency &frequency)         { m_frequency = frequency; }
  void set_active(bool active)                           { m_active = active; }

  bool operator<(const Course &rhs) const;
};

#endif // COURSE_H
