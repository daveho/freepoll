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
#include <iomanip>
#include <wx/bitmap.h>
#include <wx/sizer.h>
#include "play_button_icon.h"
#include "stop_button_icon.h"
#include "timer.h"
#include "timer_view.h"
#include "poll.h"
#include "poll_response_count_view.h"
#include "poll_model.h"
#include "poll_runner.h"
#include "poll_view.h"

namespace {

const int PLAY_STOP_BUTTON = 100;

wxBitmap PLAY_BUTTON_BITMAP(play_button_icon);
wxBitmap STOP_BUTTON_BITMAP(stop_button_icon);

}

PollView::PollView(wxWindow *parent, PollModel *model)
  : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(320, 96))
  , m_model(model)
  , m_poll_runner(nullptr) {
  wxBoxSizer *items = new wxBoxSizer(wxHORIZONTAL);

  items->AddSpacer(10);

  m_poll_control_btn = new wxButton(this, PLAY_STOP_BUTTON, "", wxDefaultPosition, wxSize(40, 40));
  m_poll_control_btn->SetBitmap(PLAY_BUTTON_BITMAP);

  items->Add(m_poll_control_btn, 0, wxALL|wxALIGN_CENTRE);

  items->AddSpacer(10);

  m_timer_view = new TimerView(this, m_model->get_timer());
  items->Add(m_timer_view, 0, wxALL|wxALIGN_CENTRE);

  m_model->get_timer()->add_observer(m_timer_view);

  items->AddSpacer(40);

  m_poll_response_count_view = new PollResponseCountView(this, m_model->get_poll());
  items->Add(m_poll_response_count_view, 0, wxALL|wxALIGN_CENTRE);

  m_model->get_poll()->add_observer(m_poll_response_count_view);

  SetSizer(items);
}

PollView::~PollView() {
  m_model->get_timer()->remove_observer(m_timer_view);
  m_model->get_poll()->remove_observer(m_poll_response_count_view);

  delete m_model;
}

void PollView::on_update(Observable *observable, int hint) {
  // TODO
}

void PollView::on_play_stop_button(wxCommandEvent &evt) {
  std::cout << "button pressed!\n";

  if (!m_model->is_poll_running() && m_model->can_start_poll()) {
    // if a poll was started previously, reset it
    if (m_model->get_poll()->is_started()) {
      assert(m_model->get_poll()->is_stopped());
      m_model->get_poll()->reset();
    }

    // start timer
    m_model->get_timer()->start();

    // change play/stop button bitmap
    m_poll_control_btn->SetBitmap(STOP_BUTTON_BITMAP);

    // start poll
    m_poll_runner = new PollRunner(m_model->get_base(), m_model->get_poll());
    m_poll_runner->start_poll();

    // TODO: save screenshot
  } else if (m_model->is_poll_running()) {
    // stop timer
    m_model->get_timer()->stop();

    // change play/stop button bitmap
    m_poll_control_btn->SetBitmap(PLAY_BUTTON_BITMAP);

    // stop the poll
    m_poll_runner->stop_poll();
    delete m_poll_runner;
    m_poll_runner = nullptr;

    // TODO: write poll results to file
    std::cout << "poll finished:\nRemoteID,Option\n";
    std::map<RemoteID, Option> responses = m_model->get_poll()->get_final_responses();
    for (auto i = responses.begin(); i != responses.end(); ++i) {
      std::cout << std::hex << i->first << "," << char('A' + int(i->second)) << "\n";
    }
  }
}

wxBEGIN_EVENT_TABLE(PollView, wxPanel)
  EVT_BUTTON(PLAY_STOP_BUTTON, PollView::on_play_stop_button)
wxEND_EVENT_TABLE()
