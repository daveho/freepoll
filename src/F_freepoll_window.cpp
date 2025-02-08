#include <FL/fl_ask.H>
#include <FL/Fl_Pack.H>
#include "course.h"
#include "F_freepoll_window.h"

F_FreePollWindow::F_FreePollWindow( Base *base, DataStore *datastore )
  : Fl_Window( DEFAULT_WIDTH, DEFAULT_HEIGHT )
  , m_base( base )
  , m_datastore( datastore )
  , m_model( nullptr )
  , m_tile( 0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT )
  , m_course_chooser( 0, 0, DEFAULT_WIDTH, DEFAULT_COURSE_CHOOSER_HEIGHT )
  , m_controls_box( 0, DEFAULT_COURSE_CHOOSER_HEIGHT, DEFAULT_WIDTH, DEFAULT_CONTROLS_HEIGHT )
  , m_graph_box( 0, DEFAULT_COURSE_CHOOSER_HEIGHT+DEFAULT_CONTROLS_HEIGHT, DEFAULT_WIDTH, DEFAULT_BARGRAPH_HEIGHT ) {

  // Layout is
  //   +--------------------------------------------+
  //   |              course chooser                |
  //   +--------------------------------------------+
  //   | [PollBtn] [Time] [Count] [Freq] [GraphBtn] |
  //   +--------------------------------------------+
  //   |                bar graph                   |
  //   +--------------------------------------------+
  //
  // There are three Fl_Pack containers with horizontal
  // layout for each of the three areas. A top-level Fl_Pack
  // with vertical layout contains these Fl_Packs.

  m_controls_box.color( 0xFF000000 );
  m_controls_box.box( FL_FLAT_BOX );
  m_graph_box.color( 0x0000FF00 );
  m_graph_box.box( FL_FLAT_BOX );

  for ( auto course : m_datastore->get_courses() )
    m_course_chooser.add( course->get_display_string().c_str() );
  m_course_chooser.value( 0 );
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
