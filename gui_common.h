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

#ifndef GUI_COMMON_H
#define GUI_COMMON_H

#define LIGHT_TEXT_COLOR wxColor(128, 128, 128)

const double FONT_SCALING_FACTOR = 2.5;

// window IDs
const int PLAY_STOP_BUTTON = 100;
const int COURSE_LIST = 101;
const int BAR_GRAPH_BUTTON = 102;

// command event IDs
const int CMD_TIMER_UPDATED = 200;
const int CMD_BAR_GRAPH_ENABLEMENT_CHANGED = 201;

// widget sizing
const int POLL_VIEW_WIDTH = 350;
const int POLL_VIEW_HEIGHT = 100;          // height w/ bar graph hidden
const int POLL_VIEW_EXPANDED_HEIGHT = 350; // height w/ bar graph visible
const int COURSE_LIST_HEIGHT = 28;
const int H_SEP_SMALL = 10;
const int TIMER_VIEW_WIDTH = 80;
const int TIMER_VIEW_HEIGHT = 44;

#endif // GUI_COMMON_H
