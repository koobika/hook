﻿// /////////////////////////////////////////////////////////////////////////////
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
// examples/network/http_server_auth_basic_controller_custom_checker.cpp
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

#include "network/protocol/http/server_builder.h"

using namespace koobika::hook::network::protocol::http;

// This is our basic controller using basic built-in authorization!
// In this example we're just creating three different handlers managing
// different funcionalities. Some of them will require authorization
// mechanism while other ones will be accessed using no credentials.
class BasicController : public RoutesController<auth::modules::Basic> {
 public:
  // Let's, for example, set the user specified checker function!
  BasicController(const Checker& custom_user_checker) {
    Check = custom_user_checker;
  }

 protected:
  // This |POST| handler will increment internal counter value!
  // In order to allow operation only authorized users can access it!
  RoutesControllerPost myIncrementHandler{
      this, "/foo/inc",
      Authorize([this](const Request& req, Response& res) {
        res.Body.Write("Incrementing internal counter to -> ")
            .Write(std::to_string(++counter_))
            .Write(" !");
        res.Ok_200();
      })};
  // This |POST| handler will decrement internal counter value!
  // In order to allow operation only authorized users can access it!
  RoutesControllerPost myDecrementHandler{
      this, "/foo/dec",
      Authorize([this](const Request& req, Response& res) {
        res.Body.Write("Decrementing internal counter to -> ")
            .Write(std::to_string(--counter_))
            .Write(" !");
        res.Ok_200();
      })};
  // This |GET| handler will return current internal counter value!
  // No authorization mechanism is enabled!
  RoutesControllerGet myCurrentValueHandler{
      this, "/foo/cur", [this](const Request& req, Response& res) {
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
    auto server = ServerBuilder().Build();
    server->Handle<BasicController>(
        [](const BasicController::Context& context) -> bool {
          return context.Username == "hook" && context.Password == "rules";
        });
    server->Start("8080");
    return getchar();
  } catch (const std::exception& exception) {
    // ((Error)) -> while performing setup!
    std::cout << exception.what() << std::endl;
    return -1;
  }
}