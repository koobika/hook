// /////////////////////////////////////////////////////////////////////////////
//
//       ╓▄▓▓▓▓▓▓▓▄╖      ╓▄▓▓▓▓▓▓▓▄╖
//    ╓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓╖╓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓w
//  ,▓▓▓▓▓▓▓▓▀▀▀▀▓▓▓▓▓▓▓▓▓▓▓▓▓▀▀▀▀▓▓▓▓▓▓▓,
//  ▓▓▓▓▓▓`       `▓▓▓▓▓▓▓▓`        ▓▓▓▓▓▓
// ╫▓▓▓▓▓           ▓▓▓▓▓▓           ▓▓▓▓▓▓
// ▓▓▓▓▓▓           ▓▓▓▓▓▓           ╟▓▓▓▓▓
// ╙▓▓▓▓▓▄         ╓▓▓▓▓▓╛          ╓▓▓▓▓▓▌
//  ▀▓▓▓▓▓▓æ,   ,g▓▓▓▓▓▓▀   ,,,  ,g▓▓▓▓▓▓▌
//   '▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓`  ╒▓▓▓▓▓▓▓▓▓▓▓▓▓'
//      ▀▓▓▓▓▓▓▓▓▓▓▓▀`     ▓▓▓▓▓▓▓▓▓▓▀`
//          `"""`            `"""`
// -----------------------------------------------------------------------------
// examples/structured/json/json_objects.cpp
// -----------------------------------------------------------------------------
// Copyright (c) 2021 koobika corporation. All rights reserved.
// Author: Marcos Rojas (mrojas@koobika.org).
// -----------------------------------------------------------------------------
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

#include "structured/json/json_value.h"

using namespace koobika::hook::structured::json;

int main() {
  try {
    // implicit declaration..
    JsonObject json_object = {{"a", "Hello, World!"}};
    // add element..
    json_object.Add({"b", true});
    json_object.Add({"c", false});
    json_object.Add({"d", nullptr});
    json_object.Add({"e", std::numeric_limits<unsigned char>::max()});
    json_object.Add({"f", std::numeric_limits<char>::max()});
    json_object.Add({"g", std::numeric_limits<unsigned short>::max()});
    json_object.Add({"h", std::numeric_limits<short>::max()});
    json_object.Add({"i", std::numeric_limits<unsigned int>::max()});
    json_object.Add({"j", std::numeric_limits<int>::max()});
    json_object.Add({"k", std::numeric_limits<unsigned long>::max()});
    json_object.Add({"l", std::numeric_limits<long>::max()});
    json_object.Add({"m", std::numeric_limits<float>::max()});
    json_object.Add({"n", std::numeric_limits<double>::max()});
    // check for an existent key..
    auto exists = json_object.Exist("c");
    // get element (by value)..
    auto by_value = json_object.At("g");
    // get element (by reference)..
    auto& by_reference = json_object.At("g");
    by_reference = "changed by reference using method!";
    // get element (by value) using operator..
    auto by_value_op = json_object["m"];
    // get element (by reference) using operator..
    auto& by_reference_op = json_object["m"];
    by_reference_op = "changed by reference using operator!";
    // explicit declaration (with conversion)..
    JsonValue json_object_aux = JsonObject{{"This is also me!", true}};
    // explicit declaration (with conversion)..
    JsonObject json_object_copy = json_object;
    // erase element..
    json_object_copy.Erase("i");
    // Let's iterate through the elements (by const ref)!
    std::cout << "> json_array elements (using iteration): " << std::endl;
    for (auto const& element : json_object) {
      std::cout << "    " << element.first << ": "
                << element.second.Serialize().ToString() << std::endl;
    }
    // Let's iterate through the elements (by ref) to change them!
    for (auto& element : json_object_copy) {
      element.second = nullptr;
    }
    // let's try now to dump (export to string) the json-array's content..
    std::cout << "> json_object_aux: " << json_object_aux.Serialize().ToString()
              << std::endl;
    std::cout << "> json_object_copy: "
              << json_object_copy.Serialize().ToString() << std::endl;
  } catch (const std::exception& exception) {
    // [error] -> while performing required operations!
    std::cout << exception.what() << std::endl;
    return -1;
  }
  return 0;
}
