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

#include "observable.h"
#include "F_async_notification.h"

F_AsyncNotification::F_AsyncNotification( F_FreePollWindow *win, Observable *obj, int hint )
  : m_win( win )
  , m_observable( obj )
  , m_hint( hint ) {

}

F_AsyncNotification::~F_AsyncNotification() {

}

F_FreePollWindow *F_AsyncNotification::get_win() const {
  return m_win;
}

Observable *F_AsyncNotification::get_observable() const {
  return m_observable;
}

int F_AsyncNotification::get_hint() const {
  return m_hint;
}
