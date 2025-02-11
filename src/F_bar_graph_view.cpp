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

#include <FL/fl_draw.H>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "poll_model.h"
#include "poll.h"
#include "F_bar_graph_view.h"

namespace {

const int INSET = 8;

const Fl_Color BAR_COLORS[] = {
  0x0000FF00,
  0xFFA50000,
  0x00800000,
  0xA020F000,
  0xFF000000,
};

const char *LABELS[] = {
  "A", "B", "C", "D", "E",
};

const int LABEL_HEIGHT = 16;

}

//#define DEBUG( stmt ) do { stmt } while (0)
#define DEBUG( stmt )

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

  DEBUG( std::cout << "draw bar graph "
    << INSET << "," << INSET << ","
    << effective_width << "," << effective_height
    << "\n" );


  // draw graph background
  fl_draw_box( FL_FLAT_BOX, xpos + INSET, ypos + INSET, effective_width, effective_height, FL_WHITE );

  // how wide the bars will be
  // (with an INSET gap between each, and an INSET
  // gap on the left and right)
  int bar_width = (effective_width - INSET*6) / 5;

  // starting x offset of the first bar
  int x_off = ( effective_width - ( bar_width*5 + INSET * 4 ) ) / 2;

  // draw labels
  auto font = FL_HELVETICA;
  auto font_size = Fl_Fontsize( 20 );
  for ( int i = 0; i < 5; ++i ) {
    int label_x = xpos + INSET + x_off + i*(bar_width + INSET) + (bar_width/2 - 5);

    fl_font( font, font_size );
    fl_color( FL_BLACK );
    fl_draw( LABELS[i], label_x, ypos + effective_height );
  }

  // if there is no data, don't draw bars
  if ( m_cur_responses.empty() )
    return;

  // tally responses
  double count[5] = { 0.0 };
  for ( const auto &pair : m_cur_responses )
    count[int(pair.second)] += 1.0;
  
  // normalize counts to range 0.0-1.0
  // (so that each is proportion of overall responses)
  for ( int i = 0; i < 5; ++i )
    count[i] /= double( m_cur_responses.size() );

  // re-normalize so that max=1 for computation of bar height
  auto j = std::max_element( count, count + 5 );
  double maxval = *j;
  for ( int i = 0; i < 5; ++i )
    count[i] = count[i] / maxval;
  
  int bar_max_height = effective_height - LABEL_HEIGHT - INSET*2;
  
  // draw bars
  for ( int i = 0; i < 5; ++i ) {
    int bar_x = xpos + INSET + x_off + i*(bar_width + INSET);
    int bar_actual_height = int( ::round( bar_max_height * count[i] ) );
    int bar_y = ypos + effective_height - LABEL_HEIGHT - bar_actual_height;
    fl_draw_box( FL_FLAT_BOX, bar_x, bar_y, bar_width, bar_actual_height, BAR_COLORS[i] );

    DEBUG( std::cout << "draw bar " << i << ": "
      << bar_x << "," << bar_y << "," << bar_width << "," << bar_actual_height << "\n" );
  }
}

void F_BarGraphView::on_update(Observable *observable, int hint, bool is_async) {
  // all notifications will come from the PollModel
  PollModel *model = dynamic_cast<PollModel*>( observable );
  m_cur_responses = model->get_poll()->get_final_responses();
  redraw();
}
