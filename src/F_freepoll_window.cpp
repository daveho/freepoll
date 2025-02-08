#include <FL/fl_ask.H>
#include <FL/Fl_Pack.H>
#include "course.h"
#include "F_freepoll_window.h"

F_FreePollWindow::F_FreePollWindow( Base *base, DataStore *datastore )
  : Fl_Window( WIDTH, HEIGHT )
  , m_base( base )
  , m_datastore( datastore )
  , m_model( nullptr )
  , m_tile( 0, 0, WIDTH, HEIGHT )
  , m_course_chooser( 0, COURSE_CHOOSER_Y, WIDTH, COURSE_CHOOSER_HEIGHT )
  , m_controls( 0, CONTROLS_Y, WIDTH, CONTROLS_HEIGHT )
  , m_poll_btn( 0, 0, POLL_BTN_WIDTH, POLL_BTN_HEIGHT )
  , m_graph_box( 0, BARGRAPH_Y, WIDTH, BARGRAPH_HEIGHT ) {

  // Layout is
  //   +--------------------------------------------+
  //   |              course chooser                |
  //   +--------------------------------------------+
  //   | [PollBtn] [Time] [Count] [Freq] [GraphBtn] |
  //   +--------------------------------------------+
  //   |                bar graph                   |
  //   +--------------------------------------------+

  m_controls.color( 0xFF000000 );
  m_controls.box( FL_FLAT_BOX );
  m_graph_box.color( 0x0000FF00 );
  m_graph_box.box( FL_FLAT_BOX );

  m_controls.type( Fl_Pack::HORIZONTAL );
  m_controls.spacing( SPACING );
  m_controls.redraw();

  m_poll_btn.label( "@>" );
  m_poll_btn.redraw();

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
