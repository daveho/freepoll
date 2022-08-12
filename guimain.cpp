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

#include <iostream>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include <wx/stdpaths.h>

#include "datatypes.h"
#include "datastore.h"
#include "base.h"
#include "exception.h"
#include "poll_view_viewport.h"
#include "poll_view.h"
#include "poll_model.h"

class FreePollApp: public wxApp
{
private:
  DataStore *m_datastore;

public:
  virtual ~FreePollApp();
  virtual bool OnInit();

private:
  void show_error_dialog(const wxString &msg);
};

class FreePollFrame: public wxFrame
{
private:
  PollModel *m_model;
  PollViewViewport *m_poll_view_viewport;
  PollView *m_poll_view;

public:
  FreePollFrame(const wxString& title, PollModel *model);
  virtual ~FreePollFrame();

private:
  void OnExit(wxCloseEvent& event);
  wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(FreePollFrame, wxFrame)
  EVT_CLOSE(FreePollFrame::OnExit)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(FreePollApp);

FreePollApp::~FreePollApp() {
  delete m_datastore;
}

bool FreePollApp::OnInit()
{
  auto &stdpaths = wxStandardPaths::Get();

  std::string user_home(stdpaths.GetUserConfigDir().mb_str());

  m_datastore = new DataStore(user_home + "/FreePoll");
  m_datastore->locate_courses();

  unsigned num_courses_located = unsigned(m_datastore->get_courses().size());
  if (num_courses_located == 0) {
    show_error_dialog(wxT("No courses found"));
    return false;
  }

  //std::cout << "Found " << num_courses_located << " course(s)\n";

  PollModel *model = new PollModel(m_datastore);

  // FIXME: it would be nice to have a more dynamic way to connect to the base
  // (e.g., periodically trying to connect to it, so that the program could
  // detect if the base is plugged in while the program is running)
  try {
    model->get_base()->initialize();
  } catch (PollException &ex) {
    //std::cerr << "Could not initialize base station: " << ex.what() << "\n";
    show_error_dialog(wxT("Error: ") + wxString(ex.what()));
    return false;
  }

  FreePollFrame *frame = new FreePollFrame( "FreePoll v" FREEPOLL_VERSION, model );
  frame->Show( true );
  return true;
}

void FreePollApp::show_error_dialog(const wxString &msg) {
  wxMessageDialog *dialog = new wxMessageDialog(NULL, msg, wxT("Error"), wxOK | wxICON_ERROR);
  dialog->ShowModal();
  delete dialog;
}

FreePollFrame::FreePollFrame(const wxString& title, PollModel *model)
  : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE|wxSTAY_ON_TOP)
{
  m_model = model;
  m_poll_view_viewport = new PollViewViewport(this, model);
  m_poll_view = new PollView(m_poll_view_viewport, model);

  m_model->add_observer(m_poll_view_viewport);

  Fit();
}

FreePollFrame::~FreePollFrame() {
  m_model->remove_observer(m_poll_view_viewport);
}

void FreePollFrame::OnExit(wxCloseEvent& event)
{
  std::cout << "FreePollFrame::OnExit called\n";

  // check the PollModel to make sure that there is not a poll in progress
  if (event.CanVeto() && m_model->is_poll_running()) {
    std::cout << "attempt to close FreePoll while a poll is running\n";
    event.Veto();
    return;
  }

  Destroy();
}
