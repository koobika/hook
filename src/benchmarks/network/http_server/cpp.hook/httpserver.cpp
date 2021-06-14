// /////////////////////////////////////////////////////////////////////////////
// _____________________________________________________________________________
// examples/network/http_server_hello_world.cpp
// _____________________________________________________________________________
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
// /////////////////////////////////////////////////////////////////////////////

#include "network/protocol/http/server_builder.h"

using namespace koobika::hook::network::protocol::http;

int main() {
  try {
    auto server = ServerBuilder().Build();
    server->Handle("/plaintext", [server](const Request& req, Response& res) {
      res.Headers.Set("Server", "Example");
      res.Headers.Set("Date", server->GetCurrentDate());
      res.Headers.Set("Content-Type", "text/plain; charset=UTF-8");
      res.Body.Write("Hello, World!\r\n");
      res.Ok_200();
    });
    server->Start("8080");
    return getchar();
  } catch (const std::exception& exception) {
    // ((Error)) -> while performing setup!
    std::cout << exception.what() << std::endl;
    return -1;
  }
}