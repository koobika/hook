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

#ifndef koobika_hook_network_protocol_http_v11_httproutesmanager_h
#define koobika_hook_network_protocol_http_v11_httproutesmanager_h

#include <functional>
#include <regex>
#include <string>
#include <unordered_map>

#include "http_auth_support.h"
#include "http_method_value.h"
#include "http_routes.h"

namespace koobika::hook::network::protocol::http::v11 {
// =============================================================================
// HttpRoutesManager                                               [ interface ]
// -----------------------------------------------------------------------------
// This specification holds for http routes manager interface
// =============================================================================
template <typename RQty, typename RSty>
class HttpRoutesManager {
 public:
  // ---------------------------------------------------------------------------
  // Methods                                                          [ public ]
  // ---------------------------------------------------------------------------
  // Adds a new <generic> handler to the internal map
  virtual void Handle(const std::string&,
                      const typename HttpRoutes<RQty, RSty>::RouteHandler&,
                      const HttpMethodValue&, const HttpAuthSupport&) = 0;
  // Adds a new <generic> handler to the internal map
  virtual void Handle(const std::regex&,
                      const typename HttpRoutes<RQty, RSty>::RouteHandler&,
                      const HttpMethodValue&, const HttpAuthSupport&) = 0;
  // Adds a new <options> handler to 'nominal' router structures
  virtual void Options(const std::string&,
                       const typename HttpRoutes<RQty, RSty>::RouteHandler&,
                       const HttpAuthSupport&) = 0;
  // Adds a new <options> handler to 'regex' router structures
  virtual void Options(const std::regex&,
                       const typename HttpRoutes<RQty, RSty>::RouteHandler&,
                       const HttpAuthSupport&) = 0;
  // Adds a new <get> handler to 'nominal' router structures
  virtual void Get(const std::string&,
                   const typename HttpRoutes<RQty, RSty>::RouteHandler&,
                   const HttpAuthSupport&) = 0;
  // Adds a new <get> handler to 'regex' router structures
  virtual void Get(const std::regex&,
                   const typename HttpRoutes<RQty, RSty>::RouteHandler&,
                   const HttpAuthSupport&) = 0;
  // Adds a new <head> handler to 'nominal' router structures
  virtual void Head(const std::string&,
                    const typename HttpRoutes<RQty, RSty>::RouteHandler&,
                    const HttpAuthSupport&) = 0;
  // Adds a new <head> handler to 'regex' router structures
  virtual void Head(const std::regex&,
                    const typename HttpRoutes<RQty, RSty>::RouteHandler&,
                    const HttpAuthSupport&) = 0;
  // Adds a new <post> handler to 'nominal' router structures
  virtual void Post(const std::string&,
                    const typename HttpRoutes<RQty, RSty>::RouteHandler&,
                    const HttpAuthSupport&) = 0;
  // Adds a new <post> handler to 'regex' router structures
  virtual void Post(const std::regex&,
                    const typename HttpRoutes<RQty, RSty>::RouteHandler&,
                    const HttpAuthSupport&) = 0;
  // Adds a new <put> handler to 'nominal' router structures
  virtual void Put(const std::string&,
                   const typename HttpRoutes<RQty, RSty>::RouteHandler&,
                   const HttpAuthSupport&) = 0;
  // Adds a new <put> handler to 'regex' router structures
  virtual void Put(const std::regex&,
                   const typename HttpRoutes<RQty, RSty>::RouteHandler&,
                   const HttpAuthSupport&) = 0;
  // Adds a new <delete> handler to 'nominal' router structures
  virtual void Delete(const std::string&,
                      const typename HttpRoutes<RQty, RSty>::RouteHandler&,
                      const HttpAuthSupport&) = 0;
  // Adds a new <delete> handler to 'regex' router structures
  virtual void Delete(const std::regex&,
                      const typename HttpRoutes<RQty, RSty>::RouteHandler&,
                      const HttpAuthSupport&) = 0;
  // Adds a new <trace> handler to 'nominal' router structures
  virtual void Trace(const std::string&,
                     const typename HttpRoutes<RQty, RSty>::RouteHandler&,
                     const HttpAuthSupport&) = 0;
  // Adds a new <trace> handler to 'regex' router structures
  virtual void Trace(const std::regex&,
                     const typename HttpRoutes<RQty, RSty>::RouteHandler&,
                     const HttpAuthSupport&) = 0;
  // Adds a new <connect> handler to 'nominal' router structures
  virtual void Connect(const std::string&,
                       const typename HttpRoutes<RQty, RSty>::RouteHandler&,
                       const HttpAuthSupport&) = 0;
  // Adds a new <connect> handler to 'regex' router structures
  virtual void Connect(const std::regex&,
                       const typename HttpRoutes<RQty, RSty>::RouteHandler&,
                       const HttpAuthSupport&) = 0;
};
}  // namespace koobika::hook::network::protocol::http::v11

#endif