#include "base.h"
#include "poll.h"
#include "timer.h"
#include "poll_model.h"

PollModel::PollModel()
  : m_base(new Base())
  , m_poll(new Poll())
  , m_timer(new Timer()) {
  m_poll->add_observer(this);
  m_timer->add_observer(this);
}

PollModel::~PollModel() {
  m_poll->remove_observer(this);
  m_timer->remove_observer(this);
  delete m_poll;
  delete m_timer;

  delete m_base;
}

bool PollModel::is_poll_running() {
  return m_poll->is_started();
}

bool PollModel::can_start_poll() {
  // TODO: could check whether the base station is connected
  return !m_poll->is_started();
}

Base *PollModel::get_base() {
  return m_base;
}

Poll *PollModel::get_poll() {
  return m_poll;
}

Timer *PollModel::get_timer() {
  return m_timer;
}

void PollModel::on_update(Observable *observable, int hint) {
  // Poll and Timer have non-overlapping hint values,
  // so no processing is necessary.
  notify_observers(hint);
}
