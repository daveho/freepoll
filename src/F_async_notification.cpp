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
