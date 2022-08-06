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
#include <wx/bitmap.h>
#include <wx/sizer.h>
#include "poll_view.h"
#include "play_button_icon.h"
#include "stop_button_icon.h"

namespace {

const int PLAY_STOP_BUTTON = 100;

wxBitmap PLAY_BUTTON_BITMAP(play_button_icon);
wxBitmap STOP_BUTTON_BITMAP(stop_button_icon);

}

PollView::PollView(wxWindow *parent)
  : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(320, 96))
  , m_poll(nullptr) {
  wxBoxSizer *items = new wxBoxSizer(wxHORIZONTAL);

  m_poll_control_btn = new wxButton(this, PLAY_STOP_BUTTON, "", wxDefaultPosition, wxSize(40, 40));
  m_poll_control_btn->SetBitmap(PLAY_BUTTON_BITMAP);
  items->Add(m_poll_control_btn);

  SetSizer(items);
}

PollView::~PollView() {
}

void PollView::on_update(Observable *observable, int hint) {
  // TODO
}

void PollView::on_play_stop_button(wxCommandEvent &evt) {
  std::cout << "button pressed!\n";

  static int x = 0;
  if (x == 0)
    m_poll_control_btn->SetBitmap(STOP_BUTTON_BITMAP);
  else
    m_poll_control_btn->SetBitmap(PLAY_BUTTON_BITMAP);
  x = !x;
}

wxBEGIN_EVENT_TABLE(PollView, wxPanel)
  EVT_BUTTON(PLAY_STOP_BUTTON, PollView::on_play_stop_button)
wxEND_EVENT_TABLE()
