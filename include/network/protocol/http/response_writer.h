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
// network/protocol/http/response_writer.h
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

#ifndef koobika_hook_network_protocol_http_responsewriter_h
#define koobika_hook_network_protocol_http_responsewriter_h

#include <optional>

#include "base/auto_buffer.h"
#include "constants/headers.h"
#include "constants/mime.h"
#include "structured/json/value.h"
#include "request.h"
#include "response_writers/default.h"
#include "response_writers/transfer_encoding.h"

namespace koobika::hook::network::protocol::http {
// =============================================================================
// ResponseWriter                                                      ( class )
// -----------------------------------------------------------------------------
// This specification holds for http response writer class
// =============================================================================
template <typename WRty = response_writers::Default>
class ResponseWriter {
 public:
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  // Tries to fill-up response object with the specified configuration.
  static Response& Prepare(
      Response& res, const base::AutoBuffer& buffer,
      const std::optional<std::string>& content_type = constants::Mime::kBIN,
      const WRty& writer = WRty()) {
    writer.Write(res, buffer);
    if (content_type.has_value()) {
      res.Headers.Set(constants::Headers::kContentType, content_type.value());
    }
    return res;
  }
  // Tries to fill-up response object with the serializable content
  static Response& Prepare(
      Response& res, const base::Serializable& serializable,
      const std::optional<std::string>& content_type = constants::Mime::kBIN,
      const WRty& writer = WRty()) {
    return Prepare(res, serializable.Serialize(), content_type, writer);
  }
};
}  // namespace koobika::hook::network::protocol::http

#endif