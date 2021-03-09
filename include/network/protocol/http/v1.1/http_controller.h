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

#ifndef koobika_hook_network_protocol_http_v11_httpcontroller_h
#define koobika_hook_network_protocol_http_v11_httpcontroller_h

#include <memory>

#include "http_router.h"
#include "http_request.h"
#include "http_response.h"

namespace koobika::hook::network::protocol::http::v11 {
// =============================================================================
// HttpController                                                      ( class )
// -----------------------------------------------------------------------------
// This class is in charge of providing the http controller class.
// -----------------------------------------------------------------------------
// Template parameters:
//    RQty - http request type being used
//    RQty - http response type being used
// =============================================================================
template <typename RQty = HttpRequest, typename RSty = HttpResponse>
class HttpController
    : public HttpRouter<RQty, RSty>,
      public std::enable_shared_from_this<HttpController<RQty, RSty>> {
 public:
  // ---------------------------------------------------------------------------
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  // ---------------------------------------------------------------------------
  HttpController() = default;
  HttpController(const HttpController&) = delete;
  HttpController(HttpController&&) noexcept = delete;
  virtual ~HttpController() = default;
  // ---------------------------------------------------------------------------
  // OPERATORs                                                        ( public )
  // ---------------------------------------------------------------------------
  HttpController& operator=(const HttpController&) = delete;
  HttpController& operator=(HttpController&&) noexcept = delete;

 protected:
  // ---------------------------------------------------------------------------
  // METHODs                                                       ( protected )
  // ---------------------------------------------------------------------------
  void AddToRouter(HttpRouter<RQty, RSty>& router) {
    for (auto const& itr : this->map_) {
      router.Handle(itr.first, itr.second.handler, itr.second.method,
                    itr.second.auth_support, this->shared_from_this());
    }
    for (auto const& itr : this->vec_) {
      router.Handle(itr.first, itr.second.handler, itr.second.method,
                    itr.second.auth_support, this->shared_from_this());
    }
  }
  // ---------------------------------------------------------------------------
  // FRIENDs                                                       ( protected )
  // ---------------------------------------------------------------------------
  template <typename TRty, template <typename, typename> class ROty,
            typename RQty, typename RSty>
  friend class HttpServerBase;
};
}  // namespace koobika::hook::network::protocol::http::v11

#endif