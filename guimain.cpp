// Copyright (c) 2022, David Hovemeyer <david.hovemeyer@gmail.com>

// This file is part of FreePoll.
//
// FreePoll is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// FreePoll is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with FreePoll. If not, see <https://www.gnu.org/licenses/>.

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
