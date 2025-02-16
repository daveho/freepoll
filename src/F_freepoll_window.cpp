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

#include <FL/fl_ask.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Pixmap.H>
#include <FL/fl_draw.H> // for XWindows headers
#include <iostream>
#include <sstream>
#include <cassert>
#include "datatypes.h"
#include "course.h"
#include "timer.h"
#include "poll.h"
#include "bar_graph_icon.h"
#include "freepoll_window_icon.h"
#include "exception.h"
#include "F_async_notification.h"
#include "F_freepoll_window.h"

namespace {

Fl_Pixmap bar_graph_pixmap( bar_graph_icon );
Fl_Pixmap freepoll_window_icon_pixmap( freepoll_window_icon );

// label text for the poll button
const char *PLAY_LABEL = "@>";
const char *STOP_LABEL = "@square";

// for simplicity, all sizes and positions are hard-coded

const int WIDTH = 360;

const int COURSE_CHOOSER_Y = 0;
const int COURSE_CHOOSER_HEIGHT = 24;
const int CONTROLS_Y = COURSE_CHOOSER_Y + COURSE_CHOOSER_HEIGHT;
const int CONTROLS_HEIGHT = 54;
const int BARGRAPH_Y = CONTROLS_Y + CONTROLS_HEIGHT;
const int BARGRAPH_HEIGHT = 160;

const int HEIGHT = COURSE_CHOOSER_HEIGHT + CONTROLS_HEIGHT + BARGRAPH_HEIGHT;
const int HEIGHT_NOGRAPH = COURSE_CHOOSER_HEIGHT + CONTROLS_HEIGHT;

const int CONTROL_Y = 0;

const int POLL_BTN_X = 0;
const int POLL_BTN_WIDTH = CONTROLS_HEIGHT;
const int POLL_BTN_HEIGHT = POLL_BTN_WIDTH;

const int TIMER_DISPLAY_X = POLL_BTN_X + POLL_BTN_WIDTH;
const int TIMER_DISPLAY_WIDTH = 96;
const int TIMER_DISPLAY_HEIGHT = POLL_BTN_HEIGHT;

const int COUNT_DISPLAY_X = TIMER_DISPLAY_X + TIMER_DISPLAY_WIDTH;
const int COUNT_DISPLAY_WIDTH = 80;
const int COUNT_DISPLAY_HEIGHT = POLL_BTN_HEIGHT;

const int FREQ_DISPLAY_X = COUNT_DISPLAY_X + COUNT_DISPLAY_WIDTH;
const int FREQ_DISPLAY_WIDTH = 64;
const int FREQ_DISPLAY_HEIGHT = POLL_BTN_HEIGHT;

const int TEXT_SIZE = POLL_BTN_HEIGHT - 16;

const int GRAPH_BTN_WIDTH = POLL_BTN_WIDTH;
const int GRAPH_BTN_X = WIDTH - GRAPH_BTN_WIDTH;
const int GRAPH_BTN_HEIGHT = POLL_BTN_WIDTH;

const Fl_Color DISABLED_TEXT_COLOR = 0x80808000;

}

//#define DEBUG( stmt ) do { stmt; } while (0)
#define DEBUG( stmt )

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

  // set window icon
  Fl_RGB_Image img( &freepoll_window_icon_pixmap, FL_WHITE );
  default_icon( &img );

  // register callbacks to handle UI events
  m_poll_btn.callback( on_poll_btn_clicked, static_cast<void*>( this ) );
  m_graph_btn.callback( on_graph_button_clicked, static_cast<void*>( this ) );
  m_course_chooser.callback( on_course_change, static_cast<void*>( this ) );

  // Observe the PollModel (this will indirectly observe the Poll and Timer)
  m_model->add_observer( this );

  // start timer to regularly poll for queued async events
  Fl::add_timeout( 0.1, on_timer_tick, static_cast<void*>( this ) );
}

F_FreePollWindow::~F_FreePollWindow() {
  // unregister from observing the PollModel
  m_model->remove_observer( this );
}

void F_FreePollWindow::show( int argc, char **argv ) {
  // Could do application-specific command line argument
  // handling here...
  Fl_Window::show( argc, argv );

  // The FreePoll window sets itself to be always on top
  // (since it typically the peer instruction questions will be
  // shown using a fullscreen app)
  set_always_on_top();
}

void F_FreePollWindow::on_update(Observable *observable, int hint, bool is_async) {
  if ( is_async ) {
    DEBUG( std::cout << "recevied async update (hint=" << hint << ")\n" );
    F_AsyncNotification *update = new F_AsyncNotification( this, observable, hint );
#if 0
    // redirect the update to the main GUI thread: it will
    // then be handled as a synchronous update
    Fl::awake( on_async_update, static_cast<void*>( update ) );
#endif
    // put async update object in the queue
    m_async_updates.enqueue( update );
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
    update_timer_display();
    m_graph_box.on_update( observable, hint, false );
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

#if 0
void F_FreePollWindow::on_async_update( void *arg ) {
  // This callback is invoked from the FLTK event loop
  // to synchronize handling of asynchronous notifications.
  // I.e., it "launders" notifications originating in
  // other threads to make it appear that they happened in
  // the main GUI thread.
  F_AsyncNotification *update = static_cast<F_AsyncNotification*>( arg );
  std::cout << "handling async update (hint=" << update->get_hint() << ")\n";
  update->get_win()->on_update( update->get_observable(), update->get_hint(), false );
  delete update;
}
#endif

void F_FreePollWindow::on_timer_tick( void *data ) {
  // deal with any accumulated async events
  F_FreePollWindow *win = static_cast< F_FreePollWindow* >( data );
  while ( !win->m_async_updates.empty() ) {
    F_AsyncNotification *update = win->m_async_updates.dequeue();
    win->on_update( update->get_observable(), update->get_hint(), false );
    delete update;
  }
  Fl::repeat_timeout( 0.1, on_timer_tick, data );
}

void F_FreePollWindow::set_always_on_top() {
  // See: https://groups.google.com/g/fltkgeneral/c/Je0bmtom-Og
  XEvent ev;
  static const char* const names[2] = { "_NET_WM_STATE",
                                        "_NET_WM_STATE_ABOVE"
                                      };
  Atom atoms[ 2 ];
  fl_open_display();
  XInternAtoms(fl_display, (char**)names, 2, False, atoms );
  Atom net_wm_state = atoms[ 0 ];
  Atom net_wm_state_above = atoms[ 1 ];
  ev.type = ClientMessage;
  ev.xclient.window = fl_xid(this);
  ev.xclient.message_type = net_wm_state;
  ev.xclient.format = 32;
  ev.xclient.data.l[ 0 ] = (int) true;
  ev.xclient.data.l[ 1 ] = net_wm_state_above;
  ev.xclient.data.l[ 2 ] = 0;
  XSendEvent(fl_display,
              DefaultRootWindow(fl_display),  False,
              SubstructureNotifyMask|SubstructureRedirectMask, &ev);
}

void F_FreePollWindow::update_timer_display() {
  DEBUG( std::cout << "update timer\n" );

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
  m_freq_display.copy_label( freq_str.c_str() );
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
  DEBUG( std::cout << "update count\n" );

  if ( m_model->is_poll_running() )
    m_count_display.labelcolor( FL_BLACK );
  else
    m_count_display.labelcolor( DISABLED_TEXT_COLOR );

  Poll *poll = m_model->get_poll();
  unsigned count = poll->get_num_final_responses();
  std::string count_str = std::to_string( count );
  m_count_display.copy_label( count_str.c_str() );
}
