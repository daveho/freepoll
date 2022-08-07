#include <cassert>
#include "base.h"
#include "poll.h"
#include "poll_response_collector.h"
#include "poll_runner.h"

PollRunner::PollRunner(Base *base, Poll *poll)
  : m_base(base)
  , m_poll(poll)
  , m_response_callback(new PollResponseCollector(*poll))
  , m_stop(false)
  , m_thread(nullptr) {
}

PollRunner::~PollRunner() {
  delete m_response_callback;
}

void PollRunner::start_poll() {
  assert(m_thread == nullptr);
  m_poll->start();
  m_thread = new std::thread(run_poll, this);
}

void PollRunner::stop_poll() {
  assert(m_thread != nullptr);
  m_stop = true;
  m_thread->join();
  m_poll->stop();
}

void PollRunner::run_poll(PollRunner *runner) {
  runner->m_base->start_poll(Base::ALPHA);
  runner->m_base->collect_responses(runner->m_stop, runner->m_response_callback);
  runner->m_base->stop_poll();
}
