// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 02.basic_types.cpp
// -----------------------------------------------------------------------------
// Copyright (c) 2020 koobika corporation. All rights reserved.
// Author: Marcos Rojas (marcos.rojas@koobika.io)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <vector>
#include <iostream>

#include "structured/json/json_value.h"

using namespace koobika::hook::structured::json;

int main() {
  std::vector<JsonValue> json_values;
  try {
    // here you have different ways to declare a <JsonNull> object..
    json_values.push_back(JsonNull());
    json_values.push_back(nullptr);
    // here you have different ways to declare a <JsonTrue> object..
    json_values.push_back(JsonTrue());
    json_values.push_back(true);
    // here you have different ways to declare a <JsonFalse> object..
    json_values.push_back(JsonFalse());
    json_values.push_back(false);
    // here you have different ways to declare a <JsonNumber> object..
    json_values.push_back(
        (unsigned char)std::numeric_limits<unsigned char>::max());
    json_values.push_back((char)std::numeric_limits<char>::min());
    json_values.push_back(
        (unsigned short)std::numeric_limits<unsigned short>::max());
    json_values.push_back((short)std::numeric_limits<short>::min());
    json_values.push_back(
        (unsigned int)std::numeric_limits<unsigned int>::max());
    json_values.push_back((int)std::numeric_limits<int>::min());
    json_values.push_back(
        (unsigned long)std::numeric_limits<unsigned long>::max());
    json_values.push_back((long)std::numeric_limits<long>::min());
    json_values.push_back((float)std::numeric_limits<float>::max());
    json_values.push_back((double)std::numeric_limits<double>::max());
    // here you have different ways to declare a <JsonString> object..
    json_values.push_back("hello world!");
    // let's perform a dump of all the previously declared objects..
    for (auto const& json_value : json_values) {
      std::cout << json_value.ToString() << std::endl;
    }
  } catch (const std::exception& exception) {
    // [error] -> while performing required operations!
    std::cout << exception.what() << std::endl;
    return -1;
  }
  return 0;
}
