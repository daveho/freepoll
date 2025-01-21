#include <cassert>
#include "utf8.h"
#include "tui.h"

namespace {

// Trim a utf-8 string so that is has (at most) a specified number
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

TuiEvent::TuiEvent( Type type, int x, int y )
  : m_type( type )
  , m_x( x )
  , m_y( y ) {

}

TuiEvent::TuiEvent( const TuiEvent &other )
  : m_type( other.m_type )
  , m_x( other.m_x )
  , m_y( other.m_y ) {
}

TuiEvent::~TuiEvent() {
}

TuiEvent &TuiEvent::operator=( const TuiEvent &rhs ) {
  if ( this != &rhs ) {
    m_type = rhs.m_type;
    m_x = rhs.m_x;
    m_y = rhs.m_y;
  }
  return *this;
}

TuiEvent::Type TuiEvent::get_type() const {
  return m_type;
}

int TuiEvent::get_x() const {
  return m_x;
}

int TuiEvent::get_y() const {
  return m_y;
}

int TuiEvent::get_width() const {
  assert( m_type == RESIZE );
  return m_x;
}

int TuiEvent::get_height() const {
  assert( m_type == RESIZE );
  return m_y;
}

int TuiEvent::get_keypress() const {
  assert( m_type == KEYPRESS );
  return m_x;
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
  on_resize( TuiEvent( TuiEvent::RESIZE, w, h) );
}

bool Widget::allow_focus() const {
  return false;
}

void Widget::on_resize( const TuiEvent &resize_evt ) {
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
