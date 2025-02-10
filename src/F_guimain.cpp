// Copyright (c) 2022-2025, David Hovemeyer <david.hovemeyer@gmail.com>

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

  // Create main window, show it
  F_FreePollWindow win( model, datastore );
  win.show( argc, argv );

  // run the GUI event loop
  return Fl::run();
}
