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
// network/protocol/http/routes_node.h
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

#ifndef koobika_hook_network_protocol_http_routesnode_h
#define koobika_hook_network_protocol_http_routesnode_h

#include <functional>
#include <string>

#include "constants/methods.h"
#include "routing_handler.h"
#include "routing_handler_extended.h"

namespace koobika::hook::network::protocol::http {
// =============================================================================
// RoutesNode                                                         ( struct )
// -----------------------------------------------------------------------------
// This specification holds for http routes <node> type. This class will
// encapsulate all the information needed to perform routing.
// =============================================================================
struct RoutesNode {
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  RoutesNode() = default;
  RoutesNode(const RoutingHandler& in_handler,
             const MethodValue& in_method_value)
      : handler{in_handler}, method{in_method_value} {}
  RoutesNode(const RoutingHandlerExtended& in_handler_extended,
             const MethodValue& in_method_value)
      : handler_extended{in_handler_extended}, method{in_method_value} {}
  RoutesNode(const RoutesNode&) = default;
  RoutesNode(RoutesNode&&) noexcept = default;
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  //
  RoutesNode& operator=(const RoutesNode&) = default;
  RoutesNode& operator=(RoutesNode&&) noexcept = default;
  // ___________________________________________________________________________
  // PROPERTIEs                                                       ( public )
  //
  RoutingHandler handler = nullptr;
  RoutingHandlerExtended handler_extended = nullptr;
  MethodValue method = constants::Methods::kInvalid;
};
}  // namespace koobika::hook::network::protocol::http

#endif