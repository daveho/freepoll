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

#include <sstream>
#include "course.h"

Course::Course()
  : m_term_num(0)
  , m_section(0)
  , m_year(0)
  , m_active(false)
{
}

Course::Course(const std::string &title,
               const std::string &term_display,
               int term_num,
               int section,
               int year,
               const std::string &directory,
               bool active)
  : m_title(title)
  , m_term_display(term_display)
  , m_term_num(term_num)
  , m_section(section)
  , m_year(year)
  , m_directory(directory)
  , m_active(active)
{
}

Course::~Course() {
}

std::string Course::get_display_string() const {
  std::stringstream ss;
  ss << m_title << " " << m_term_display << " " << m_year << " (Sec " << m_section << ")";
  return ss.str();
}

#define COMPARE(fieldname) \
if (fieldname < rhs.fieldname) return true; \
if (fieldname > rhs.fieldname) return false

bool Course::operator<(const Course &rhs) const {
  // order active courses before inactive ones
  if (m_active != rhs.m_active) { return m_active; }

  COMPARE(m_year);
  COMPARE(m_term_num);
  COMPARE(m_term_display);
  COMPARE(m_title);
  COMPARE(m_section);
  COMPARE(m_directory);

  return false;
}
