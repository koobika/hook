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
// network/protocol/http/http_routes_manager.h
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

#ifndef koobika_hook_network_protocol_http_httproutesmanager_h
#define koobika_hook_network_protocol_http_httproutesmanager_h

#include <regex>
#include <string>

#include "http_method_value.h"
#include "http_routes_node.h"
#include "http_routing_handler.h"
#include "http_routing_handler_extended.h"

namespace koobika::hook::network::protocol::http {
// =============================================================================
// HttpRoutesManager                                               ( interface )
// -----------------------------------------------------------------------------
// This specification holds for http routes manager interface.
// =============================================================================
class HttpRoutesManager {
 public:
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  // Adds a new <generic> route to the internal map using an string route.
  virtual void Handle(const std::string& route,
                      const HttpRoutingHandler& handler,
                      const HttpMethodValue& method) = 0;
  // Adds a new <generic> route to the internal map using an string route.
  virtual void Handle(const std::string& route,
                      const HttpRoutingHandlerExtended& handler_extended,
                      const HttpMethodValue& method) = 0;
  // Adds a new <options> route to 'string-guided' router structures.
  virtual void Options(const std::string& route,
                       const HttpRoutingHandler& handler) = 0;
  // Adds a new <options> route to 'string-guided' router structures.
  virtual void Options(const std::string& route,
                       const HttpRoutingHandlerExtended& handler_extended) = 0;
  // Adds a new <get> route to 'string-guided' router structures.
  virtual void Get(const std::string& route,
                   const HttpRoutingHandler& handler) = 0;
  // Adds a new <get> route to 'string-guided' router structures.
  virtual void Get(const std::string& route,
                   const HttpRoutingHandlerExtended& handler_extended) = 0;
  // Adds a new <head> route to 'string-guided' router structures.
  virtual void Head(const std::string& route,
                    const HttpRoutingHandler& handler) = 0;
  // Adds a new <head> route to 'string-guided' router structures.
  virtual void Head(const std::string& route,
                    const HttpRoutingHandlerExtended& handler_extended) = 0;
  // Adds a new <post> route to 'string-guided' router structures.
  virtual void Post(const std::string& route,
                    const HttpRoutingHandler& handler) = 0;
  // Adds a new <post> route to 'string-guided' router structures.
  virtual void Post(const std::string& route,
                    const HttpRoutingHandlerExtended& handler_extended) = 0;
  // Adds a new <put> route to 'string-guided' router structures.
  virtual void Put(const std::string& route,
                   const HttpRoutingHandler& handler) = 0;
  // Adds a new <put> route to 'string-guided' router structures.
  virtual void Put(const std::string& route,
                   const HttpRoutingHandlerExtended& handler_extended) = 0;
  // Adds a new <delete> route to 'string-guided' router structures.
  virtual void Delete(const std::string& route,
                      const HttpRoutingHandler& handler) = 0;
  // Adds a new <delete> route to 'string-guided' router structures.
  virtual void Delete(const std::string& route,
                      const HttpRoutingHandlerExtended& handler_extended) = 0;
  // Adds a new <trace> route to 'string-guided' router structures.
  virtual void Trace(const std::string& route,
                     const HttpRoutingHandler& handler) = 0;
  // Adds a new <trace> route to 'string-guided' router structures.
  virtual void Trace(const std::string& route,
                     const HttpRoutingHandlerExtended& handler_extended) = 0;
  // Adds a new <connect> route to 'string-guided' router structures.
  virtual void Connect(const std::string& route,
                       const HttpRoutingHandler& handler) = 0;
  // Adds a new <connect> route to 'string-guided' router structures.
  virtual void Connect(const std::string& route,
                       const HttpRoutingHandlerExtended& handler_extended) = 0;
};
}  // namespace koobika::hook::network::protocol::http

#endif