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
// examples/structured/json/json_import.cpp
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

#include <fstream>
#include <chrono>

#include "base/auto_buffer.h"
#include "structured/json/json_importer.h"

using namespace koobika::hook::base;
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
    // Let's parse the above declared json data..
    auto json = JsonImporter::From(json_txt);
    // Let's dump some information..
    std::cout << json.Serialize().ToString() << std::endl;
  } catch (const std::exception& exception) {
    // ((Error)) -> while performing setup!
    std::cout << exception.what() << std::endl;
    return -1;
  }
  return 0;
}
