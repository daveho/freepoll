#include "exception.h"
#include "base.h"

namespace {

const unsigned VENDOR_ID = 0x1881;
const unsigned PRODUCT_ID = 0x0150;

} // end anonymous namespace

Base::Base()
  : m_initialized(false)
  , m_dev(nullptr) {
}

Base::~Base() {
  if (m_dev != nullptr) {
    hid_close(m_dev);
  }

  if (m_initialized) {
    hid_exit();
  }
}

void Base::initialize() {
  if (hid_init() != 0) {
    throw PollException("hid_init() failed");
  }

  m_initialized = true;

  m_dev = hid_open(VENDOR_ID, PRODUCT_ID, nullptr);
  if (m_dev == nullptr) {
    throw PollException("could not open base station HID device");
  }
}
