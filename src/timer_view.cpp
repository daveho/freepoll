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
#include <sstream>
#include <iomanip>
#include "timer.h"
#include "gui_common.h"
#include "timer_view.h"

namespace {
}

// Caller is responsible for registering the TimerView as an
// observer of the Timer

TimerView::TimerView(wxWindow *parent, Timer *timer)
  : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(TIMER_VIEW_WIDTH, TIMER_VIEW_HEIGHT))
  , m_timer(timer) {
  SetFont(GetFont().Scale(FONT_SCALING_FACTOR));
  m_label = new wxStaticText(this, wxID_ANY, "0:00");
  m_label->SetForegroundColour(LIGHT_TEXT_COLOR);
}

TimerView::~TimerView() {
}

void TimerView::on_update(Observable *observable, int hint) {
  //std::cout << "timer sent a notification\n";

  // this is called from the timer thread, so post an event
  // which will ensure that the GUI update happens in the main
  // thread
  wxCommandEvent event(wxEVT_COMMAND_TEXT_UPDATED, CMD_TIMER_UPDATED);
  GetEventHandler()->AddPendingEvent(event);
}

void TimerView::on_timer_update(wxCommandEvent &evt) {
  //std::cout << "processing timer event in view\n";

  if (!m_timer->is_running()) {
    m_label->SetForegroundColour(LIGHT_TEXT_COLOR);
    m_label->SetLabel("0:00");
  } else {
    unsigned num_seconds = m_timer->get_num_seconds();

    unsigned min = num_seconds / 60;
    unsigned sec = num_seconds % 60;

    std::stringstream ss;
    ss << min << ":" << std::setfill('0') << std::setw(2) << sec;

    m_label->SetForegroundColour(*wxBLACK);
    m_label->SetLabel(ss.str());
  }
}

wxBEGIN_EVENT_TABLE(TimerView, wxPanel)
  EVT_COMMAND(CMD_TIMER_UPDATED, wxEVT_COMMAND_TEXT_UPDATED, TimerView::on_timer_update)
wxEND_EVENT_TABLE()
