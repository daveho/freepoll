#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <chrono>
#include <thread>
#include "response_callback.h"
#include "base.h"

// this is just for testing
class PrintResponses : public ResponseCallback {
private:
  PrintResponses(const PrintResponses &);
  PrintResponses &operator=(const PrintResponses &);

public:
  PrintResponses();
  virtual ~PrintResponses();

  virtual void on_response(RemoteID remote_id, Option option);
};

PrintResponses::PrintResponses() {
}

PrintResponses::~PrintResponses() {
}

void PrintResponses::on_response(RemoteID remote_id, Option option) {
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

}

int main() {
  std::unique_ptr<Base> base(new Base());
  base->initialize();

  std::cout << "initialized base?\n";

  base->set_screen(" ALL YOUR BASE  ", 0);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  base->set_screen("ARE BELONG TO US", 1);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  PrintResponses print_responses;
  std::thread *poll_watcher = nullptr;
  bool stop = false; // will control when poll stops

  bool done = false;
  std::string line;
  while (!done && std::getline(std::cin, line)) {
    if (line == "start" && poll_watcher == nullptr) {
      std::cout << "starting poll...\n";
      poll_watcher = new std::thread( watch_poll, base.get(), &print_responses, &stop);
    } else if (line == "stop" && poll_watcher != nullptr) {
      stop = true;
      poll_watcher->join();
      delete poll_watcher;
      std::cout << "poll finished\n";
      done = true;
    }
  }

  return 0;
}
