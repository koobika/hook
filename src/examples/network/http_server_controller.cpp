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

#include "base/stream.h"
#include "network/protocol/http/v1.1/http_controller_handler.h"
#include "network/protocol/http/v1.1/http_server_builder.h"

using namespace koobika::hook::network::protocol::http::v11;
using namespace koobika::hook::base;

// This is our custom controller! In this example we're just creating
// two different handlers managing two kind of routes: string/regex.
class CustomController : public HttpController<> {
 public:
  // This is my sample <nominal> controller!
  HttpControllerHandler<> myNominalHandler{
      this, "/foo/bar",
      [](const HttpRequest& req, HttpResponse& res) {
        // Set the response body using the provided stream writer..
        res.Body.Write("Hello, string routing World!\r\n");
        // Set the response code and.. that's all!
        res.Ok_200();
      },
      HttpConstants::Methods::kGet, HttpAuthSupport::kDisabled};
  // This is my sample <regex> controller!
  HttpControllerHandler<> myRegExHandler{
      this, std::regex("/foo/abc+"),
      [](const HttpRequest& req, HttpResponse& res) {
        // Set the response body using the provided stream writer..
        res.Body.Write("Hello, regex routing World!\r\n");
        // Set the response code and.. that's all!
        res.Ok_200();
      },
      HttpConstants::Methods::kGet, HttpAuthSupport::kDisabled};
};

int main() {
  try {
    // Let's create our server using the default configuration..
    auto server = HttpServerBuilder().Build();
    // Let's configure our server to handle our controller..
    server->Handle<CustomController>();
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
