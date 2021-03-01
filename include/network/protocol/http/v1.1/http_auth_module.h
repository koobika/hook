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

#ifndef koobika_hook_network_protocol_http_v11_httpauthmodule_h
#define koobika_hook_network_protocol_http_v11_httpauthmodule_h

#include <functional>
#include <string>

#include "http_routes.h"

namespace koobika::hook::network::protocol::http::v11 {
// =============================================================================
// HttpAuthModule                                                  [ interface ]
// -----------------------------------------------------------------------------
// This templated interface needs to be implemented by any <auth>
// mechanism (class) in charge of providing any kind of auth-implementation.
// In other words, every single class being used as an auth-specification
// need to inherit from this one.
// =============================================================================
template <typename RQty, typename RSty>
class HttpAuthModule {
 public:
  // ---------------------------------------------------------------------------
  // Usings                                                           [ public ]
  // ---------------------------------------------------------------------------
  // This signature will be used while defining <auth> verification hooks.
  // It uses the pair request/response as parameters and returns a boolean
  // indicating if auth-verification succeeded (or not). Every function
  // performing auth-checking must follow this signature.
  using Checker =
      std::function<bool(typename HttpRoutes<RQty, RSty>::Request,
                         typename HttpRoutes<RQty, RSty>::Response)>;
  // ---------------------------------------------------------------------------
  // Methods                                                          [ public ]
  // ---------------------------------------------------------------------------
  // Used by the <http-router> module to get the associated <auth> verification
  // function within this module. The function returned by this
  // method (implemented by the corresponding specialization) will be called
  // to allow/deny authorization.
  virtual Checker GetChecker() const = 0;
};
}  // namespace koobika::hook::network::protocol::http::v11

#endif