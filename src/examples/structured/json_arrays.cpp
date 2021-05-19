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
// examples/structured/json/json_arrays.cpp
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
    JsonArray json_array = {"Hello, World!", false, true, nullptr};
    // add element..
    json_array.Add(true);
    json_array.Add(false);
    json_array.Add(nullptr);
    json_array.Add(std::numeric_limits<unsigned char>::max());
    json_array.Add(std::numeric_limits<char>::max());
    json_array.Add(std::numeric_limits<unsigned short>::max());
    json_array.Add(std::numeric_limits<short>::max());
    json_array.Add(std::numeric_limits<unsigned int>::max());
    json_array.Add(std::numeric_limits<int>::max());
    json_array.Add(std::numeric_limits<unsigned long>::max());
    json_array.Add(std::numeric_limits<long>::max());
    json_array.Add(std::numeric_limits<float>::max());
    json_array.Add(std::numeric_limits<double>::max());
    json_array.Add("Hello, Array World!");
    // get element (by value)..
    auto by_value = json_array.At(0);
    // get element (by reference)..
    auto& by_reference = json_array.At(0);
    by_reference = "changed by reference using method!";
    // get element (by value) using operator..
    auto by_value_op = json_array[1];
    // get element (by reference) using operator..
    auto& by_reference_op = json_array[1];
    by_reference_op = "changed by reference using operator!";
    // explicit declaration (with conversion)..
    JsonValue json_array_aux = JsonArray{"This is also me!", true};
    // explicit declaration (with conversion)..
    JsonArray json_array_copy = json_array;
    // erase element..
    json_array_copy.Erase(0);
    // erase range (initial/last)..
    json_array_copy.Erase(0, 16);
    // Let's iterate through the elements (by const ref)!
    std::cout << "> json_array elements (using iteration): " << std::endl;
    for (auto const& element : json_array) {
      std::cout << "    " << element.Serialize().ToString() << std::endl;
    }
    // Let's iterate through the elements (by ref) to change them!
    for (auto& element : json_array_copy) {
      element = nullptr;
    }
    // let's try now to dump (export to string) the json-array's content..
    std::cout << "> json_array_aux: " << json_array_aux.Serialize().ToString()
              << std::endl;
    std::cout << "> json_array_copy: " << json_array_copy.Serialize().ToString()
              << std::endl;
  } catch (const std::exception& exception) {
    // ((Error)) -> while performing setup!
    std::cout << exception.what() << std::endl;
    return -1;
  }
  return 0;
}
