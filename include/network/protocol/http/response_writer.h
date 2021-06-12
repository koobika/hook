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
#include "structured/json/json_value.h"

namespace koobika::hook::network::protocol::http {
// =============================================================================
// ResponseWriter                                                      ( class )
// -----------------------------------------------------------------------------
// This specification holds for http response writer class
// =============================================================================
class ResponseWriter {
 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  template <typename SEty>
  ResponseWriter(const SEty& serializable_object,
                 const std::string& content_type)
      : content_type_{content_type} {
    buffer_.Write(serializable_object.Serialize());
  }
  ResponseWriter(const structured::json::JsonValue& json)
      : ResponseWriter(json, constants::Mime::kJSON) {}
  ResponseWriter(
      const std::string& string_content,
      const std::optional<std::string>& content_type = constants::Mime::kTXT)
      : content_type_{content_type} {
    buffer_.Write(string_content);
  }
  ResponseWriter(
      const char* c_string_content,
      const std::optional<std::string>& content_type = constants::Mime::kTXT)
      : content_type_{content_type} {
    buffer_.Write(c_string_content);
  }
  ResponseWriter(void* buffer, const std::size_t& length,
                 const std::optional<std::string>& content_type)
      : content_type_{content_type} {
    buffer_.Write(buffer, length);
  }
  ResponseWriter(const ResponseWriter&) = delete;
  ResponseWriter(ResponseWriter&&) noexcept = delete;
  ~ResponseWriter() = default;
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  //
  ResponseWriter& operator=(const ResponseWriter&) = delete;
  ResponseWriter& operator=(ResponseWriter&&) noexcept = delete;
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  // Tries to fill-up incoming response object with current configutation.
  template <typename RSty>
  RSty& Prepare(RSty& res) {
    if (content_type_.has_value()) {
      res.Headers.Set(constants::Headers::kContentType, content_type_.value());
    }
    res.Headers.Set(constants::Headers::kContentLength, buffer_.Length());
    res.Body = std::move(buffer_);
    return res;
  }

 private:
  // ___________________________________________________________________________
  // ATTRIBUTEs                                                      ( private )
  //
  base::AutoBuffer buffer_;
  std::optional<std::string> content_type_;
};
}  // namespace koobika::hook::network::protocol::http

#endif