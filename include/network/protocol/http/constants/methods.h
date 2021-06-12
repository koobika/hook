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
// network/protocol/http/constants/methods.h
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

#ifndef koobika_hook_network_protocol_http_constants_methods_h
#define koobika_hook_network_protocol_http_constants_methods_h

#include "network/protocol/http/method_value.h"

namespace koobika::hook::network::protocol::http::constants {
// =============================================================================
// Methods                                                             ( class )
// -----------------------------------------------------------------------------
// This specification holds for all <methods> constants.
// =============================================================================
class Methods {
 public:
  // ___________________________________________________________________________
  // CONSTANTs                                                        ( public )
  //
  // Options.
  static constexpr MethodValue kOptions = 1;
  // Get.
  static constexpr MethodValue kGet = 2;
  // Head.
  static constexpr MethodValue kHead = 4;
  // Post.
  static constexpr MethodValue kPost = 8;
  // Put.
  static constexpr MethodValue kPut = 16;
  // Delete.
  static constexpr MethodValue kDelete = 32;
  // Trace.
  static constexpr MethodValue kTrace = 64;
  // Connect.
  static constexpr MethodValue kConnect = 128;
  // Extension.
  static constexpr MethodValue kExtension = 256;
  // Invalid.
  static constexpr MethodValue kInvalid = 512;
  // All.
  static constexpr MethodValue kAll = kOptions | kGet | kHead | kPost | kPut |
                                      kDelete | kTrace | kConnect | kExtension;
};
}  // namespace koobika::hook::network::protocol::http::constants

#endif