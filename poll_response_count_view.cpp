#include <sstream>
#include "poll.h"
#include "poll_response_count_view.h"

namespace {

const int POLL_UPDATED = 400;

}

PollResponseCountView::PollResponseCountView(wxWindow *parent, Poll *poll)
  : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(80, 44))
  , m_poll(poll) {
  SetFont(GetFont().Scale(2.5));
  m_label = new wxStaticText(this, wxID_ANY, "");
}

PollResponseCountView::~PollResponseCountView() {
}

void PollResponseCountView::on_update(Observable *observable, int hint) {
  // this is called from the poll runner thread, so post an event
  // which will ensure that the GUI update happens in the main
  // thread
  wxCommandEvent event(wxEVT_COMMAND_TEXT_UPDATED, POLL_UPDATED);
  GetEventHandler()->AddPendingEvent(event);
}

void PollResponseCountView::on_poll_update(wxCommandEvent &evt) {
  std::string text;

  if (!m_poll->is_started()) {
    text = "";
  } else {
    unsigned num_responses = m_poll->get_final_responses().size();
    std::stringstream ss;
    ss << num_responses;
    text = ss.str();
  }

  m_label->SetLabel(text);
}

wxBEGIN_EVENT_TABLE(PollResponseCountView, wxPanel)
  EVT_COMMAND(POLL_UPDATED, wxEVT_COMMAND_TEXT_UPDATED, PollResponseCountView::on_poll_update)
wxEND_EVENT_TABLE()
