#ifndef POLL_VIEW_H
#define POLL_VIEW_H

#include <wx/panel.h>
#include "observer.h"

class Poll;

class PollView : public wxPanel, public Observer {
private:
  Poll *m_poll;

public:
  PollView(wxWindow *parent);
  virtual ~PollView();

  virtual void on_update(Observable *observable, int hint);
};

#endif // POLL_VIEW_H
