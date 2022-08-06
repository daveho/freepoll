#include <iostream>
#include <sstream>
#include <iomanip>
#include "timer.h"
#include "timer_view.h"

namespace {

const int TIMER_UPDATED = 200;

}

// Caller is responsible for registering the TimerView as an
// observer of the Timer

TimerView::TimerView(wxWindow *parent, Timer *timer)
  : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(100, 44))
  , m_timer(timer) {
  SetFont(GetFont().Scale(2.5));
  m_label = new wxStaticText(this, wxID_ANY, "");
}

TimerView::~TimerView() {
}

void TimerView::on_update(Observable *observable, int hint) {
  std::cout << "timer sent a notification\n";

  // this is called from the timer thread, so post an event
  // which will ensure that the GUI update happens in the main
  // thread
  wxCommandEvent event(wxEVT_COMMAND_TEXT_UPDATED, TIMER_UPDATED);
  GetEventHandler()->AddPendingEvent(event);
}

void TimerView::on_timer_update(wxCommandEvent &evt) {
  std::cout << "processing timer event in view\n";

  if (!m_timer->is_running()) {
    m_label->SetLabel("");
  } else {
    unsigned num_seconds = m_timer->get_num_seconds();

    unsigned min = num_seconds / 60;
    unsigned sec = num_seconds % 60;

    std::stringstream ss;
    ss << min << ":" << std::setfill('0') << std::setw(2) << sec;

    m_label->SetLabel(ss.str());
  }
}

wxBEGIN_EVENT_TABLE(TimerView, wxPanel)
  EVT_COMMAND(TIMER_UPDATED, wxEVT_COMMAND_TEXT_UPDATED, TimerView::on_timer_update)
wxEND_EVENT_TABLE()
