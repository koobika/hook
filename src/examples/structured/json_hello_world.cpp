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
    json::Value json_null = nullptr;
    json::Value json_false = false;
    json::Value json_true = true;
    json::Value json_unsigned_char = std::numeric_limits<unsigned char>::max();
    json::Value json_char = std::numeric_limits<char>::max();
    json::Value json_unsigned_short =
        std::numeric_limits<unsigned short>::max();
    json::Value json_short = std::numeric_limits<short>::max();
    json::Value json_unsigned_int = std::numeric_limits<unsigned int>::max();
    json::Value json_int = std::numeric_limits<int>::max();
    json::Value json_unsigned_long = std::numeric_limits<unsigned long>::max();
    json::Value json_long = std::numeric_limits<long>::max();
    json::Value json_float = std::numeric_limits<float>::max();
    json::Value json_double = std::numeric_limits<double>::max();
    json::Value json_string = "koobika";
    json::Value json_array = json::Array{
        json_null,   json_false,          json_true,  json_unsigned_char,
        json_char,   json_unsigned_short, json_short, json_unsigned_int,
        json_int,    json_unsigned_long,  json_long,  json_float,
        json_double, json_string};
    json::Value json_nested_array = json::Array{json_array};
    json::Value json_object = json::Object{{"MyObject", json_nested_array}};
    json::Value json_nested_object =
        json::Object{{"MyNestedObject", json_object}};
    std::string content;
    auto serialized = json_nested_object.Serialize();
    serialized.ReadAll(content);
    std::cout << content << std::endl;
  } catch (const std::exception& exception) {
    // [error] -> while performing required operations!
    std::cout << exception.what() << std::endl;
    return -1;
  }
  return 0;
}
