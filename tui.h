#ifndef TUI_H
#define TUI_H

#include <ncursesw/curses.h>
#include <string>

class Widget {
private:
  int m_width, m_height;

  // no value semantics
  Widget( const Widget & );
  Widget &operator=( const Widget & );

public:
  Widget();
  virtual ~Widget();

  int get_width() const;
  int get_height() const;

  void resize( int w, int h );

  virtual bool allow_focus() const;
  virtual void on_resize( int w, int h );
  virtual void on_keypress( int ch );
  virtual void paint( WINDOW *win );
};

class Label : public Widget {
private:
  std::string m_text;

  // no value semantics
  Label( const Label & );
  Label &operator=( const Label & );

public:
  Label();
  virtual ~Label();

  virtual void paint( WINDOW *win );
};

#endif // TUI_H
