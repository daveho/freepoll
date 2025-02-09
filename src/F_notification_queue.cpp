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
