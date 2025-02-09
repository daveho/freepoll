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
