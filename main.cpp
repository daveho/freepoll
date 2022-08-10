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
#include <fstream>
#include <iomanip>
#include <string>
#include <memory>
#include <csignal>
#include <cstdlib>
#include <unistd.h> // for pause()
#include "poll.h"
#include "base.h"
#include "poll_runner.h"
#include "frequency.h"
#include "exception.h"
#include "argparse.h"

void handle_sigint(int) {
  // do nothing
}

int main(int argc, char **argv) {
  std::string progname = argv[0];

  argparse::Parser parse_args;
  auto help_opt = parse_args.AddFlag("help", 'h', "Show usage info");
  auto dest_opt = parse_args.AddArg<std::string>("dest", "Set output CSV filename");
  auto frequency_opt = parse_args.AddArg<std::string>("frequency", "Set the base station frequency (e.g., \"AA\", etc.)");

  parse_args.ParseArgs(argc, argv);

  if (*help_opt) {
    std::cerr << parse_args.DefaultUsageString(progname);
    return 0;
  }

  if (!dest_opt) {
    std::cerr << "No destination file specified (use --dest option)\n\n";
    std::cerr << parse_args.DefaultUsageString(progname);
    return 1;
  }
  std::string dest_filename = *dest_opt;

  Frequency freq("AA");
  if (frequency_opt) {
    freq = Frequency(*frequency_opt);
  }

  std::unique_ptr<Base> base(new Base());
  try {
    base->initialize();
  } catch (PollException &ex) {
    std::cerr << "Error: " << ex.what() << "\n";
    return 1;
  }

  signal(SIGINT, handle_sigint);

  Poll poll;
  PollRunner runner(base.get(), &poll);

  std::cout << "Starting poll (use control-C to end poll)...\n";
  runner.start_poll();

  // wait for control-C
  // Note that this code will only work on POSIX systems:
  // for Windows, it looks like an alternate mechanism is available
  // (see https://stackoverflow.com/questions/18291284)
  pause();

  std::cout << "Stopping poll...\n";
  runner.stop_poll();

  std::ofstream out(dest_filename);
  if (!out.is_open()) {
    std::cerr << "Failed to output destination file " << dest_filename << "\n";
    return 1;
  }

  // save poll results
  std::map<RemoteID, Option> poll_results = poll.get_final_responses();
  out << "RemoteID,Option\n";
  for (auto i = poll_results.begin(); i != poll_results.end(); ++i) {
    out << std::hex << std::setw(8) << i->first << "," << char('A' + int(i->second)) << "\n";
  }

  std::cout << "Wrote results to " << dest_filename << "\n";

  return 0;
}
