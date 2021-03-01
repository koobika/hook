// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 01.hello_world.cpp
// -----------------------------------------------------------------------------
// Copyright (c) 2020 koobika corporation. All rights reserved.
// Author: Marcos Rojas (marcos.rojas@koobika.io)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <iostream>

#include "encoding/base64/decoder.h"

using namespace koobika::hook::base;
using namespace koobika::hook::encoding;

int main() {
  try {
    std::cout
        << base64::Decoder::Decode(
               "SW4gcHJvZ3JhbW1pbmcsIEJhc2U2NCBpcyBhIGdyb3VwIG9mIGJpbmFyeS10by1"
               "0ZXh0IGVuY29kaW5nIHNjaGVtZXMgdGhhdCByZXByZXNlbnQgYmluYXJ5IGRhdG"
               "EgKG1vcmUgc3BlY2lmaWNhbGx5IGEgc2VxdWVuY2Ugb2YgOC1iaXQgYnl0ZXMpI"
               "GluIGFuIEFTQ0lJIHN0cmluZyBmb3JtYXQgYnkgdHJhbnNsYXRpbmcgaXQgaW50"
               "byBhIHJhZGl4LTY0IHJlcHJlc2VudGF0aW9uLiBUaGUgdGVybSBCYXNlNjQgb3J"
               "pZ2luYXRlcyBmcm9tIGEgc3BlY2lmaWMgTUlNRSBjb250ZW50IHRyYW5zZmVyIG"
               "VuY29kaW5nLiBFYWNoIG5vbi1maW5hbCBCYXNlNjQgZGlnaXQgcmVwcmVzZW50c"
               "yBleGFjdGx5IDYgYml0cyBvZiBkYXRhLiBUaHJlZSA4LWJpdCBieXRlcyAoaS5l"
               "LiwgYSB0b3RhbCBvZiAyNCBiaXRzKSBjYW4gdGhlcmVmb3JlIGJlIHJlcHJlc2V"
               "udGVkIGJ5IGZvdXIgNi1iaXQgQmFzZTY0IGRpZ2l0cy4=")
               .ToStringView()
        << std::endl;
  } catch (const std::exception& exception) {
    // [error] -> while performing required operations!
    std::cout << exception.what() << std::endl;
    return -1;
  }
  return 0;
}
