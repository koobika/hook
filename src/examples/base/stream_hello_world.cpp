// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// stream_hello_world.cpp
// -----------------------------------------------------------------------------
// Copyright (c) 2020 koobika corporation. All rights reserved.
// Author: Marcos Rojas (marcos.rojas@koobika.io)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <memory>
#include <iostream>

#include "base/stream.h"

using namespace koobika::hook::base;

int main() {
  try {
    Stream stream, stream2;
    stream.Write("Hello hook::base::Stream World!");
    stream2.Write(stream);
    int a = 0x0;
  } catch (const std::exception& exception) {
    // [error] -> while performing required operations!
    std::cout << exception.what() << std::endl;
    return -1;
  }
  return 0;
}
