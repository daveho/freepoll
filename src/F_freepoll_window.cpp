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
  , m_timer_display( 0, 0, TIMER_DISPLAY_WIDTH, TIMER_DISPLAY_HEIGHT, " 0:00" )
  , m_count_display( 0, 0, COUNT_DISPLAY_WIDTH, COUNT_DISPLAY_HEIGHT, "  0" )
  , m_graph_box( 0, BARGRAPH_Y, WIDTH, BARGRAPH_HEIGHT ) {

  // Layout is
  //   +--------------------------------------------+
  //   |              course chooser                |
  //   +--------------------------------------------+
  //   | [PollBtn] [Time] [Count] [Freq] [GraphBtn] |
  //   +--------------------------------------------+
  //   |                bar graph                   |
  //   +--------------------------------------------+

  m_graph_box.color( 0x0000FF00 );
  m_graph_box.box( FL_FLAT_BOX );

  m_controls.type( Fl_Pack::HORIZONTAL );
  m_controls.spacing( SPACING );

  m_poll_btn.label( "@>" );
  m_poll_btn.clear_visible_focus();

  m_timer_display.labelsize( TIMER_DISPLAY_HEIGHT - 12 );
  m_timer_display.labelcolor( DISABLED_TEXT_COLOR );

  m_count_display.labelsize( COUNT_DISPLAY_HEIGHT - 12 );
  m_count_display.labelcolor( DISABLED_TEXT_COLOR );

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
