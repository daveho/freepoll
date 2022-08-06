#include "poll_view.h"

PollView::PollView(wxWindow *parent)
  : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(320, 96))
  , m_poll(nullptr) {
}

PollView::~PollView() {
}

void PollView::on_update(Observable *observable, int hint) {
  // TODO
}
