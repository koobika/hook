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
// network/protocol/http/routes_controller_handler.h
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

#ifndef koobika_hook_network_protocol_http_routescontrollerhandler_h
#define koobika_hook_network_protocol_http_routescontrollerhandler_h

#include <string>

#include "router.h"

namespace koobika::hook::network::protocol::http {
// =============================================================================
// RoutesControllerHandler                                             ( class )
// -----------------------------------------------------------------------------
// This class is in charge of providing the http routes controller handler.
// =============================================================================
class RoutesControllerHandler {
 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  RoutesControllerHandler(Router* parent, const std::string& route,
                          const RoutingHandler& handler,
                          const MethodValue& method) {
    parent->Handle(route, handler, method);
  }
  RoutesControllerHandler(const RoutesControllerHandler&) = delete;
  RoutesControllerHandler(RoutesControllerHandler&&) noexcept = delete;
  virtual ~RoutesControllerHandler() = default;
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  //
  RoutesControllerHandler& operator=(const RoutesControllerHandler&) = delete;
  RoutesControllerHandler& operator=(RoutesControllerHandler&&) noexcept =
      delete;
};

// =============================================================================
// HTTP_CONTROLLER_DEF                                                 ( macro )
// -----------------------------------------------------------------------------
// This macro will help creating shortcuts for handlers.
// =============================================================================
#define HTTP_CONTROLLER_DEF(METHOD)                                    \
  class RoutesController##METHOD : public RoutesControllerHandler {    \
   public:                                                             \
    RoutesController##METHOD(Router* parent, const std::string& route, \
                             const RoutingHandler& handler)            \
        : RoutesControllerHandler(parent, route, handler,              \
                                  constants::Methods::k##METHOD) {}    \
  };
// =============================================================================
// Helpers (shortcuts)
// =============================================================================
HTTP_CONTROLLER_DEF(Options)
HTTP_CONTROLLER_DEF(Get)
HTTP_CONTROLLER_DEF(Head)
HTTP_CONTROLLER_DEF(Post)
HTTP_CONTROLLER_DEF(Put)
HTTP_CONTROLLER_DEF(Delete)
HTTP_CONTROLLER_DEF(Trace)
HTTP_CONTROLLER_DEF(Connect)
HTTP_CONTROLLER_DEF(Extension)
}  // namespace koobika::hook::network::protocol::http

#endif