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
#include <cmath>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/pen.h>
#include <wx/brush.h>
#include "poll_model.h"
#include "poll.h"
#include "gui_common.h"
#include "bar_graph_view.h"

namespace {

const int BAR_GRAPH_HEIGHT = POLL_VIEW_EXPANDED_HEIGHT - POLL_VIEW_HEIGHT;

const int INSET = 8;

const wxColour BAR_COLORS[] = {
  wxColour(0, 0, 255),
  wxColour(0xff, 0xa5 ,0x00),
  wxColour(0x73, 0x82, 0x76),
  wxColour(0xa0, 0x20, 0xf0),
  wxColour(255, 0, 0),
};

}

BarGraphView::BarGraphView(wxWindow *parent, PollModel *model)
  : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(POLL_VIEW_WIDTH, BAR_GRAPH_HEIGHT))
  , m_model(model) {
  //SetBackgroundColour(*wxBLUE);
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
  wxPaintDC dc(this);
  draw_bar_graph(dc);
}

void BarGraphView::draw_bar_graph(wxDC &dc) {
  wxPen gray_pen(LIGHT_TEXT_COLOR);

  // draw background rectangle
  dc.SetPen(*wxTRANSPARENT_PEN);
  dc.SetBrush(*wxWHITE_BRUSH);
  dc.DrawRectangle(INSET, INSET, POLL_VIEW_WIDTH - INSET*2, BAR_GRAPH_HEIGHT - INSET*2);

  // draw x axis line
  int x_axis_row = BAR_GRAPH_HEIGHT-INSET*2;
  dc.SetPen(gray_pen);
  dc.DrawLine(INSET*2, x_axis_row, POLL_VIEW_WIDTH-INSET*2, x_axis_row); 

  std::map<RemoteID, Option> current_responses = m_model->get_poll()->get_final_responses();

  if (current_responses.empty()) {
    return;
  }

  // tally responses
  int counts[5] = {0, 0, 0, 0, 0};
  for (auto i = current_responses.begin(); i != current_responses.end(); ++i) {
    assert(int(i->second) >= 0 && int(i->second) <= 4);
    counts[int(i->second)]++;
  }

  // compute faction of total vote for each option
  double frac[5];
  double max_frac = 0.0;
  for (int i = 0; i < 5; i++) {
    frac[i] = double(counts[i]) / double(current_responses.size());
    if (frac[i] > max_frac) {
      max_frac = frac[i];
    }
  }

  // draw bars
  int max_bar_height = BAR_GRAPH_HEIGHT-INSET*4;
  int bar_width = (POLL_VIEW_WIDTH - INSET*8) / 5;
  for (int i = 0; i < 5; i++) {
    dc.SetPen(gray_pen);
    wxBrush fill(BAR_COLORS[i]);
    dc.SetBrush(fill);

    int bar_height = (frac[i] / max_frac) * max_bar_height;
    std::cout << "bar " << i << " height=" << bar_height << "\n";

    int left = INSET*2 + i*(INSET+bar_width);
    int top = x_axis_row - bar_height;
    dc.DrawRectangle(left, top, bar_width, bar_height);
  }
}

wxBEGIN_EVENT_TABLE(BarGraphView, wxPanel)
  EVT_COMMAND(CMD_POLL_DATA_UPDATED, wxEVT_COMMAND_TEXT_UPDATED, BarGraphView::on_poll_data_updated)
  EVT_PAINT(BarGraphView::on_paint)
wxEND_EVENT_TABLE()
