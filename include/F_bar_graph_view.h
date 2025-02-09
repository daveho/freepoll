#ifndef F_BAR_GRAPH_VIEW_H
#define F_BAR_GRAPH_VIEW_H

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include "observer.h"
#include "poll.h"

class F_BarGraphView : public Fl_Box, public Observer {
private:
  std::map<RemoteID, Option> m_cur_responses;

public:
  F_BarGraphView( int x, int y, int w, int h );
  ~F_BarGraphView();

  void draw() override;

  virtual void on_update(Observable *observable, int hint, bool is_async);
};

#endif // F_BAR_GRAPH_VIEW_H
