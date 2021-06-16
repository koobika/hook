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
// examples/network/http_server_handler_shortcut.cpp
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

int main() {
  try {
    auto server = ServerBuilder().Build();
    //! [GET]
    server->Get("/foo/bar", [](const Request& req, Response& res) {
      res.Body.Write("GET based route!");
      res.Ok_200();
    });
    //! [GET]
    //! [GET-EXTENDED]
    server->Get("/{a}/{b}/{c}", [](const Request& req, Response& res,
                                   const Parameters& parameters) {
      for (auto const& parameter : parameters) {
        std::cout << parameter.first << ": " << parameter.second << std::endl;
      }
      res.Body.Write("GET based route using extended parameters!");
      res.Ok_200();
    });
    //! [GET-EXTENDED]
    //! [PUT]
    server->Put("/foo/bar", [](const Request& req, Response& res) {
      res.Body.Write("PUT based route!");
      res.Ok_200();
    });
    //! [PUT]
    //! [PUT-EXTENDED]
    server->Put("/{a}/{b}/{c}", [](const Request& req, Response& res,
                                   const Parameters& parameters) {
      for (auto const& parameter : parameters) {
        std::cout << parameter.first << ": " << parameter.second << std::endl;
      }
      res.Body.Write("PUT based route using extended parameters!");
      res.Ok_200();
    });
    //! [PUT-EXTENDED]
    //! [POST]
    server->Post("/foo/bar", [](const Request& req, Response& res) {
      res.Body.Write("POST based route!");
      res.Ok_200();
    });
    //! [POST]
    //! [POST-EXTENDED]
    server->Post("/{a}/{b}/{c}", [](const Request& req, Response& res,
                                    const Parameters& parameters) {
      for (auto const& parameter : parameters) {
        std::cout << parameter.first << ": " << parameter.second << std::endl;
      }
      res.Body.Write("POST based route using extended parameters!");
      res.Ok_200();
    });
    //! [POST-EXTENDED]
    //! [DELETE]
    server->Delete("/foo/bar", [](const Request& req, Response& res) {
      res.Body.Write("DELETE based route!");
      res.Ok_200();
    });
    //! [DELETE]
    //! [DELETE-EXTENDED]
    server->Delete("/{a}/{b}/{c}", [](const Request& req, Response& res,
                                      const Parameters& parameters) {
      for (auto const& parameter : parameters) {
        std::cout << parameter.first << ": " << parameter.second << std::endl;
      }
      res.Body.Write("DELETE based route using extended parameters!");
      res.Ok_200();
    });
    //! [DELETE-EXTENDED]
    //! [OPTIONS]
    server->Options("/foo/bar", [](const Request& req, Response& res) {
      res.Body.Write("OPTIONS based route!");
      res.Ok_200();
    });
    //! [OPTIONS]
    //! [OPTIONS-EXTENDED]
    server->Options("/{a}/{b}/{c}", [](const Request& req, Response& res,
                                       const Parameters& parameters) {
      for (auto const& parameter : parameters) {
        std::cout << parameter.first << ": " << parameter.second << std::endl;
      }
      res.Body.Write("OPTIONS based route using extended parameters!");
      res.Ok_200();
    });
    //! [OPTIONS-EXTENDED]
    //! [HEAD]
    server->Head("/foo/bar", [](const Request& req, Response& res) {
      res.Body.Write("HEAD based route!");
      res.Ok_200();
    });
    //! [HEAD]
    //! [HEAD-EXTENDED]
    server->Head("/{a}/{b}/{c}", [](const Request& req, Response& res,
                                    const Parameters& parameters) {
      for (auto const& parameter : parameters) {
        std::cout << parameter.first << ": " << parameter.second << std::endl;
      }
      res.Body.Write("HEAD based route using extended parameters!");
      res.Ok_200();
    });
    //! [HEAD-EXTENDED]
    //! [TRACE]
    server->Trace("/foo/bar", [](const Request& req, Response& res) {
      res.Body.Write("TRACE based route!");
      res.Ok_200();
    });
    //! [TRACE]
    //! [TRACE-EXTENDED]
    server->Trace("/{a}/{b}/{c}", [](const Request& req, Response& res,
                                     const Parameters& parameters) {
      for (auto const& parameter : parameters) {
        std::cout << parameter.first << ": " << parameter.second << std::endl;
      }
      res.Body.Write("TRACE based route using extended parameters!");
      res.Ok_200();
    });
    //! [TRACE-EXTENDED]
    //! [CONNECT]
    server->Connect("/foo/bar", [](const Request& req, Response& res) {
      res.Body.Write("CONNECT based route!");
      res.Ok_200();
    });
    //! [CONNECT]
    //! [CONNECT-EXTENDED]
    server->Connect("/{a}/{b}/{c}", [](const Request& req, Response& res,
                                       const Parameters& parameters) {
      for (auto const& parameter : parameters) {
        std::cout << parameter.first << ": " << parameter.second << std::endl;
      }
      res.Body.Write("CONNECT based route using extended parameters!");
      res.Ok_200();
    });
    //! [CONNECT-EXTENDED]
    server->Start("8080");
    return getchar();
  } catch (const std::exception& exception) {
    // ((Error)) -> while performing setup!
    std::cout << exception.what() << std::endl;
    return -1;
  }
}