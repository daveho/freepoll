#include "F_freepoll_window.h"

int main( int argc, char **argv ) {
  F_FreePollWindow win;
  win.show( argc, argv );
  return Fl::run();
}
