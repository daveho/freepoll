#include <FL/fl_ask.H>
#include "F_freepoll_window.h"

F_FreePollWindow::F_FreePollWindow( Base *base, DataStore *datastore )
  : Fl_Window( DEFAULT_WIDTH, DEFAULT_HEIGHT )
  , m_base( base )
  , m_datastore( datastore )
  , m_model( nullptr ) {
}

F_FreePollWindow::~F_FreePollWindow() {

}

void F_FreePollWindow::show( int argc, char **argv ) {
  // Could do application-specific command line argument
  // handling here...
  Fl_Window::show( argc, argv );
}

void F_FreePollWindow::on_update(Observable *observable, int hint) {

}
