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

#ifndef TIMER_VIEW_H
#define TIMER_VIEW_H

#include <wx/panel.h>
#include <wx/stattext.h>
#include "observer.h"

class Timer;

class TimerView : public wxPanel, public Observer {
private:
  Timer *m_timer;
  wxStaticText *m_label;

public:
  TimerView(wxWindow *parent, Timer *timer);
  virtual ~TimerView();

  virtual void on_update(Observable *observable, int hint);

private:
  wxDECLARE_EVENT_TABLE();

  void on_timer_update(wxCommandEvent &evt);
};

#endif // TIMER_VIEW_H
