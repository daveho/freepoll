#ifndef POLL_RESPONSE_COUNT_VIEW_H
#define POLL_RESPONSE_COUNT_VIEW_H

#include <wx/panel.h>
#include <wx/stattext.h>
#include "observer.h"

class Poll;

// this view just prints a count of how many RemoteIDs have
// responded in the current Poll

class PollResponseCountView : public wxPanel, public Observer {
private:
  Poll *m_poll;
  wxStaticText *m_label;

public:
  PollResponseCountView(wxWindow *parent, Poll *poll);
  virtual ~PollResponseCountView();

  virtual void on_update(Observable *observable, int hint);

private:
  wxDECLARE_EVENT_TABLE();

  void on_poll_update(wxCommandEvent &evt);
};

#endif // POLL_RESPONSE_COUNT_VIEW_H
