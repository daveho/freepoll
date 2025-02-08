#include <FL/fl_ask.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Pixmap.H>
#include "datatypes.h"
#include "course.h"
#include "bar_graph_icon.h"
#include "F_freepoll_window.h"

namespace {

Fl_Pixmap bar_graph_pixmap( bar_graph_icon );

}

F_FreePollWindow::F_FreePollWindow( Base *base, DataStore *datastore )
  : Fl_Window( WIDTH, HEIGHT, "FreePoll " FREEPOLL_VERSION "-fltk" )
  , m_base( base )
  , m_datastore( datastore )
  , m_model( nullptr )
  , m_graph_shown( true )
  , m_tile( 0, 0, WIDTH, HEIGHT )
  , m_course_chooser( 0, COURSE_CHOOSER_Y, WIDTH, COURSE_CHOOSER_HEIGHT )
  , m_controls( 0, CONTROLS_Y, WIDTH, CONTROLS_HEIGHT )
  , m_poll_btn( 0, 0, POLL_BTN_WIDTH, POLL_BTN_HEIGHT )
  , m_timer_display( 0, 0, TIMER_DISPLAY_WIDTH, TIMER_DISPLAY_HEIGHT, " 0:00" )
  , m_count_display( 0, 0, COUNT_DISPLAY_WIDTH, COUNT_DISPLAY_HEIGHT, "  0" )
  , m_gap( 0, 0, GAP_WIDTH, CONTROLS_HEIGHT )
  , m_graph_btn( 0, 0, GRAPH_BTN_WIDTH, GRAPH_BTN_HEIGHT )
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

  m_graph_btn.image( &bar_graph_pixmap );
  m_graph_btn.clear_visible_focus();
  m_graph_btn.callback( on_graph_button_clicked, static_cast<void*>(this) );

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

void F_FreePollWindow::on_graph_button_clicked( Fl_Widget *w, void *data ) {
  F_FreePollWindow *win = static_cast<F_FreePollWindow*>( data );
  if ( win->m_graph_shown ) {
    // resize the window to hide the graph
    win->resize( win->x(), win->y(), win->w(), HEIGHT_NOGRAPH );
    win->m_graph_shown = false;
  } else {
    // resize the window to show the graph
    win->resize( win->x(), win->y(), win->w(), HEIGHT );
    win->m_graph_shown = true;
  }
}
