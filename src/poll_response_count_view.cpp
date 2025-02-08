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

#include <sstream>
#include "poll.h"
#include "gui_common.h"
#include "poll_response_count_view.h"

namespace {

const int POLL_UPDATED = 400;

const int POLL_RESPONSE_COUNT_VIEW_WIDTH = 75;
const int POLL_RESPONSE_COUNT_VIEW_HEIGHT = 44;

}

PollResponseCountView::PollResponseCountView(wxWindow *parent, Poll *poll)
  : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(POLL_RESPONSE_COUNT_VIEW_WIDTH, POLL_RESPONSE_COUNT_VIEW_HEIGHT))
  , m_poll(poll) {
  SetFont(GetFont().Scale(FONT_SCALING_FACTOR));
  m_label = new wxStaticText(this, wxID_ANY, "0");

  m_label->SetForegroundColour(LIGHT_TEXT_COLOR);
}

PollResponseCountView::~PollResponseCountView() {
}

void PollResponseCountView::on_update(Observable *observable, int hint) {
  // this is called from the poll runner thread, so post an event
  // which will ensure that the GUI update happens in the main
  // thread
  wxCommandEvent event(wxEVT_COMMAND_TEXT_UPDATED, POLL_UPDATED);
  GetEventHandler()->AddPendingEvent(event);
}

void PollResponseCountView::on_poll_update(wxCommandEvent &evt) {
  std::string text;

  if (!m_poll->is_started() || m_poll->is_stopped()) {
    m_label->SetForegroundColour(LIGHT_TEXT_COLOR);
  } else {
    m_label->SetForegroundColour(*wxBLACK);
  }

  std::stringstream ss;
  ss << m_poll->get_num_final_responses();
  text = ss.str();

  m_label->SetLabel(text);
}

wxBEGIN_EVENT_TABLE(PollResponseCountView, wxPanel)
  EVT_COMMAND(POLL_UPDATED, wxEVT_COMMAND_TEXT_UPDATED, PollResponseCountView::on_poll_update)
wxEND_EVENT_TABLE()
