#include <FL/fl_draw.H>
#include <iostream>
#include <cmath>
#include "poll_model.h"
#include "poll.h"
#include "F_bar_graph_view.h"

namespace {

const int INSET = 4;

const Fl_Color BAR_COLORS[] = {
  0x0000FF00,
  0xFFA50000,
  0x00800000,
  0xA020F000,
  0xFF000000,
};

const int LABEL_HEIGHT = 16;

}

F_BarGraphView::F_BarGraphView( int x, int y, int w, int h )
  : Fl_Box( x, y, w, h ) {

}

F_BarGraphView::~F_BarGraphView() {

}

void F_BarGraphView::draw() {
  Fl_Box::draw();

  int xpos = x();
  int ypos = y();

  int width = w();
  int effective_width = width - INSET*2;
  int height = h();
  int effective_height = height - INSET*2;
/*
  std::cout << "draw bar graph "
    << INSET << "," << INSET << ","
    << effective_width << "," << effective_height
    << "\n";
*/

  // draw graph background
  fl_draw_box( FL_FLAT_BOX, xpos + INSET, ypos + INSET, effective_width, effective_height, FL_WHITE );

  // TODO: draw labels

  // if there is no data, don't draw bars
  if ( m_cur_responses.empty() )
    return;

  // how wide the bars will be
  // (with an INSET gap between each, and an INSET
  // gap on the left and right)
  int bar_width = (effective_width - INSET*6) / 5;

  // starting x offset of the first bar
  int x_off = ( effective_width - ( bar_width*5 + INSET * 4 ) ) / 2;

  // tally responses
  double count[5] = { 0.0 };
  for ( const auto &pair : m_cur_responses )
    count[int(pair.second)] += 1.0;
  
  // normalize counts to range 0.0-1.0
  for ( int i = 0; i < 5; ++i )
    count[i] /= double( m_cur_responses.size() );
  
  int bar_max_height = effective_height - LABEL_HEIGHT - INSET;
  
  // draw bars
  for ( int i = 0; i < 5; ++i ) {
    int bar_x = xpos + INSET + x_off + i*(bar_width + INSET);
    int bar_actual_height = int( ::round( bar_max_height * count[i] ) );
    int bar_y = ypos + INSET + effective_height - INSET - LABEL_HEIGHT;
    fl_draw_box( FL_FLAT_BOX, bar_x, bar_y, bar_width, bar_actual_height, BAR_COLORS[i] );
  }
}

void F_BarGraphView::on_update(Observable *observable, int hint, bool is_async) {
  // all notifications will come from the PollModel
  PollModel *model = dynamic_cast<PollModel*>( observable );
  m_cur_responses = model->get_poll()->get_final_responses();
  redraw();
}
