#include <iostream>
#include <string>
#include <cstdlib>
#include "exception.h"
#include "F_freepoll_window.h"

int main( int argc, char **argv ) {
  // FIXME: it would be nice to pop up a GUI error dialog if initialization fails

  // Create data store and make sure there is at least one course
  const char *home = ::getenv("HOME");
  if ( home == nullptr ) {
    std::cerr << "HOME environment variable isn't set\n";
    return 1;
  }
  std::string data_dir = std::string( home ) + "/FreePoll";
  DataStore *datastore = new DataStore( data_dir );
  datastore->locate_courses();
  if ( datastore->get_courses().empty() ) {
    std::cerr << data_dir << " does not contain any courses\n";
    return 1;
  }

  // Instantiate PollModel
  PollModel *model = new PollModel( datastore );

  // Initialize base
  try {
    model->get_base()->initialize();
  } catch ( PollException &ex ) {
    std::cerr << "Couldn't connect to base: " << ex.what() << "\n";
    return 1;
  }

  F_FreePollWindow win( model, datastore );
  win.show( argc, argv );
  return Fl::run();
}
