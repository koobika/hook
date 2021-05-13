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
// network/protocol/http/http_controller.h
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

#ifndef koobika_hook_network_protocol_http_httpcontroller_h
#define koobika_hook_network_protocol_http_httpcontroller_h

#include "http_router.h"
#include "http_controller_handler.h"
#include "auth/modules/no_auth.h"

namespace koobika::hook::network::protocol::http {
// =============================================================================
// HttpController                                                      ( class )
// -----------------------------------------------------------------------------
// This class is in charge of providing the http controller class.
// =============================================================================
template <typename AUty = auth::modules::NoAuth>
class HttpController : public HttpRouter, public AUty {
 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  // 
  HttpController() = default;
  HttpController(const HttpController&) = delete;
  HttpController(HttpController&&) noexcept = delete;
  virtual ~HttpController() = default;
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  // 
  HttpController& operator=(const HttpController&) = delete;
  HttpController& operator=(HttpController&&) noexcept = delete;

 protected:
  // ___________________________________________________________________________
  // METHODs                                                       ( protected )
  // 
  void AddToRouter(HttpRouter& router) const {
    for (auto const& itr : this->routes_) {
      if (!itr.first.index()) {
        router.Handle(std ::get<std::string>(itr.first), itr.second.handler,
                      itr.second.method);
      } else {
        router.Handle(std ::get<std::regex>(itr.first), itr.second.handler,
                      itr.second.method);
      }
    }
  }
  // ___________________________________________________________________________
  // FRIENDs                                                       ( protected )
  // 
  template <typename TRty, typename ROty>
  friend class HttpServerBase;
};
}  // namespace koobika::hook::network::protocol::http

#endif