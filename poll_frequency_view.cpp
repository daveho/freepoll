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

#include <cctype>
#include "poll.h"
#include "poll_model.h"
#include "course.h"
#include "gui_common.h"
#include "poll_frequency_view.h"

namespace {

const int POLL_OR_COURSE_UPDATED = 500;

std::string display_frequency(Course *course) {
  std::string freq = course->get_frequency();
  freq[0] = ::toupper(freq[0]);
  freq[1] = ::toupper(freq[1]);
  return freq;
}

}

PollFrequencyView::PollFrequencyView(wxWindow *parent, PollModel *model)
  : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(56, 44))
  , m_model(model)
  , m_poll_is_active(false) {
  SetFont(GetFont().Scale(FONT_SCALING_FACTOR));
  std::string freq_str = display_frequency(m_model->get_current_course());
  m_label = new wxStaticText(this, wxID_ANY, freq_str);
  m_label->SetForegroundColour(LIGHT_TEXT_COLOR);
}

PollFrequencyView::~PollFrequencyView() {
}

void PollFrequencyView::on_update(Observable *observable, int hint) {
  // this could be called from the poll runner thread, so post an event
  // which will ensure that the GUI update happens in the main
  // thread
  wxCommandEvent event(wxEVT_COMMAND_TEXT_UPDATED, POLL_OR_COURSE_UPDATED);
  event.SetInt(hint); // on_model_update will need the hint value
  GetEventHandler()->AddPendingEvent(event);
}

void PollFrequencyView::on_model_update(wxCommandEvent &evt) {
  int hint = evt.GetInt();

  if (hint == Poll::POLL_STARTED) {
    m_poll_is_active = true;
  } else if (hint == Poll::POLL_STOPPED) {
    m_poll_is_active = false;
  }

  if (m_poll_is_active) {
    m_label->SetForegroundColour(*wxBLACK);
  } else {
    m_label->SetForegroundColour(LIGHT_TEXT_COLOR);
  }

  m_label->SetLabel(display_frequency(m_model->get_current_course())); 
}

wxBEGIN_EVENT_TABLE(PollFrequencyView, wxPanel)
  EVT_COMMAND(POLL_OR_COURSE_UPDATED, wxEVT_COMMAND_TEXT_UPDATED, PollFrequencyView::on_model_update)
wxEND_EVENT_TABLE()
