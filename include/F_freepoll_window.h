#ifndef F_FREEPOLL_WINDOW_H
#define F_FREE

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Tile.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include "observer.h"
#include "base.h"
#include "datastore.h"
#include "poll_model.h"
#include "poll_runner.h"
#include "F_notification_queue.h"

// Rather than properly decomposing the GUI into views for
// individual model objects, we just have a single monolithic
// window class that implements the entire GUI and observes
// all model objects.
class F_FreePollWindow : public Fl_Window, public Observer {
public:
  // for simplicity, all sizes and positions are hard-coded

  static const int WIDTH = 360;

  static const int COURSE_CHOOSER_Y = 0;
  static const int COURSE_CHOOSER_HEIGHT = 24;
  static const int CONTROLS_Y = COURSE_CHOOSER_Y + COURSE_CHOOSER_HEIGHT;
  static const int CONTROLS_HEIGHT = 54;
  static const int BARGRAPH_Y = CONTROLS_Y + CONTROLS_HEIGHT;
  static const int BARGRAPH_HEIGHT = 140;

  static const int HEIGHT = COURSE_CHOOSER_HEIGHT + CONTROLS_HEIGHT + BARGRAPH_HEIGHT;
  static const int HEIGHT_NOGRAPH = COURSE_CHOOSER_HEIGHT + CONTROLS_HEIGHT;

  static const int CONTROL_Y = 0;

  static const int POLL_BTN_X = 0;
  static const int POLL_BTN_WIDTH = CONTROLS_HEIGHT;
  static const int POLL_BTN_HEIGHT = POLL_BTN_WIDTH;

  static const int TIMER_DISPLAY_X = POLL_BTN_X + POLL_BTN_WIDTH;
  static const int TIMER_DISPLAY_WIDTH = 96;
  static const int TIMER_DISPLAY_HEIGHT = POLL_BTN_HEIGHT;

  static const int COUNT_DISPLAY_X = TIMER_DISPLAY_X + TIMER_DISPLAY_WIDTH;
  static const int COUNT_DISPLAY_WIDTH = 80;
  static const int COUNT_DISPLAY_HEIGHT = POLL_BTN_HEIGHT;

  static const int FREQ_DISPLAY_X = COUNT_DISPLAY_X + COUNT_DISPLAY_WIDTH;
  static const int FREQ_DISPLAY_WIDTH = 64;
  static const int FREQ_DISPLAY_HEIGHT = POLL_BTN_HEIGHT;

  static const int TEXT_SIZE = POLL_BTN_HEIGHT - 16;

  static const int GRAPH_BTN_WIDTH = POLL_BTN_WIDTH;
  static const int GRAPH_BTN_X = WIDTH - GRAPH_BTN_WIDTH;
  static const int GRAPH_BTN_HEIGHT = POLL_BTN_WIDTH;

  static const Fl_Color DISABLED_TEXT_COLOR = 0x80808000;

private:
  PollModel *m_model;
  DataStore *m_datastore;
  PollRunner *m_poll_runner;
  F_NotificationQueue m_async_updates;

  // containers
  Fl_Pack m_pack; // overall container for window contents

  Fl_Choice m_course_chooser;
  
  Fl_Group m_controls;
  Fl_Button m_poll_btn;
  Fl_Box m_timer_display;
  Fl_Box m_count_display;
  Fl_Box m_freq_display;
  Fl_Button m_graph_btn;
  Fl_End m_controls_end;

  Fl_Box m_graph_box;

  Fl_End m_pack_end;

public:
  F_FreePollWindow( PollModel *model, DataStore *datastore );
  ~F_FreePollWindow();

  void show( int argc, char **argv );

  // called when an observed model object is updated
  virtual void on_update(Observable *observable, int hint, bool is_async);

  // UI event handlers
  static void on_poll_btn_clicked( Fl_Widget *w, void *data );
  static void on_course_change( Fl_Widget *w, void *data );
  static void on_graph_button_clicked( Fl_Widget *w, void *data );
#if 0
  // handler for async updates
  static void on_async_update( void *arg );
#endif

  // timer callback so that we can deal with queued async events
  static void on_timer_tick( void *data );

private:
  // update display elements (these should generally be called
  // in response to UI events)
  void update_timer_display();
  void update_frequency_display();
  void show_or_hide_graph();
  void update_count_display();
};

#endif // F_FREEPOLL_WINDOW_H
