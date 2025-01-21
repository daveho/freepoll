#include "utf8.h"
#include "tui.h"

namespace {

// Trim a utf-8 string so that is has a specified number
// of characters. Note that this ignores the possibility
// of a character requiring a double-width glyph.
std::string utf8_trim( const std::string &s, int len ) {
  if ( len <= 0 )
    return "";

  unsigned sz = unsigned(len), count = 0;
  auto i = s.begin();

  std::string result;

  while ( i != s.end() && count < sz ) {
    try {
      int cp = utf8::next( i, s.end() );
      ++count;
      utf8::append( cp, result );
    } catch ( utf8::invalid_utf8 &ex ) {
      break;
    }
  }

  return result;
}

}

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
  std::string disp = utf8_trim( m_text, get_width() );
  wclear( win );
  mvwprintw( win, 0, 0, "%s", disp.c_str() );
}
