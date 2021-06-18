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
// network/protocol/http/response_writers/default.h
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

#ifndef koobika_hook_network_protocol_http_responsewriters_default_h
#define koobika_hook_network_protocol_http_responsewriters_default_h

#include "base/auto_buffer.h"
#include "network/protocol/http/response.h"

namespace koobika::hook::network::protocol::http::response_writers {
//! @brief Http default writer
class Default {
 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  Default() = default;
  Default(const Default&) = default;
  Default(Default&&) noexcept = default;
  ~Default() = default;
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  //
  Default& operator=(const Default&) = default;
  Default& operator=(Default&&) noexcept = default;
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  //! @brief Writes content provided by <em>buffer</em> to the <em>res</em>
  //! object using no special encoding (Content-Length based)
  //! @param[out] res Response to update
  //! @param[in] buffer Autobuffer containing the data to write
  //! @section Example
  //! @snippet network/http_server_response_builder_transfer_encoding.cpp Ex
  void Write(http::Response& res, const base::AutoBuffer& buffer) const {
    res.Headers.Set(constants::Headers::kContentLength, buffer.Length());
    res.Body = buffer;
  }
};
}  // namespace koobika::hook::network::protocol::http

#endif