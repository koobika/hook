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

#ifndef koobika_hook_network_protocol_http_v11_httpresponsewriter_h
#define koobika_hook_network_protocol_http_v11_httpresponsewriter_h

#include <optional>

#include "base/stream_writer.h"
#include "http_constants.h"
#include "http_mime_types.h"
#include "structured/json/json_value.h"

namespace koobika::hook::network::protocol::http::v11 {
// =============================================================================
// HttpResponseWriter                                                  ( class )
// -----------------------------------------------------------------------------
// This specification holds for http response writer class
// =============================================================================
class HttpResponseWriter {
 public:
  // ---------------------------------------------------------------------------
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  // ---------------------------------------------------------------------------
  template <typename SEty>
  HttpResponseWriter(const SEty& serializable_object,
                     const std::string& content_type)
      : content_type_{content_type} {
    auto data = serializable_object.Serialize();
    if (data.has_value()) {
      writer_.Write(data.value());
    }
  }
  HttpResponseWriter(const structured::json::JsonValue& json)
      : HttpResponseWriter(json, HttpMimeTypes::kJSON){}
  HttpResponseWriter(
      const std::string& string_content,
      const std::optional<std::string>& content_type = HttpMimeTypes::kTXT)
      : content_type_{content_type} {
    writer_.Write(string_content);
  }
  HttpResponseWriter(
      const char* c_string_content,
      const std::optional<std::string>& content_type = HttpMimeTypes::kTXT)
      : content_type_{content_type} {
    writer_.Write(c_string_content);
  }
  HttpResponseWriter(void* buffer, const std::size_t& length,
                     const std::optional<std::string>& content_type)
      : content_type_{content_type} {
    writer_.Write(buffer, length);
  }
  HttpResponseWriter(const HttpResponseWriter&) = delete;
  HttpResponseWriter(HttpResponseWriter&&) noexcept = delete;
  ~HttpResponseWriter() = default;
  // ---------------------------------------------------------------------------
  // OPERATORs                                                        ( public )
  // ---------------------------------------------------------------------------
  HttpResponseWriter& operator=(const HttpResponseWriter&) = delete;
  HttpResponseWriter& operator=(HttpResponseWriter&&) noexcept = delete;
  // ---------------------------------------------------------------------------
  // METHODs                                                          ( public )
  // ---------------------------------------------------------------------------
  // Tries to fill-up incoming response object with current configutation.
  template <typename RSty>
  RSty& Prepare(RSty& res) {
    if (content_type_.has_value()) {
      res.Headers.Set(HttpConstants::Headers::kContentType,
                      content_type_.value());
    }
    res.Headers.Set(HttpConstants::Headers::kContentLength, writer_.Length());
    res.Body = std::move(writer_);
    return res;
  }

 private:
  // ---------------------------------------------------------------------------
  // ATTRIBUTEs                                                      ( private )
  // ---------------------------------------------------------------------------
  base::StreamWriter writer_;
  std::optional<std::string> content_type_;
};
}  // namespace koobika::hook::network::protocol::http::v11

#endif