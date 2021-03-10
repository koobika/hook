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

#include "http_controller.h"

namespace koobika::hook::network::protocol::http::v11 {
// =============================================================================
// HttpAuthModule                                                  ( interface )
// -----------------------------------------------------------------------------
// This interface must be implemented by every auth-module.
// =============================================================================
template <typename RQty, typename RSty>
class HttpAuthModule : public HttpController<RQty, RSty> {
 public:
  // ---------------------------------------------------------------------------
  // USINGs                                                           ( public )
  // ---------------------------------------------------------------------------
  // Every checker method must follow this signature. A checker is a function
  // that will be called in order to evaluate an auth-mechanism. Every auth
  // class providing for any authorization mechanism must provide for an
  // implementation of this method (returned by 'GetChecker' method).
  // Basically, it will use the provided RQty/RSty pair to check if the
  // request can be processed (satisfies the auth requirements) filling the
  // corresponding response with the required data. It must return TRUE if
  // access was granted, FALSE otherwise.
  using Checker =
      std::function<bool(typename HttpRoutesTypes<RQty, RSty>::Request,
                         typename HttpRoutesTypes<RQty, RSty>::Response)>;
  // ---------------------------------------------------------------------------
  // METHODs                                                          ( public )
  // ---------------------------------------------------------------------------
  // Returns the associated checker functor.
  virtual Checker GetChecker() const = 0;
};
}  // namespace koobika::hook::network::protocol::http::v11

#endif