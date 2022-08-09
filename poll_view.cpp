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
#include "datastore.h"
#include "course.h"
#include "exception.h"
#include "base.h"
#include "poll_view.h"

namespace {

const int PLAY_STOP_BUTTON = 100;
const int COURSE_LIST = 101;

wxBitmap PLAY_BUTTON_BITMAP(play_button_icon);
wxBitmap STOP_BUTTON_BITMAP(stop_button_icon);

const int POLL_VIEW_WIDTH = 400;
const int POLL_VIEW_HEIGHT = 116;

}

PollView::PollView(wxWindow *parent, PollModel *model)
  : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(POLL_VIEW_WIDTH, POLL_VIEW_HEIGHT))
  , m_model(model)
  , m_poll_runner(nullptr) {
  wxBoxSizer *vlayout = new wxBoxSizer(wxVERTICAL);

  m_course_list = new wxChoice(this, COURSE_LIST, wxDefaultPosition, wxSize(POLL_VIEW_WIDTH, 32));
  for (Course *course : m_model->get_datastore()->get_courses()) {
    m_course_list->Append(course->get_display_string(), course);
  }
  m_course_list->SetSelection(0);
  vlayout->Add(m_course_list, 0, wxEXPAND);

  vlayout->AddSpacer(8);

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
  vlayout->Add(items);

  SetSizer(vlayout);
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
  if (!m_model->is_poll_running() && m_model->can_start_poll()) {
    // if a poll was started previously, reset it
    if (m_model->get_poll()->is_started()) {
      assert(m_model->get_poll()->is_stopped());
      m_model->get_poll()->reset();
    }

    assert(!m_model->get_poll()->is_started());
    assert(!m_model->get_poll()->is_stopped());

    // disable the course selector
    m_course_list->Enable(false);

    // start timer
    m_model->get_timer()->start();

    // change play/stop button bitmap
    m_poll_control_btn->SetBitmap(STOP_BUTTON_BITMAP);

    // make sure base station frequency is set appropriately
    Course *course = m_model->get_current_course();
    std::string freq = course->get_frequency();
    m_model->get_base()->set_frequency(freq[0], freq[1]);

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

    // write poll results to files
    Course *course = m_model->get_current_course();
    try {
      m_model->get_datastore()->write_poll_results(course, m_model->get_poll());
    } catch (PollException &ex) {
      std::cerr << "Error writing poll data: " << ex.what() << "\n";
      // TODO: probably should display the error in the GUI somehow
    }

    // re-enable course selection
    m_course_list->Enable(true);
  }
}

void PollView::on_selected_course_change(wxCommandEvent &evt) {
  m_model->set_current_course(unsigned(m_course_list->GetSelection()));
}

wxBEGIN_EVENT_TABLE(PollView, wxPanel)
  EVT_BUTTON(PLAY_STOP_BUTTON, PollView::on_play_stop_button)
  EVT_CHOICE(COURSE_LIST, PollView::on_selected_course_change)
wxEND_EVENT_TABLE()
