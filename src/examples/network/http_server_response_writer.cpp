﻿// /////////////////////////////////////////////////////////////////////////////
//   ██░ ██  ▒█████   ▒█████   ██ ▄█▀
//  ▓██░ ██▒▒██▒  ██▒▒██▒  ██▒ ██▄█▒
//  ▒██▀▀██░▒██░  ██▒▒██░  ██▒▓███▄░
//  ░▓█ ░██ ▒██   ██░▒██   ██░▓██ █▄
//  ░▓█▒░██▓░ ████▓▒░░ ████▓▒░▒██▒ █▄
//   ▒ ░░▒░▒░ ▒░▒░▒░ ░ ▒░▒░▒░ ▒ ▒▒ ▓▒
//   ▒ ░▒░ ░  ░ ▒ ▒░   ░ ▒ ▒░ ░ ░▒ ▒░
//   ░  ░░ ░░ ░ ░ ▒  ░ ░ ░ ▒  ░ ░░ ░
//   ░  ░  ░    ░ ░      ░ ░  ░  ░
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

#include "network/protocol/http/v1.1/http_response_writer.h"
#include "network/protocol/http/v1.1/http_server_builder.h"

using namespace koobika::hook::network::protocol::http::v11;
using namespace koobika::hook::base;

int main() {
  try {
    // Let's create our server using the default configuration..
    auto server = HttpServerBuilder().Build();
    // Let's configure our server to handle requests over '/foo/bar' uri..
    server->Handle("/foo/bar", [](const HttpRequest& req, HttpResponse& res) {
      // In this example we're only interested on <GET> requests..
      if (req.Method.IsGet()) {
        // Set some response headers..
        res.Headers.Set("Server", "Example");
        res.Headers.Set("Date", "Wed, 17 Apr 2013 12:00:00 GMT");
        // Let's directly write text using our 'HttpResponseWriter' class!
        HttpResponseWriter("Hello, World!").Prepare(res).Ok_200();
      } else {
        HttpResponseWriter("Not supported!").Prepare(res).Forbidden_403();
      }
    });
    // Start server activity..
    server->Start("8542");
    // Wait until user press a key..
    return getchar();
  } catch (std::exception exception) {
    // [error] -> while performing setup!
    std::cout << exception.what() << std::endl;
    return -1;
  }
}
