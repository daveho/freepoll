#ifndef TIMER_VIEW_H
#define TIMER_VIEW_H

#include <wx/panel.h>
#include <wx/stattext.h>
#include "observer.h"

class Timer;

class TimerView : public wxPanel, public Observer {
private:
  Timer *m_timer;
  wxStaticText *m_label;

public:
  TimerView(wxWindow *parent, Timer *timer);
  virtual ~TimerView();

  virtual void on_update(Observable *observable, int hint);

private:
  wxDECLARE_EVENT_TABLE();

  void on_timer_update(wxCommandEvent &evt);
};

#endif // TIMER_VIEW_H
