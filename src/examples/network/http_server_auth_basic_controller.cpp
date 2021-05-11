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
// http_server_auth_basic_controller.cpp
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

#include "network/protocol/http/http_server_builder.h"

using namespace koobika::hook::network::protocol::http;

// This is our basic controller using basic built-in authorization!
// In this example we're just creating three different handlers managing
// different funcionalities. Some of them will require authorization
// mechanism while other ones will be accessed using no credentials.
class BasicController : public HttpController<auth::modules::Basic> {
 public:
  // Let's, for example, add here our default supported user/passwords!
  BasicController() { Set("koobika", "koobika"); }

 protected:
  // This |POST| handler will increment internal counter value!
  // In order to allow operation only authorized users can access it!
  HttpControllerPost myIncrementHandler{
      this, "/foo/inc",
      Authorize([this](const HttpRequest& req, HttpResponse& res) {
        res.Body.Write("Incrementing internal counter to -> ")
            .Write(std::to_string(++counter_))
            .Write(" !");
        res.Ok_200();
      })};
  // This |POST| handler will decrement internal counter value!
  // In order to allow operation only authorized users can access it!
  HttpControllerPost myDecrementHandler{
      this, "/foo/dec",
      Authorize([this](const HttpRequest& req, HttpResponse& res) {
        res.Body.Write("Decrementing internal counter to -> ")
            .Write(std::to_string(--counter_))
            .Write(" !");
        res.Ok_200();
      })};
  // This |GET| handler will return current internal counter value!
  // No authorization mechanism is enabled!
  HttpControllerGet myCurrentValueHandler{
      this, "/foo/cur", [this](const HttpRequest& req, HttpResponse& res) {
        res.Body.Write("Current internal counter value -> ")
            .Write(std::to_string(counter_))
            .Write(" !");
        res.Ok_200();
      }};

 private:
  int counter_ = 0;
};

int main() {
  try {
    auto server = HttpServerBuilder().Build();
    server->Handle<BasicController>();
    server->Start("8542");
    return getchar();
  } catch (const std::exception& exception) {
    // ((Error)) -> while performing setup!
    std::cout << exception.what() << std::endl;
    return -1;
  }
}