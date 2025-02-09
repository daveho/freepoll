#include <FL/fl_ask.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Pixmap.H>
#include <iostream>
#include <sstream>
#include <cassert>
#include "datatypes.h"
#include "course.h"
#include "timer.h"
#include "poll.h"
#include "bar_graph_icon.h"
#include "exception.h"
#include "F_async_notification.h"
#include "F_freepoll_window.h"

namespace {

Fl_Pixmap bar_graph_pixmap( bar_graph_icon );

// label text for the poll button
const char *PLAY_LABEL = "@>";
const char *STOP_LABEL = "@square";

}

F_FreePollWindow::F_FreePollWindow( PollModel *model, DataStore *datastore )
  : Fl_Window( WIDTH, HEIGHT_NOGRAPH, "FreePoll " FREEPOLL_VERSION "-fltk" )
  , m_model( model )
  , m_datastore( datastore )
  , m_poll_runner( nullptr )
  , m_pack( 0, 0, WIDTH, HEIGHT )
  , m_course_chooser( 0, 0, WIDTH, COURSE_CHOOSER_HEIGHT )
  , m_controls( 0, 0, WIDTH, CONTROLS_HEIGHT )
  , m_poll_btn( POLL_BTN_X, 0, POLL_BTN_WIDTH, POLL_BTN_HEIGHT )
  , m_timer_display( TIMER_DISPLAY_X, 0, TIMER_DISPLAY_WIDTH, TIMER_DISPLAY_HEIGHT, "" )
  , m_count_display( COUNT_DISPLAY_X, 0, COUNT_DISPLAY_WIDTH, COUNT_DISPLAY_HEIGHT, "" )
  , m_freq_display( FREQ_DISPLAY_X, 0, FREQ_DISPLAY_WIDTH, FREQ_DISPLAY_HEIGHT, "" )
  , m_graph_btn( GRAPH_BTN_X, 0, GRAPH_BTN_WIDTH, GRAPH_BTN_HEIGHT )
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

  m_graph_box.color( 0x0000FF00 );
  m_graph_box.box( FL_FLAT_BOX );

  m_poll_btn.label( PLAY_LABEL );
  m_poll_btn.clear_visible_focus();

  m_timer_display.labelsize( TEXT_SIZE );

  m_count_display.labelsize( TEXT_SIZE );
  //m_count_display.align( FL_ALIGN_RIGHT );

  m_freq_display.labelsize( TEXT_SIZE );

  m_graph_btn.image( &bar_graph_pixmap );
  m_graph_btn.clear_visible_focus();

  // Populate courses in course chooser
  for ( auto course : m_datastore->get_courses() )
    m_course_chooser.add( course->get_display_string().c_str() );
  m_course_chooser.value( 0 );

  // Sync displays with model
  update_timer_display();
  update_count_display();
  update_frequency_display();

  // register callbacks to handle UI events
  m_poll_btn.callback( on_poll_btn_clicked, static_cast<void*>( this ) );
  m_graph_btn.callback( on_graph_button_clicked, static_cast<void*>( this ) );
  m_course_chooser.callback( on_course_change, static_cast<void*>( this ) );

  // Observe the PollModel, Poll, and Timer
  m_model->add_observer( this );
  m_model->get_poll()->add_observer( this );
  m_model->get_timer()->add_observer( this );
}

F_FreePollWindow::~F_FreePollWindow() {
  // unregister from observing model objects
  m_model->get_timer()->remove_observer( this );
  m_model->get_poll()->remove_observer( this );
  m_model->remove_observer( this );
}

void F_FreePollWindow::show( int argc, char **argv ) {
  // Could do application-specific command line argument
  // handling here...
  Fl_Window::show( argc, argv );
}

void F_FreePollWindow::on_update(Observable *observable, int hint, bool is_async) {
  if ( is_async ) {
    // redirect the update to the main GUI thread: it will
    // then be handled as a synchronous update
    F_AsyncNotification *update = new F_AsyncNotification( this, observable, hint );
    Fl::awake( on_async_update, static_cast<void*>( update ) );
    return;
  }

  switch ( hint ) {
  case PollModel::POLL_MODEL_SELECTED_COURSE_CHANGED:
    update_frequency_display();
    break;
  case PollModel::POLL_MODEL_BAR_GRAPH_ENABLEMENT_CHANGED:
    show_or_hide_graph();
    break;
  case Poll::POLL_STARTED:
  case Poll::POLL_RESET:
  case Poll::POLL_RESPONSE_RECORDED:
  case Poll::POLL_STOPPED:
    update_count_display();
    // TODO: should update bar graph
    break;
  case Timer::TIMER_STARTED:
  case Timer::TIMER_NUM_SECONDS_UPDATED:
  case Timer::TIMER_STOPPED:
    update_timer_display();
    break;
  default:
    break;
  }
}

