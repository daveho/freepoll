// Copyright (c) 2022-2025, David Hovemeyer <david.hovemeyer@gmail.com>

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

#ifndef F_BAR_GRAPH_VIEW_H
#define F_BAR_GRAPH_VIEW_H

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include "observer.h"
#include "poll.h"

class F_BarGraphView : public Fl_Box, public Observer {
private:
  std::map<RemoteID, Option> m_cur_responses;

public:
  F_BarGraphView( int x, int y, int w, int h );
  ~F_BarGraphView();

  void draw() override;

  virtual void on_update(Observable *observable, int hint, bool is_async);
};

#endif // F_BAR_GRAPH_VIEW_H
