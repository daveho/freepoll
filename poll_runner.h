#ifndef POLL_RUNNER_H
#define POLL_RUNNER_H

#include <thread>
class Base;
class Poll;
class ResponseCallback;

// A PollRunner object takes care of executing a thread
// in which to run a poll and collect the responses

class PollRunner {
private:
  Base *m_base;
  Poll *m_poll;
  ResponseCallback *m_response_callback;
  volatile bool m_stop;
  std::thread *m_thread;

public:
  PollRunner(Base *base, Poll *poll);
  ~PollRunner();

  void start_poll();
  void stop_poll();

  bool is_poll_started() const { return m_thread != nullptr; }

private:
  static void run_poll(PollRunner *runner);
};

#endif // POLL_RUNNER_H