void F_FreePollWindow::on_poll_btn_clicked( Fl_Widget *w, void *data ) {
  F_FreePollWindow *win = static_cast<F_FreePollWindow*>( data );
  PollModel *model = win->m_model;
  Poll *poll = win->m_model->get_poll();
  DataStore *datastore = model->get_datastore();

  if ( !model->is_poll_running() ) {
    // TODO: most of this code should be factored into a member function in PollModel

    assert( win->m_poll_runner == nullptr );

    if ( !model->can_start_poll() )
      return;

    // if a poll was started previously, reset it
    if ( poll->is_started() ) {
      assert( poll->is_stopped() );
      poll->reset();
    }
    
    // disable the course selector
    win->m_course_chooser.deactivate();

    // start the timer
    model->get_timer()->start();

    // update the poll button label
    win->m_poll_btn.label( STOP_LABEL );

    // make sure base station frequency is set correctly
    Course *course = model->get_current_course();
    model->get_base()->set_frequency( course->get_frequency() );

    // create a data directory for the poll results
    // and make a note of it
    std::string poll_data_dir = datastore->create_poll_data_dir( course );
    model->set_poll_data_dir( poll_data_dir );

    // take a screenshot
    datastore->take_screenshot( poll_data_dir );

    // start poll!
    win->m_poll_runner = new PollRunner( model->get_base(), poll );
    win->m_poll_runner->start_poll();

    return;
  }

  if ( model->is_poll_running() ) {
    // TODO: most of this code should be factored into a member function in PollModel

    assert( win->m_poll_runner != nullptr );

    // stop the timer
    model->get_timer()->stop();

    // change the poll button label back to "play"
    win->m_poll_btn.label( PLAY_LABEL );

    // stop the poll
    win->m_poll_runner->stop_poll();
    delete win->m_poll_runner;
    win->m_poll_runner = nullptr;

    // write poll results to files
    std::string poll_data_dir = model->get_poll_data_dir();
    try {
      datastore->write_poll_results( poll_data_dir, poll );
    } catch ( PollException &ex ) {
      // FIXME: should display in GUI somehow
      std::cerr << "Error writing poll data: " << ex.what() << "\n";
    }

    // poll is done
    model->set_poll_data_dir( "" );

    // re-enable course selection
    win->m_course_chooser.activate();
  }
}

void F_FreePollWindow::on_course_change( Fl_Widget *w, void *data ) {
  // Callback for when course is selected in the course chooser
  F_FreePollWindow *win = static_cast<F_FreePollWindow*>( data );
  int course_index = win->m_course_chooser.value();
  win->m_model->set_current_course( unsigned(course_index) );
}

void F_FreePollWindow::on_graph_button_clicked( Fl_Widget *w, void *data ) {
  // Callback for when the bar graph button is clicked
  F_FreePollWindow *win = static_cast<F_FreePollWindow*>( data );
  bool bar_graph_enabled = win->m_model->is_bar_graph_enabled();
  win->m_model->set_bar_graph_enabled( !bar_graph_enabled );
}

void F_FreePollWindow::on_async_update( void *arg ) {
  // This callback is invoked from the FLTK event loop
  // to synchronize handling of asynchronous notifications.
  // I.e., it "launders" notifications originating in
  // other threads to make it appear that they happened in
  // the main GUI thread.
  F_AsyncNotification *update = static_cast<F_AsyncNotification*>( arg );
  update->get_win()->on_update( update->get_observable(), update->get_hint(), false );
  delete update;
}

void F_FreePollWindow::update_timer_display() {
  if ( m_model->is_poll_running() )
    m_timer_display.labelcolor( FL_BLACK );
  else
    m_timer_display.labelcolor( DISABLED_TEXT_COLOR );

  Timer *timer = m_model->get_timer();
  std::string disp_time = timer->get_display_time();
  m_timer_display.copy_label( disp_time.c_str() );
}

void F_FreePollWindow::update_frequency_display() {
  int course_index = m_course_chooser.value();
  const auto &courses = m_datastore->get_courses();
  Course *course = courses.at( course_index );
  Frequency freq = course->get_frequency();
  std::string freq_str = freq.str();
  m_freq_display.copy_label( freq.str().c_str() );
}

void F_FreePollWindow::show_or_hide_graph() {
  bool bar_graph_enabled = m_model->is_bar_graph_enabled();

  if ( !bar_graph_enabled )
    // resize the window to hide the graph
    resize( x(), y(), w(), HEIGHT_NOGRAPH );
  else
    // resize the window to show the graph
    resize( x(), y(), w(), HEIGHT );
}

void F_FreePollWindow::update_count_display() {
  if ( m_model->is_poll_running() )
    m_count_display.labelcolor( FL_BLACK );
  else
    m_count_display.labelcolor( DISABLED_TEXT_COLOR );

  Poll *poll = m_model->get_poll();
  unsigned count = poll->get_num_final_responses();
  std::string count_str = std::to_string( count );
  m_count_display.copy_label( count_str.c_str() );
}
