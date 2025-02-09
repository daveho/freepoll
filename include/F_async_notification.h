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
