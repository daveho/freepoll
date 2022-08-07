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

#include <iostream>
#include <filesystem>
#include <memory>
#include <cctype>
#include <algorithm>
#include "course.h"
#include "rapidcsv.h"
#include "datastore.h"

namespace fs = std::filesystem;

DataStore::DataStore(const std::string &base_dir)
  : m_base_dir(base_dir) {
}

DataStore::~DataStore() {
  for (auto i = m_courses.begin(); i != m_courses.end(); ++i) {
    delete *i;
  }
}

void DataStore::locate_courses() {
  fs::path base_dir_path(m_base_dir);

  auto status = fs::status(base_dir_path);
  if (status.type() != fs::file_type::directory) {
    std::cerr << "path " << m_base_dir << " is not a directory\n";
    return;
  }

  for (const auto &entry : fs::directory_iterator(base_dir_path)) {
    auto path = entry.path();
    auto courseinfo_path = path / "courseinfo.csv";
    auto courseinfo_status = fs::status(courseinfo_path);

    if (courseinfo_status.type() == fs::file_type::regular) {
      try {
        load_course(path.string(), courseinfo_path.string());
      } catch (std::exception &ex) {
        std::cerr << "Error reading " << courseinfo_path.string() << ": " << ex.what() << "\n";
      }
    }
  }
}

void DataStore::load_course(const std::string &course_dir, const std::string &courseinfo_filename) {
  std::ifstream courseinfo_in(courseinfo_filename);
  if (!courseinfo_in.is_open()) {
    return;
  }

  rapidcsv::Document doc(courseinfo_in, rapidcsv::LabelParams(-1, -1));

  std::unique_ptr<Course> course(new Course());
  course->set_directory(course_dir);

  for (size_t i = 0; i < doc.GetRowCount(); ++i) {
    std::vector<std::string> row = doc.GetRow<std::string>(i);

    if (row.size() == 2) {
      std::string key = row[0];
      std::string value = row[1];

      if (key == "title") {
        course->set_title(value);
      } else if (key == "term_display") {
        course->set_term_display(value);
      } else if (key == "term_num") {
        course->set_term_num(std::stoi(value));
      } else if (key == "section") {
        course->set_section(std::stoi(value));
      } else if (key == "year") {
        course->set_year(std::stoi(value));
      } else if (key == "active") {
        std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c){ return ::tolower(c); });
        course->set_active(value == "true");
      } else {
        std::cerr << "Unknown key " << key << " in " << courseinfo_filename << "\n";
      }
    }
  }

  m_courses.push_back(course.release());
}
