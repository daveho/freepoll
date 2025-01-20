#include "tui.h"

Widget::Widget()
  : m_width( -1 )
  , m_height( -1 ) {
}

Widget::~Widget() {
}

int Widget::get_width() const {
  return m_width;
}

int Widget::get_height() const {
  return m_height;
}

void Widget::resize( int w, int h ) {
  m_width = w;
  m_height = h;
  on_resize( w, h );
}

bool Widget::allow_focus() const {
  return false;
}

void Widget::on_resize( int w, int h ) {
  // Do nothing: subclasses can override
}

void Widget::on_keypress( int ch ) {
  // Do nothing: subclasses can override
}

void Widget::paint( WINDOW *win ) {
  // Do nothing: subclasses can override
}

Label::Label() {
}

Label::~Label() {
}

void Label::paint( WINDOW *win ) {
  std::string disp = m_text.substr( 0, size_t( get_width() ) );
  wclear( win );
  mvwprintw( win, 0, 0, "%s", disp.c_str() );
}
