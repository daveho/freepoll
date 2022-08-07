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

#ifndef DATASTORE_H
#define DATASTORE_H

#include <vector>
#include <string>
class Course;

// The DataStore is the central location where all course and
// clicker response data is stored.

class DataStore {
private:
  std::string m_base_dir;
  std::vector<Course *> m_courses;

  // value semantics prohibited
  DataStore(const DataStore &);
  DataStore &operator=(const DataStore &);

public:
  DataStore(const std::string &base_dir);
  ~DataStore();

  void locate_courses();

  unsigned get_num_courses() const { return unsigned(m_courses.size()); }

  const std::vector<Course *> &get_courses() const { return m_courses; }

  // TODO: functions for creating and saving screenshot and clicker data

private:
  void load_course(const std::string &course_dir, const std::string &courseinfo_filename);
};

#endif // DATASTORE_H
