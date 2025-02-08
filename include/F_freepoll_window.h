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

// Rather than properly decomposing the GUI into views for
// individual model objects, we just have a single monolithic
// window class that implements the entire GUI and observes
// all model objects.
class F_FreePollWindow : public Fl_Window, public Observer {
public:
  // for simplicity, all sizes and positions are hard-coded

  static const int SPACING = 4;

  static const int WIDTH = 360;

  static const int COURSE_CHOOSER_Y = 0;
  static const int COURSE_CHOOSER_HEIGHT = 32;
  static const int CONTROLS_Y = COURSE_CHOOSER_Y + COURSE_CHOOSER_HEIGHT;
  static const int CONTROLS_HEIGHT = 64;
  static const int BARGRAPH_Y = CONTROLS_Y + CONTROLS_HEIGHT;
  static const int BARGRAPH_HEIGHT = 200;

  static const int HEIGHT = COURSE_CHOOSER_HEIGHT + CONTROLS_HEIGHT + BARGRAPH_HEIGHT;
  static const int HEIGHT_NOGRAPH = COURSE_CHOOSER_HEIGHT + CONTROLS_HEIGHT;

  static const int POLL_BTN_WIDTH = CONTROLS_HEIGHT - SPACING*2;
  static const int POLL_BTN_HEIGHT = POLL_BTN_WIDTH;

  static const int TIMER_DISPLAY_WIDTH = 100;
  static const int TIMER_DISPLAY_HEIGHT = POLL_BTN_HEIGHT;

  static const int COUNT_DISPLAY_WIDTH = 80;
  static const int COUNT_DISPLAY_HEIGHT = POLL_BTN_HEIGHT;

  static const int GAP_WIDTH = 50;

  static const int GRAPH_BTN_WIDTH = POLL_BTN_WIDTH;
  static const int GRAPH_BTN_HEIGHT = POLL_BTN_WIDTH;

  static const Fl_Color DISABLED_TEXT_COLOR = 0x80808000;

private:
  Base *m_base;
  DataStore *m_datastore;
  PollModel *m_model;
  bool m_graph_shown;

  // containers
  Fl_Tile m_tile;
  Fl_Choice m_course_chooser;
  Fl_Pack m_controls;
  Fl_Button m_poll_btn;
  Fl_Box m_timer_display;
  Fl_Box m_count_display;
  Fl_Box m_gap;
  Fl_Button m_graph_btn;
  Fl_End m_controls_end;
  Fl_Box m_graph_box;
  Fl_End m_tile_end;

public:
  F_FreePollWindow( Base *base, DataStore *datastore );
  ~F_FreePollWindow();

  void show( int argc, char **argv );
  virtual void on_update(Observable *observable, int hint);

  static void on_graph_button_clicked( Fl_Widget *w, void *data );

private:
};

#endif // F_FREEPOLL_WINDOW_H
