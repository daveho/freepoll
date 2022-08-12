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

#ifndef POLL_VIEW_VIEWPORT_H
#define POLL_VIEW_VIEWPORT_H

#include <wx/panel.h>
#include "observer.h"
class PollModel;

// The PollViewViewport is a panel that adjusts its size to
// accommodate the bar graph view if it is being displayed.
// It observes the PollModel and adjusts its size depending on whether
// or not the bar graph display is enabled.

class PollViewViewport : public wxPanel, public Observer {
private:
  PollModel *m_model;

public:
  PollViewViewport(wxWindow *parent, PollModel *model);
  virtual ~PollViewViewport();

  virtual void on_update(Observable *observable, int hint);
};

#endif // POLL_VIEW_VIEWPORT_H
