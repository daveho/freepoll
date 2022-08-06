// See: https://docs.wxwidgets.org/3.0/overview_helloworld.html

// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif

#include "datatypes.h"
#include "poll_view.h"

class FreePollApp: public wxApp
{
public:
  virtual bool OnInit();
};

class FreePollFrame: public wxFrame
{
private:
  PollView *m_poll_view;

public:
  FreePollFrame(const wxString& title);

private:
  void OnExit(wxCommandEvent& event);
  wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(FreePollFrame, wxFrame)
  EVT_MENU(wxID_EXIT,  FreePollFrame::OnExit)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(FreePollApp);

bool FreePollApp::OnInit()
{
  FreePollFrame *frame = new FreePollFrame( "FreePoll " FREEPOLL_VERSION );
  frame->Show( true );
  return true;
}

FreePollFrame::FreePollFrame(const wxString& title)
  : wxFrame(NULL, wxID_ANY, title)
{
  m_poll_view = new PollView(this);
  //m_poll_view->SetBackgroundColour(wxColour(*wxBLUE));

  Fit();
}

void FreePollFrame::OnExit(wxCommandEvent& event)
{
  Close( true );
}
