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
  static const int COURSE_CHOOSER_HEIGHT = 24;
  static const int CONTROLS_Y = COURSE_CHOOSER_Y + COURSE_CHOOSER_HEIGHT;
  static const int CONTROLS_HEIGHT = 54;
  static const int BARGRAPH_Y = CONTROLS_Y + CONTROLS_HEIGHT;
  static const int BARGRAPH_HEIGHT = 140;

  static const int HEIGHT = COURSE_CHOOSER_HEIGHT + CONTROLS_HEIGHT + BARGRAPH_HEIGHT + SPACING*2;
  static const int HEIGHT_NOGRAPH = COURSE_CHOOSER_HEIGHT + CONTROLS_HEIGHT + SPACING;

  static const int CONTROL_Y = SPACING;

  static const int POLL_BTN_X = 0;
  static const int POLL_BTN_WIDTH = CONTROLS_HEIGHT - SPACING*2;
  static const int POLL_BTN_HEIGHT = POLL_BTN_WIDTH;

  static const int TIMER_DISPLAY_X = POLL_BTN_X + POLL_BTN_WIDTH + SPACING;
  static const int TIMER_DISPLAY_WIDTH = 96;
  static const int TIMER_DISPLAY_HEIGHT = POLL_BTN_HEIGHT;

  static const int COUNT_DISPLAY_X = TIMER_DISPLAY_X + TIMER_DISPLAY_WIDTH + SPACING;
  static const int COUNT_DISPLAY_WIDTH = 80;
  static const int COUNT_DISPLAY_HEIGHT = POLL_BTN_HEIGHT;

  static const int FREQ_DISPLAY_X = COUNT_DISPLAY_X + COUNT_DISPLAY_WIDTH + SPACING;
  static const int FREQ_DISPLAY_WIDTH = 64;
  static const int FREQ_DISPLAY_HEIGHT = POLL_BTN_HEIGHT;

  static const int TEXT_SIZE = POLL_BTN_HEIGHT - 10;

  static const int GRAPH_BTN_WIDTH = POLL_BTN_WIDTH;
  static const int GRAPH_BTN_X = WIDTH - GRAPH_BTN_WIDTH;
  static const int GRAPH_BTN_HEIGHT = POLL_BTN_WIDTH;

  static const Fl_Color DISABLED_TEXT_COLOR = 0x80808000;

private:
  PollModel *m_model;
  DataStore *m_datastore;
  bool m_graph_shown;

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
  virtual void on_update(Observable *observable, int hint);

  static void on_course_change( Fl_Widget *w, void *data );
  static void on_graph_button_clicked( Fl_Widget *w, void *data );

private:
  void update_timer_display();
  void update_frequency_display();
};

#endif // F_FREEPOLL_WINDOW_H
