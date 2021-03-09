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

#ifndef koobika_hook_network_protocol_http_v11_httproutesnode_h
#define koobika_hook_network_protocol_http_v11_httproutesnode_h

#include <functional>
#include <string>

#include "http_routes_types.h"
#include "http_method_value.h"
#include "http_auth_support.h"
#include "http_constants.h"

namespace koobika::hook::network::protocol::http::v11 {
// =============================================================================
// types-forwarding.
// =============================================================================
template <typename, typename>
class HttpController;
// =============================================================================
// HttpRoutesNode                                                     ( struct )
// -----------------------------------------------------------------------------
// This specification holds for http routes <node> type. This class will
// encapsulate all the information needed to perform routing.
// =============================================================================
template <typename RQty, typename RSty>
struct HttpRoutesNode {
  // ---------------------------------------------------------------------------
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  // ---------------------------------------------------------------------------
  HttpRoutesNode() = default;
  HttpRoutesNode(const typename HttpRoutesTypes<RQty, RSty>::RouteHandler& hdl,
                 const HttpMethodValue& mth, const HttpAuthSupport& aut,
                 const std::shared_ptr<HttpController<RQty, RSty>>& ctl)
      : handler{hdl}, method{mth}, auth_support{aut}, controller{ctl} {}
  // ---------------------------------------------------------------------------
  // ATTRIBUTEs                                                       ( public )
  // ---------------------------------------------------------------------------
  std::shared_ptr<HttpController<RQty, RSty>> controller = nullptr;
  typename HttpRoutesTypes<RQty, RSty>::RouteHandler handler = nullptr;
  HttpMethodValue method = HttpConstants::Methods::kExtension;
  HttpAuthSupport auth_support = HttpAuthSupport::kDisabled;
};
}  // namespace koobika::hook::network::protocol::http::v11

#endif