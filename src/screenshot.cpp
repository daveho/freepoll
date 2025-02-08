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
#include <thread>
#include <filesystem>
#include <cassert>
#include <cstdlib>
#include "screenshot.h"

namespace Screenshot {

namespace fs = std::filesystem;

struct ScreenshotUtil {
  std::string exename;
  std::vector<std::string> invocation;
};

const std::vector<ScreenshotUtil> SCREENSHOT_UTILS = {
  { "/usr/bin/scrot",  {"%p", "%f"} },
  { "/usr/bin/import", {"%p", "-window", "root", "%f"} },
};

struct ScreenshotUtilLocator {
  const ScreenshotUtil *util;

  ScreenshotUtilLocator();
  bool found_screenshot_util() const { return util != nullptr; }
  std::string format_command(const std::string &output_filename);
};

ScreenshotUtilLocator::ScreenshotUtilLocator()
  : util(nullptr) {
  for (const ScreenshotUtil &sutil : SCREENSHOT_UTILS) {
    fs::path exepath(sutil.exename);
    auto status = fs::status(exepath);
    if (status.type() == fs::file_type::regular) {
      // we'll assume this file is an executable
      util = &sutil;
      return;
    }
  }
  std::cerr << "Failed to locate a screenshot utility\n";
}

std::string ScreenshotUtilLocator::format_command(const std::string &output_filename) {
  assert(found_screenshot_util());

  std::string cmd;
  for (auto i = util->invocation.begin(); i != util->invocation.end(); ++i) {
    std::string token = *i;

    if (!cmd.empty()) cmd += " ";

    if (token == "%p") {
      cmd += util->exename;
    } else if (token == "%f") {
      cmd += output_filename;
    } else {
      cmd += token;
    }
  }

  return cmd;
}

ScreenshotUtilLocator s_locator;

void take_snapshot(const std::string &output_filename) {
  std::string cmd = s_locator.format_command(output_filename);
  ::system(cmd.c_str());
  std::cout  << "Saved screenshot to " << output_filename << "\n";
}

void take_screenshot(const std::string &output_filename) {
  if (!s_locator.found_screenshot_util()) {
    return;
  }
  std::thread snapshot_thread(take_snapshot, output_filename);
  snapshot_thread.detach();
}

}
