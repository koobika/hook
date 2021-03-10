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

#ifndef koobika_hook_network_protocol_http_v11_httproutesperformer_h
#define koobika_hook_network_protocol_http_v11_httproutesperformer_h

#include <string>

#include "http_routes_types.h"

namespace koobika::hook::network::protocol::http::v11 {
// =============================================================================
// Forward-Declarations
// =============================================================================
template <typename, typename>
class HttpAuthModule;
// =============================================================================
// HttpRoutesPerformer                                             ( interface )
// -----------------------------------------------------------------------------
// This specification holds for http routes performer interface.
// =============================================================================
template <typename RQty, typename RSty>
class HttpRoutesPerformer {
 public:
  // ---------------------------------------------------------------------------
  // METHODs                                                          ( public )
  // ---------------------------------------------------------------------------
  // Tries to perform router enabled action.
  virtual bool Perform(
      const std::string& route,
      typename HttpRoutesTypes<RQty, RSty>::Request request,
      typename HttpRoutesTypes<RQty, RSty>::Response response,
      const std::shared_ptr<
          HttpAuthModule<typename HttpRoutesTypes<RQty, RSty>::Request,
                         typename HttpRoutesTypes<RQty, RSty>::Response>>&
          auth_module) const = 0;
};
}  // namespace koobika::hook::network::protocol::http::v11

#endif