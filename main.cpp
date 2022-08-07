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
#include <string>
#include <sstream>
#include <memory>
#include <chrono>
#include <thread>
#include "poll.h"
#include "base.h"
#include "poll_runner.h"

#if 0
// this is just for testing
class PrintResponses : public PollResponseCollector {
private:
  PrintResponses(const PrintResponses &);
  PrintResponses &operator=(const PrintResponses &);

public:
  PrintResponses(Poll &poll);
  virtual ~PrintResponses();

  virtual void on_response(RemoteID remote_id, Option option);
};

PrintResponses::PrintResponses(Poll &poll)
  : PollResponseCollector(poll) {
}

PrintResponses::~PrintResponses() {
}

void PrintResponses::on_response(RemoteID remote_id, Option option) {
  PollResponseCollector::on_response(remote_id, option);

  std::stringstream ss;
  ss << std::hex;
  ss << "RemoteID:" << remote_id << ", option=" << char('A' + int(option));
  std::cout << ss.str() << "\n";
}

namespace {

void watch_poll(Base *base, ResponseCallback *response_callback, volatile const bool *stop) {
  base->start_poll(Base::ALPHA);
  base->collect_responses(*stop, response_callback);
  base->stop_poll();
}

std::string option_to_str(Option option) {
  switch (option) {
  case Option::A: return "A";
  case Option::B: return "B";
  case Option::C: return "C";
  case Option::D: return "D";
  case Option::E: return "E";
  default:        return "?";
  }
}

}
#endif

int main() {
  std::cout << "Initalizing base...\n";
  std::unique_ptr<Base> base(new Base());
  base->initialize();

  std::cout << "initialized base?\n";

  std::cout << "Test program: enter \"start\" to start a poll,\n"
            << "and enter \"stop\" to end the poll and quit\n"
            << "the program\n";

  Poll poll;
  //PrintResponses print_responses(poll);
  //std::thread *poll_watcher = nullptr;
  //bool stop = false; // will control when poll stops
  PollRunner runner(base.get(), &poll);

  bool done = false;
  std::string line;
  while (!done && std::getline(std::cin, line)) {
    if (line == "start" && !runner.is_poll_started()) {
      std::cout << "starting poll...\n";
      //poll_watcher = new std::thread( watch_poll, base.get(), &print_responses, &stop);
      runner.start_poll();
    } else if (line == "stop") {
      if (runner.is_poll_started()) {
        //stop = true;
        //poll_watcher->join();
        //delete poll_watcher;
        runner.stop_poll();
        std::cout << "poll finished\n";
      }
      done = true;
    }
  }

  // print poll results
  std::map<RemoteID, Option> poll_results = poll.get_final_responses();
  std::cout << "RemoteID,Option\n";
  for (auto i = poll_results.begin(); i != poll_results.end(); ++i) {
    std::cout << std::hex << std::setw(8) << i->first << "," << char('A' + int(i->second)) << "\n";
  }

  return 0;
}
