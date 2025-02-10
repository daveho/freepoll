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

#include <cassert>
#include "F_notification_queue.h"

F_NotificationQueue::F_NotificationQueue() {

}

F_NotificationQueue::~F_NotificationQueue() {

}

bool F_NotificationQueue::empty() {
  std::lock_guard g( m_lock );
  return m_event_queue.empty();
}

F_AsyncNotification *F_NotificationQueue::dequeue() {
  std::lock_guard g( m_lock );
  assert( !m_event_queue.empty() );
  F_AsyncNotification *update = m_event_queue.front();
  m_event_queue.pop_front();
  return update;
}

void F_NotificationQueue::enqueue( F_AsyncNotification *update ) {
  std::lock_guard g( m_lock );
  m_event_queue.push_back( update );
}
