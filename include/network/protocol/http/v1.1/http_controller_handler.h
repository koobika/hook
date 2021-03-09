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

#ifndef koobika_hook_network_protocol_http_v11_httpcontrollerhandler_h
#define koobika_hook_network_protocol_http_v11_httpcontrollerhandler_h

#include <string>
#include <regex>

#include "http_router.h"
#include "http_request.h"
#include "http_response.h"

namespace koobika::hook::network::protocol::http::v11 {
// =============================================================================
// HttpControllerHandler                                               ( class )
// -----------------------------------------------------------------------------
// This class is in charge of providing the http controller handler class.
// -----------------------------------------------------------------------------
// Template parameters:
//    RQty - http request type being used
//    RQty - http response type being used
// =============================================================================
template <typename RQty = HttpRequest, typename RSty = HttpResponse>
class HttpControllerHandler {
 public:
  // ---------------------------------------------------------------------------
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  // ---------------------------------------------------------------------------
  HttpControllerHandler(
      HttpRouter<RQty, RSty>* parent,
      const std::string& route,
      const typename HttpRoutesTypes<RQty, RSty>::RouteHandler& handler, 
      const HttpMethodValue& method = HttpConstants::Methods::kAll, 
      const HttpAuthSupport& auth_support = HttpAuthSupport::kDisabled) {
    parent->Handle(route, handler, method, auth_support);
  }
  HttpControllerHandler(
      HttpRouter<RQty, RSty>* parent, const std::regex& regex,
      const typename HttpRoutesTypes<RQty, RSty>::RouteHandler& handler,
      const HttpMethodValue& method = HttpConstants::Methods::kAll,
      const HttpAuthSupport& auth_support = HttpAuthSupport::kDisabled) {
    parent->Handle(regex, handler, method, auth_support);
  }
  HttpControllerHandler(const HttpControllerHandler&) = delete;
  HttpControllerHandler(HttpControllerHandler&&) noexcept = delete;
  virtual ~HttpControllerHandler() = default;
  // ---------------------------------------------------------------------------
  // OPERATORs                                                        ( public )
  // ---------------------------------------------------------------------------
  HttpControllerHandler& operator=(const HttpControllerHandler&) = delete;
  HttpControllerHandler& operator=(HttpControllerHandler&&) noexcept = delete;
};
}  // namespace koobika::hook::network::protocol::http::v11

#endif