#include <FL/fl_ask.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Pixmap.H>
#include <iostream>
#include <cassert>
#include "datatypes.h"
#include "course.h"
#include "bar_graph_icon.h"
#include "F_freepoll_window.h"

namespace {

Fl_Pixmap bar_graph_pixmap( bar_graph_icon );

}

F_FreePollWindow::F_FreePollWindow( Base *base, DataStore *datastore )
  : Fl_Window( WIDTH, HEIGHT_NOGRAPH, "FreePoll " FREEPOLL_VERSION "-fltk" )
  , m_base( base )
  , m_datastore( datastore )
  , m_model( nullptr )
  , m_graph_shown( false )
  , m_pack( 0, 0, WIDTH, HEIGHT )
  , m_course_chooser( 0, 0, WIDTH, COURSE_CHOOSER_HEIGHT )
  , m_controls( 0, 0, WIDTH, CONTROLS_HEIGHT )
  , m_poll_btn( POLL_BTN_X, CONTROL_Y, POLL_BTN_WIDTH, POLL_BTN_HEIGHT )
  , m_timer_display( TIMER_DISPLAY_X, CONTROL_Y, TIMER_DISPLAY_WIDTH, TIMER_DISPLAY_HEIGHT, "0:00" )
  , m_count_display( COUNT_DISPLAY_X, CONTROL_Y, COUNT_DISPLAY_WIDTH, COUNT_DISPLAY_HEIGHT, "0" )
  , m_freq_display( FREQ_DISPLAY_X, CONTROL_Y, FREQ_DISPLAY_WIDTH, FREQ_DISPLAY_HEIGHT, "" )
  , m_graph_btn( GRAPH_BTN_X, CONTROL_Y, GRAPH_BTN_WIDTH, GRAPH_BTN_HEIGHT )
  , m_graph_box( 0, 0, WIDTH, BARGRAPH_HEIGHT ) {

  // Layout is
  //   +--------------------------------------------+
  //   |              course chooser                |
  //   +--------------------------------------------+
  //   | [PollBtn] [Time] [Count] [Freq] [GraphBtn] |
  //   +--------------------------------------------+
  //   |                bar graph                   |
  //   +--------------------------------------------+

  m_pack.type( Fl_Pack::VERTICAL );
  m_pack.spacing( SPACING );

  m_graph_box.color( 0x0000FF00 );
  m_graph_box.box( FL_FLAT_BOX );

  m_poll_btn.label( "@>" );
  m_poll_btn.clear_visible_focus();

  m_timer_display.labelsize( TEXT_SIZE );
  m_timer_display.labelcolor( DISABLED_TEXT_COLOR );

  m_count_display.labelsize( TEXT_SIZE );
  m_count_display.labelcolor( DISABLED_TEXT_COLOR );
  //m_count_display.align( FL_ALIGN_RIGHT );

  m_freq_display.labelsize( TEXT_SIZE );

  m_graph_btn.image( &bar_graph_pixmap );
  m_graph_btn.clear_visible_focus();

  // Populate courses in course chooser
  for ( auto course : m_datastore->get_courses() )
    m_course_chooser.add( course->get_display_string().c_str() );
  m_course_chooser.value( 0 );

  // Display current frequency
  update_frequency_display();

  // register callbacks to handle UI events
  m_graph_btn.callback( on_graph_button_clicked, static_cast<void*>(this) );
  m_course_chooser.callback( on_course_change, static_cast<void*>( this ) );

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

void F_FreePollWindow::on_course_change( Fl_Widget *w, void *data ) {
  F_FreePollWindow *win = static_cast<F_FreePollWindow*>( data );
  win->update_frequency_display();
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

void F_FreePollWindow::update_frequency_display() {
  int course_index = m_course_chooser.value();
  const auto &courses = m_datastore->get_courses();
  Course *course = courses.at( course_index );
  Frequency freq = course->get_frequency();
  std::string freq_str = freq.str();
  //std::cout << "freq_str=" << freq_str << "\n";
  m_freq_display.copy_label( freq.str().c_str() );
}