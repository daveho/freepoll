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
