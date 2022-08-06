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

#ifndef POLL_VIEW_H
#define POLL_VIEW_H

#include <wx/panel.h>
#include <wx/button.h>
#include "observer.h"

class Base;
class Poll;
class Timer;
class TimerView;

class PollView : public wxPanel, public Observer {
private:
  Base *m_base;
  Poll *m_poll;
  Timer *m_timer;
  wxButton *m_poll_control_btn;
  TimerView *m_timer_view;

public:
  PollView(wxWindow *parent, Base *base);
  virtual ~PollView();

  virtual void on_update(Observable *observable, int hint);

private:
  void on_play_stop_button(wxCommandEvent &evt);

  wxDECLARE_EVENT_TABLE();
};

#endif // POLL_VIEW_H
