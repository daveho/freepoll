#ifndef F_FREEPOLL_WINDOW_H
#define F_FREE

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
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
  static const int DEFAULT_WIDTH = 360;
  static const int DEFAULT_HEIGHT = 100;

private:
  Base *m_base;
  DataStore *m_datastore;
  PollModel *m_model;

public:
  F_FreePollWindow( Base *base, DataStore *datastore );
  ~F_FreePollWindow();

  void show( int argc, char **argv );
  virtual void on_update(Observable *observable, int hint);

private:
};

#endif // F_FREEPOLL_WINDOW_H
