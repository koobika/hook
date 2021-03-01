// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 05.import.cpp
// -----------------------------------------------------------------------------
// Copyright (c) 2020 koobika corporation. All rights reserved.
// Author: Marcos Rojas (marcos.rojas@koobika.io)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <iostream>

#include "structured/json/json_value.h"

using namespace koobika::hook::structured::json;

int main() {
  try {
    auto json_txt =
        "  {"
        "  \"Wakes\" : ["
        "    {"
        "      \"Mac\" : \"40:16:7E:AD:3B:96\","
        "      \"Broadcast\" : \"10.10.1.255\","
        "      \"Packet\" : 9,"
        "      \"Aliases\" : [ \"Lienzo\", \"Lizo\" ]"
        "    },"
        "    {"
        "      \"Mac\" : \"B0:6E:BF:CD:86:29\","
        "      \"Broadcast\" : \"10.10.1.255\","
        "      \"Packet\" : 9,"
        "      \"Aliases\" : [ \"Luciernaga\", \"Lucena\", \"Luciernagas\" ]"
        "    }"
        "  ]"
        "}";
    // straightforward import..
    auto json = JsonValue::From(json_txt);
    // let's try now to dump (export to string) the entire json content..
    std::cout << json->ToString() << std::endl;
  } catch (const std::exception& exception) {
    // [error] -> while performing required operations!
    std::cout << exception.what() << std::endl;
    return -1;
  }
  return 0;
}
