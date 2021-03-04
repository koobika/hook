// /////////////////////////////////////////////////////////////////////////////
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

#include "network/protocol/http/v1.1/decoders/json.h"
#include "network/protocol/http/v1.1/http_response_writer.h"
#include "network/protocol/http/v1.1/http_server_builder.h"

using namespace koobika::hook::network::protocol::http::v11;
using namespace koobika::hook::structured::json;
using namespace koobika::hook::base;

int main() {
  try {
    // Let's create our server using the default configuration..
    auto server = HttpServerBuilder().Build();
    // Let's configure our server to handle requests over '/foo/bar' uri..
    server->Handle("/foo/bar", [](const HttpRequest& req, HttpResponse& res) {
      // In this example we're only interested on <GET> requests..
      if (req.Method.IsGet()) {
        // Let's declare a simple json array object!
        JsonArray json = {"Sunday",   "Monday", "Tuesday",  "Wednesday",
                          "Thursday", "Friday", "Saturday", 123456,
                          6543.21,   nullptr,      false};
        // Let's directly use it through 'HttpResponseWriter' class!
        HttpResponseWriter(json).Prepare(res).Ok_200();
      } else {
        HttpResponseWriter("Not supported!").Prepare(res).Forbidden_403();
      }
    });
    // Start server activity..
    server->Start("8542");
    // Wait until user press a key..
    return getchar();
  } catch (std::exception exception) {
    // ((Error)) -> while performing setup!
    std::cout << exception.what() << std::endl;
    return -1;
  }
}
