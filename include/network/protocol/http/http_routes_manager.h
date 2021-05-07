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
#include "http_routes_types.h"

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
                      const typename HttpRoutesTypes::Handler& handler,
                      const HttpMethodValue& method) = 0;
  // Adds a new <generic> handler to the internal map using a regex route.
  virtual void Handle(const std::regex& regex,
                      const typename HttpRoutesTypes::Handler& handler,
                      const HttpMethodValue& method) = 0;
  // Adds a new <options> route to 'string-guided' router structures.
  virtual void Options(const std::string& route,
                       const typename HttpRoutesTypes::Handler& handler) = 0;
  // Adds a new <options> route to 'regex-guided' router structures.
  virtual void Options(const std::regex& regex,
                       const typename HttpRoutesTypes::Handler& handler) = 0;
  // Adds a new <get> route to 'string-guided' router structures.
  virtual void Get(const std::string& route,
                   const typename HttpRoutesTypes::Handler& handler) = 0;
  // Adds a new <get> route to 'regex-guided' router structures.
  virtual void Get(const std::regex& regex,
                   const typename HttpRoutesTypes::Handler& handler) = 0;
  // Adds a new <head> route to 'string-guided' router structures.
  virtual void Head(const std::string& route,
                    const typename HttpRoutesTypes::Handler& handler) = 0;
  // Adds a new <head> route to 'regex-guided' router structures.
  virtual void Head(const std::regex& regex,
                    const typename HttpRoutesTypes::Handler& handler) = 0;
  // Adds a new <post> route to 'string-guided' router structures.
  virtual void Post(const std::string& route,
                    const typename HttpRoutesTypes::Handler& handler) = 0;
  // Adds a new <post> route to 'regex-guided' router structures.
  virtual void Post(const std::regex& regex,
                    const typename HttpRoutesTypes::Handler& handler) = 0;
  // Adds a new <put> route to 'string-guided' router structures.
  virtual void Put(const std::string& route,
                   const typename HttpRoutesTypes::Handler& handler) = 0;
  // Adds a new <put> route to 'regex-guided' router structures.
  virtual void Put(const std::regex& regex,
                   const typename HttpRoutesTypes::Handler& handler) = 0;
  // Adds a new <delete> route to 'string-guided' router structures.
  virtual void Delete(const std::string& route,
                      const typename HttpRoutesTypes::Handler& handler) = 0;
  // Adds a new <delete> route to 'regex-guided' router structures.
  virtual void Delete(const std::regex& regex,
                      const typename HttpRoutesTypes::Handler& handler) = 0;
  // Adds a new <trace> route to 'string-guided' router structures.
  virtual void Trace(const std::string& route,
                     const typename HttpRoutesTypes::Handler& handler) = 0;
  // Adds a new <trace> route to 'regex-guided' router structures.
  virtual void Trace(const std::regex& regex,
                     const typename HttpRoutesTypes::Handler& handler) = 0;
  // Adds a new <connect> route to 'string-guided' router structures.
  virtual void Connect(const std::string& route,
                       const typename HttpRoutesTypes::Handler& handler) = 0;
  // Adds a new <connect> route to 'regex-guided' router structures.
  virtual void Connect(const std::regex& regex,
                       const typename HttpRoutesTypes::Handler& handler) = 0;
};
}  // namespace koobika::hook::network::protocol::http

#endif