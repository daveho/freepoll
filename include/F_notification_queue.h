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

#ifndef F_NOTIFICATION_QUEUE_H
#define F_NOTIFICATION_QUEUE_H

#include <deque>
#include <mutex>
class F_AsyncNotification;

// A thread-safe queue of F_AsyncNotification objects.
// This is used by F_FreePollWindow to accumulate async
// notifications. I originally intended to use Fl::awake( callback, data )
// to redirect async updates to the main thread, but for some
// reason the callback is never actually called. So instead,
// we'll just have F_FreePollWindow use a timer to periodically
// check for async updates and deal with them.
class F_NotificationQueue {
private:
  std::deque<F_AsyncNotification*> m_event_queue;
  std::mutex m_lock;

public:
  F_NotificationQueue();
  ~F_NotificationQueue();

  bool empty();
  F_AsyncNotification *dequeue();

  void enqueue( F_AsyncNotification *update );
};

#endif // F_NOTIFICATION_QUEUE_H
