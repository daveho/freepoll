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
#include "timer.h"
#include "timer_view.h"

namespace {

const int PLAY_STOP_BUTTON = 100;

wxBitmap PLAY_BUTTON_BITMAP(play_button_icon);
wxBitmap STOP_BUTTON_BITMAP(stop_button_icon);

}

PollView::PollView(wxWindow *parent, Base *base)
  : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(320, 96))
  , m_base(base)
  , m_poll(nullptr)
  , m_timer(new Timer()) {
  wxBoxSizer *items = new wxBoxSizer(wxHORIZONTAL);

  items->AddSpacer(10);

  m_poll_control_btn = new wxButton(this, PLAY_STOP_BUTTON, "", wxDefaultPosition, wxSize(40, 40));
  m_poll_control_btn->SetBitmap(PLAY_BUTTON_BITMAP);

  items->Add(m_poll_control_btn, 0, wxALL|wxALIGN_CENTRE);

  items->AddSpacer(10);

  m_timer_view = new TimerView(this, m_timer);
  items->Add(m_timer_view, 0, wxALL|wxALIGN_CENTRE);

  m_timer->add_observer(m_timer_view);

  SetSizer(items);
}

PollView::~PollView() {
  m_timer->remove_observer(m_timer_view);

  delete m_timer;
}

void PollView::on_update(Observable *observable, int hint) {
  // TODO
}

void PollView::on_play_stop_button(wxCommandEvent &evt) {
  std::cout << "button pressed!\n";

  if (!m_timer->is_running()) {
    m_timer->start();
    m_poll_control_btn->SetBitmap(STOP_BUTTON_BITMAP);
  } else {
    m_timer->stop();
    m_poll_control_btn->SetBitmap(PLAY_BUTTON_BITMAP);
  }
}

wxBEGIN_EVENT_TABLE(PollView, wxPanel)
  EVT_BUTTON(PLAY_STOP_BUTTON, PollView::on_play_stop_button)
wxEND_EVENT_TABLE()
