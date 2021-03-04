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

#include <regex>
#include <string>

#include "http_auth_support.h"
#include "http_method_value.h"
#include "http_routes.h"

namespace koobika::hook::network::protocol::http::v11 {
// =============================================================================
// HttpRoutesManager                                               ( interface )
// -----------------------------------------------------------------------------
// This specification holds for http routes manager interface.
// =============================================================================
template <typename RQty, typename RSty>
class HttpRoutesManager {
 public:
  // ---------------------------------------------------------------------------
  // METHODs                                                          ( public )
  // ---------------------------------------------------------------------------
  // Adds a new <generic> route_handler to the internal map using an string
  // route.
  virtual void Handle(
      const std::string& route,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
      const HttpMethodValue& method, const HttpAuthSupport& auth_support) = 0;
  // Adds a new <generic> route_handler to the internal map using a regex route.
  virtual void Handle(
      const std::regex& regex,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
      const HttpMethodValue& method, const HttpAuthSupport& auth_support) = 0;
  // Adds a new <options> route_handler to 'string-guided' router structures.
  virtual void Options(
      const std::string& route,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
      const HttpAuthSupport& auth_support) = 0;
  // Adds a new <options> route_handler to 'regex-guided' router structures.
  virtual void Options(
      const std::regex& regex,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
      const HttpAuthSupport& auth_support) = 0;
  // Adds a new <get> route_handler to 'string-guided' router structures.
  virtual void Get(
      const std::string& route,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
      const HttpAuthSupport& auth_support) = 0;
  // Adds a new <get> route_handler to 'regex-guided' router structures.
  virtual void Get(
      const std::regex& regex,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
      const HttpAuthSupport& auth_support) = 0;
  // Adds a new <head> route_handler to 'string-guided' router structures.
  virtual void Head(
      const std::string& route,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
      const HttpAuthSupport& auth_support) = 0;
  // Adds a new <head> route_handler to 'regex-guided' router structures.
  virtual void Head(
      const std::regex& regex,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
      const HttpAuthSupport& auth_support) = 0;
  // Adds a new <post> route_handler to 'string-guided' router structures.
  virtual void Post(
      const std::string& route,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
      const HttpAuthSupport& auth_support) = 0;
  // Adds a new <post> route_handler to 'regex-guided' router structures.
  virtual void Post(
      const std::regex& regex,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
      const HttpAuthSupport& auth_support) = 0;
  // Adds a new <put> route_handler to 'string-guided' router structures.
  virtual void Put(
      const std::string& route,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
      const HttpAuthSupport& auth_support) = 0;
  // Adds a new <put> route_handler to 'regex-guided' router structures.
  virtual void Put(
      const std::regex& regex,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
      const HttpAuthSupport& auth_support) = 0;
  // Adds a new <delete> route_handler to 'string-guided' router structures.
  virtual void Delete(
      const std::string& route,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
      const HttpAuthSupport& auth_support) = 0;
  // Adds a new <delete> route_handler to 'regex-guided' router structures.
  virtual void Delete(
      const std::regex& regex,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
      const HttpAuthSupport& auth_support) = 0;
  // Adds a new <trace> route_handler to 'string-guided' router structures.
  virtual void Trace(
      const std::string& route,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
      const HttpAuthSupport& auth_support) = 0;
  // Adds a new <trace> route_handler to 'regex-guided' router structures.
  virtual void Trace(
      const std::regex& regex,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
      const HttpAuthSupport& auth_support) = 0;
  // Adds a new <connect> route_handler to 'string-guided' router structures.
  virtual void Connect(
      const std::string& route,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
      const HttpAuthSupport& auth_support) = 0;
  // Adds a new <connect> route_handler to 'regex-guided' router structures.
  virtual void Connect(
      const std::regex& regex,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
      const HttpAuthSupport& auth_support) = 0;
};
}  // namespace koobika::hook::network::protocol::http::v11

#endif