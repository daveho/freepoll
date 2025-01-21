#ifndef TUI_H
#define TUI_H

#include <ncursesw/curses.h>
#include <string>

class TuiEvent {
public:
  enum Type {
    RESIZE,
    FOCUS,
    KEYPRESS,
  };

private:
  Type m_type;
  int m_x, m_y;

public:
  TuiEvent( Type type, int x, int y );
  TuiEvent( const TuiEvent &other );
  ~TuiEvent();

  TuiEvent &operator=( const TuiEvent &rhs );

  int get_x() const;
  int get_y() const;
  int get_width() const;
  int get_height() const;
};

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
