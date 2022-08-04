#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include "base.h"

std::string hexstr(unsigned val) {
  std::stringstream ss;
  ss << std::hex << val;
  return ss.str();
}

int main() {
#if 0
  // Some of this code is adapted from the hidapi test program,
  // https://github.com/libusb/hidapi/blob/master/hidtest/test.c

  struct hid_device_info *devs, *cur_dev;

  if (hid_init() != 0) {
    std::cerr << "hid_init() failed\n";
    return 1;
  }

  devs = hid_enumerate(0x0, 0x0);
  cur_dev = devs;

  while (cur_dev != nullptr) {
    std::cout << "Device found: type=" << hexstr(cur_dev->vendor_id)
              << " " << hexstr(cur_dev->product_id)
              << ", path=" << cur_dev->path
              << "\n";
    cur_dev = cur_dev->next;
  }

  hid_free_enumeration(devs);
#endif

  std::unique_ptr<Base> base(new Base());
  base->initialize();

  std::cout << "initialized base?\n";

  return 0;
}
