// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 01.hello_world.cpp
// -----------------------------------------------------------------------------
// Copyright (c) 2020 koobika corporation. All rights reserved.
// Author: Marcos Rojas (marcos.rojas@koobika.io)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <iostream>

#include "structured/json/json_value.h"

using namespace koobika::hook::structured::json;

int main() {
  try {
    std::cout << JsonObject({{"hello", "world!"}}).ToString() << std::endl;
  } catch (const std::exception& exception) {
    // [error] -> while performing required operations!
    std::cout << exception.what() << std::endl;
    return -1;
  }
  return 0;
}
