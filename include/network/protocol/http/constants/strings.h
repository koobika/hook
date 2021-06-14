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
// network/protocol/http/constants/limits.h
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

#ifndef koobika_hook_network_protocol_http_constants_strings_h
#define koobika_hook_network_protocol_http_constants_strings_h

#include <memory>

namespace koobika::hook::network::protocol::http::constants {
// =============================================================================
// Strings                                                             ( class )
// -----------------------------------------------------------------------------
// This specification holds for all <strings> constants.
// =============================================================================
class Strings {
 public:
  // ___________________________________________________________________________
  // CONSTANTs                                                        ( public )
  //
  // Http-version.
  static constexpr char kHttpVersion[] = "HTTP/1.1";
  static constexpr std::size_t kHttpVersionLen = 8;
  // CrLf.
  static constexpr char kCrLf[] = "\r\n";
  static constexpr std::size_t kCrLfLen = 2;
  // Empty-line.
  static constexpr char kEmptyLine[] = "\r\n\r\n";
  static constexpr std::size_t kEmptyLineLen = 4;
  // Space.
  static constexpr char kSpace[] = " ";
  static constexpr std::size_t kSpaceLen = 1;
  // Colon.
  static constexpr char kColon[] = ":";
  static constexpr std::size_t kColonLen = 1;
  // Header (field-name) separator.
  static constexpr char kHeaderFieldSeparator[] = ": ";
  static constexpr std::size_t kHeaderFieldSeparatorLen = 2;
};
}  // namespace koobika::hook::network::protocol::http::constants

#endif