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
#include <iomanip>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <memory>
#include <cctype>
#include <algorithm>
#include <ctime>
#include <chrono>
#include "course.h"
#include "poll.h"
#include "rapidcsv.h"
#include "exception.h"
#include "datatypes.h"
#include "screenshot.h"
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

  // sort courses (this will put them in a reasonable order in the selection list)
  std::sort(m_courses.begin(), m_courses.end(),
            [](Course *lhs, Course *rhs) { return *lhs < *rhs; });
}

std::string DataStore::create_poll_data_dir(Course *course) {
  // Get date (year, month, day) in local time
  std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  time_t tt = std::chrono::system_clock::to_time_t(now);
  tm local_tm;
  localtime_r(&tt, &local_tm);

  int y = local_tm.tm_year + 1900;
  int m = local_tm.tm_mon;
  int d = local_tm.tm_mday;

  // Search the course directory for all directories sharing the
  // YYYY-MM-DD- prefix for the current day.  (I.e., polls previously
  // recorded on the same day.)  Find the one that has the highest
  // number appended to it.

  int highest_poll_num = -1;

  std::stringstream ss;
  ss << std::setw(4) << std::setfill('0') << y << "-"
     << std::setw(2) << std::setfill('0') << m << "-"
     << std::setw(2) << std::setfill('0') << d;
  std::string dir_pfx = ss.str() + "-";

  fs::path course_dir_path(course->get_directory());

  for (const auto &entry : fs::directory_iterator(course_dir_path)) {
    auto path = entry.path();
    std::string prev_poll_dir = path.filename().string();
    //std::cout << "check " << prev_poll_dir << " against " << dir_pfx << "\n";
    if (prev_poll_dir.rfind(dir_pfx, 0) == 0) {
      //std::cout << "found previous poll directory " << prev_poll_dir << "?\n";
      std::string rest = prev_poll_dir.substr(dir_pfx.size());

      try {
        int num = std::stoi(rest);
        if (num > highest_poll_num) {
          highest_poll_num = num;
        }
      } catch (std::exception &ex) {
        // rest of the directory name wasn't numeric?
      }
    }
  }

  // Choose a poll number for this poll, and turn it into a path.
  int poll_num = (highest_poll_num == -1) ? 1 : highest_poll_num + 1;
  std::stringstream ss2;
  ss2 << dir_pfx << std::setw(3) << std::setfill('0') << poll_num;
  fs::path poll_dir = course_dir_path / ss2.str();

  if (!fs::create_directory(poll_dir)) {
    throw PollException("Could not create poll directory " + poll_dir.string());
  }

  return poll_dir.string();
}

void DataStore::take_screenshot(const std::string &poll_data_dir) {
  fs::path poll_dir(poll_data_dir);

  fs::path screenshot_path = poll_dir / "screenshot.png";
  Screenshot::take_screenshot(screenshot_path.string());
}

void DataStore::write_poll_results(const std::string &poll_data_dir, const Poll *poll) {
  // Write CSV files. Note that because the data doesn't contain any
  // arbitrary strings or special characters, writing using ofstreams
  // should be fine.

  fs::path poll_dir(poll_data_dir);

  // Write responses.csv
  {
    fs::path responses_csv_path = poll_dir / "responses.csv";
    std::ofstream out(responses_csv_path.string());
    if (!out.is_open()) {
      throw PollException("Could not open " + responses_csv_path.string());
    }

    out << "RemoteID,Option\n";

    std::map<RemoteID, Option> final_responses = poll->get_final_responses();
    for (auto i = final_responses.begin(); i != final_responses.end(); ++i) {
      out << std::hex << i->first << "," << char('A' + int(i->second)) << "\n";
    }

    //std::cout << "wrote " << responses_csv_path.string() << "\n";
  }

  // Write all_responses.csv
  {
    fs::path all_responses_csv_path = poll_dir / "all_responses.csv";
    std::ofstream out(all_responses_csv_path.string());
    if (!out.is_open()) {
      throw PollException("Could not open " + all_responses_csv_path.string());
    }

    out << "RemoteID,Timestamp,Option\n";

    std::map<RemoteID, std::vector<Response>> all_responses = poll->get_all_responses();
    for (auto i = all_responses.begin(); i != all_responses.end(); ++i) {
      RemoteID remote_id = i->first;
      const std::vector<Response> &resp = i->second;

      for (auto j = resp.begin(); j != resp.end(); ++j) {
        out << std::hex << remote_id << "," << std::dec << j->get_timestamp() << ","
            << char('A' + int(j->get_option())) << "\n";
      }
    }

    //std::cout << "wrote " << all_responses_csv_path.string() << "\n";
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
      } else if (key == "frequency") {
        if (value.size() != 2 || !is_valid_freq_char(value[0]) || !is_valid_freq_char(value[1])) {
          throw PollException("Invalid frequency code " + value);
        }
        course->set_frequency(value);
      } else {
        std::cerr << "Unknown key " << key << " in " << courseinfo_filename << "\n";
      }
    }
  }

  m_courses.push_back(course.release());
}
