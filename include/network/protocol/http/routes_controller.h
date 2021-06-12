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
// network/protocol/http/routes_controller.h
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

#ifndef koobika_hook_network_protocol_http_routescontroller_h
#define koobika_hook_network_protocol_http_routescontroller_h

#include "router.h"
#include "routes_controller_handler.h"
#include "auth/modules/no_auth.h"

namespace koobika::hook::network::protocol::http {
// =============================================================================
// RoutesController                                                    ( class )
// -----------------------------------------------------------------------------
// This class is in charge of providing the http routes controller class.
// =============================================================================
template <typename AUty = auth::modules::NoAuth>
class RoutesController : public Router, public AUty {
 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  RoutesController() = default;
  RoutesController(const RoutesController&) = delete;
  RoutesController(RoutesController&&) noexcept = delete;
  virtual ~RoutesController() = default;
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  //
  RoutesController& operator=(const RoutesController&) = delete;
  RoutesController& operator=(RoutesController&&) noexcept = delete;

 protected:
  // ___________________________________________________________________________
  // METHODs                                                       ( protected )
  //
  void AddToRouter(Router& router) const { router.routes_ = routes_; }
  // ___________________________________________________________________________
  // FRIENDs                                                       ( protected )
  //
  template <typename TRty, typename ROty>
  friend class ServerBase;
};
}  // namespace koobika::hook::network::protocol::http

#endif