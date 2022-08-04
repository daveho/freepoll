#ifndef BASE_H
#define BASE_H

#include <hidapi.h>

// Encapsulation of iClicker base station

class Base {
private:
  bool m_initialized;
  hid_device *m_dev;

  // value semantics are not allowed
  Base(const Base &);
  Base &operator=(const Base &);

public:
  Base();
  ~Base();

  void initialize();
};

#endif // BASE_H
