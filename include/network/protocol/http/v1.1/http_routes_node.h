﻿// /////////////////////////////////////////////////////////////////////////////
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

#include "http_constants.h"
#include "http_method_value.h"
#include "http_routes_types.h"

namespace koobika::hook::network::protocol::http::v11 {
// =============================================================================
// HttpRoutesNode                                                     ( struct )
// -----------------------------------------------------------------------------
// This specification holds for http routes <node> type. This class will
// encapsulate all the information needed to perform routing.
// =============================================================================
struct HttpRoutesNode {
  // ---------------------------------------------------------------------------
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  // ---------------------------------------------------------------------------
  HttpRoutesNode() = default;
  HttpRoutesNode(const typename HttpRoutesTypes::Handler& hdl,
                 const HttpMethodValue& mth)
      : handler{hdl}, method{mth} {}
  HttpRoutesNode(const HttpRoutesNode&) = default;
  HttpRoutesNode(HttpRoutesNode&&) noexcept = default;
  // ---------------------------------------------------------------------------
  // OPERATORs                                                        ( public )
  // ---------------------------------------------------------------------------
  HttpRoutesNode& operator=(const HttpRoutesNode&) = default;
  HttpRoutesNode& operator=(HttpRoutesNode&&) noexcept = default;
  // ---------------------------------------------------------------------------
  // PROPERTIEs                                                       ( public )
  // ---------------------------------------------------------------------------
  typename HttpRoutesTypes::Handler handler = nullptr;
  HttpMethodValue method = HttpConstants::Methods::kExtension;
};
}  // namespace koobika::hook::network::protocol::http::v11

#endif