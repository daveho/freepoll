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

#ifndef F_ASYNC_NOTIFICATION_H
#define F_ASYNC_NOTIFICATION_H

class F_FreePollWindow;
class Observable;

// Object used to redirect observable updates
// generated in threads back to the main GUI thread.
// All such updates are reported to the F_FreePollWindow
// object via its on_update() member function. This
// works out well because it's the only observer
// in the FLTK GUI implementation.
class F_AsyncNotification {
private:
  F_FreePollWindow *m_win;
  Observable *m_observable;
  int m_hint;

public:
  F_AsyncNotification( F_FreePollWindow *win, Observable *obj, int hint );
  ~F_AsyncNotification();

  F_FreePollWindow *get_win() const;
  Observable *get_observable() const;
  int get_hint() const;
};

#endif // F_ASYNC_NOTIFICATION_H
