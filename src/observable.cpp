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

#include <algorithm>
#include "observer.h"
#include "observable.h"

Observable::Observable() {
}

Observable::~Observable() {
}

void Observable::add_observer(Observer *observer) {
  m_observers.push_back(observer);
}

void Observable::remove_observer(Observer *observer) {
  auto i = std::find(m_observers.begin(), m_observers.end(), observer);
  if (i != m_observers.end()) {
    m_observers.erase(i);
  }
}

void Observable::notify_observers(int hint, bool is_async) {
  for (auto i = m_observers.begin(); i != m_observers.end(); ++i) {
    (*i)->on_update(this, hint, is_async);
  }
}
