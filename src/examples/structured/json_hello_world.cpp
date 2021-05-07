// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 01.hello_world.cpp
// -----------------------------------------------------------------------------
// Copyright (c) 2020 koobika corporation. All rights reserved.
// Author: Marcos Rojas (marcos.rojas@koobika.io)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <iostream>
#include <limits>

#include "structured/json/value.h"

using namespace koobika::hook::structured;

int main() {
  try {
    // Let's create some dummy json values/arrays/objects..
    json::Value json_string = "Hello, World!";
    json::Value json_number = 2021;
    json::Array json_array = {json_string, json_number};
    json::Object json_object = {{"hook", json_array}};
    json::Object json_tmp = {{"hooka", json_array}};
    // Let's dump the json 'object' content to string..
    std::string content;
    json_object.Serialize().ReadAll(content);
    std::cout << content << std::endl;
  } catch (const std::exception& exception) {
    // ((Error)) -> while performing required operations!
    std::cout << exception.what() << std::endl;
    return -1;
  }
  return 0;
}
