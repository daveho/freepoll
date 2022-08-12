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
#include "poll_model.h"
#include "poll.h"
#include "gui_common.h"
#include "bar_graph_view.h"

BarGraphView::BarGraphView(wxWindow *parent, PollModel *model)
  : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(POLL_VIEW_WIDTH, POLL_VIEW_EXPANDED_HEIGHT - POLL_VIEW_HEIGHT))
  , m_model(model) {
  SetBackgroundColour(*wxBLUE);
}

BarGraphView::~BarGraphView() {
}

void BarGraphView::on_update(Observable *observable, int hint) {
  if (hint == Poll::POLL_STARTED || hint == Poll::POLL_STARTED || hint == Poll::POLL_RESPONSE_RECORDED) {
    // poll data changed, most likely in a thread other than the
    // GUI event loop thread, so schedule an update
    wxCommandEvent event(wxEVT_COMMAND_TEXT_UPDATED, CMD_POLL_DATA_UPDATED);
    GetEventHandler()->AddPendingEvent(event);
  }
}

void BarGraphView::on_poll_data_updated(wxCommandEvent &evt) {
  Refresh();
}

void BarGraphView::on_paint(wxPaintEvent &evt) {
  std::cout << "paint bar graph!\n";
}

wxBEGIN_EVENT_TABLE(BarGraphView, wxPanel)
  EVT_COMMAND(CMD_POLL_DATA_UPDATED, wxEVT_COMMAND_TEXT_UPDATED, BarGraphView::on_poll_data_updated)
  EVT_PAINT(BarGraphView::on_paint)
wxEND_EVENT_TABLE()
