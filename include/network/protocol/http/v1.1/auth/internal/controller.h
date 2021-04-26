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

#ifndef koobika_hook_network_protocol_http_v11_auth_internal_controller_h
#define koobika_hook_network_protocol_http_v11_auth_internal_controller_h

#include "auth/controller.h"
#include "network/protocol/http/v1.1/http_routes_types.h"

namespace koobika::hook::network::protocol::http::v11::auth::internal {
// =============================================================================
// Controller                                                          ( class )
// -----------------------------------------------------------------------------
// This specification holds for <basic-auth> controller module
// =============================================================================
template <typename CXty>
class Controller : public hook::auth::Controller<CXty> {
 public:
  // ---------------------------------------------------------------------------
  // METHODs                                                          ( public )
  // ---------------------------------------------------------------------------
  typename HttpRoutesTypes::Handler Authorize(
      const typename HttpRoutesTypes::Handler handler) const {
    return [this, handler](typename HttpRoutesTypes::Request req,
                           typename HttpRoutesTypes::Response res) {
      typename hook::auth::Controller<CXty>::Context context;
      if (context.Map(req)) {
        if (this->Check == nullptr || this->Check(context)) {
          handler(req, res);
        } else {
          res.Forbidden_403();
        }
      } else {
        res.Forbidden_403();
      }
    };
  }
};
}  // namespace koobika::hook::network::protocol::http::v11::auth::internal

#endif