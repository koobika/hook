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
// examples/structured/json/json_basic_types.cpp
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
    // Let's create some basic json values..
    JsonValue json_true = true;
    JsonValue json_false = false;
    JsonValue json_null = nullptr;
    JsonValue json_unsigned_char = std::numeric_limits<unsigned char>::max();
    JsonValue json_char = std::numeric_limits<char>::max();
    JsonValue json_unsigned_short = std::numeric_limits<unsigned short>::max();
    JsonValue json_short = std::numeric_limits<short>::max();
    JsonValue json_unsigned_int = std::numeric_limits<unsigned int>::max();
    JsonValue json_int = std::numeric_limits<int>::max();
    JsonValue json_unsigned_long = std::numeric_limits<unsigned long>::max();
    JsonValue json_long = std::numeric_limits<long>::max();
    JsonValue json_float = std::numeric_limits<float>::max();
    JsonValue json_double = std::numeric_limits<double>::max();
    // Let's dump serialized content..
    std::cout << json_true.Serialize().ToString() << std::endl
              << json_false.Serialize().ToString() << std::endl
              << json_null.Serialize().ToString() << std::endl
              << json_unsigned_char.Serialize().ToString() << std::endl
              << json_char.Serialize().ToString() << std::endl
              << json_unsigned_short.Serialize().ToString() << std::endl
              << json_short.Serialize().ToString() << std::endl
              << json_unsigned_int.Serialize().ToString() << std::endl
              << json_int.Serialize().ToString() << std::endl
              << json_unsigned_long.Serialize().ToString() << std::endl
              << json_long.Serialize().ToString() << std::endl
              << json_float.Serialize().ToString() << std::endl
              << json_double.Serialize().ToString() << std::endl;
  } catch (const std::exception& exception) {
    // [error] -> while performing required operations!
    std::cout << exception.what() << std::endl;
    return -1;
  }
  return 0;
}
