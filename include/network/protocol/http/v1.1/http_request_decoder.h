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

#ifndef koobika_hook_network_protocol_http_v11_httprequestdecoder_h
#define koobika_hook_network_protocol_http_v11_httprequestdecoder_h

#include <functional>
#include <string>

#include "base/stream.h"
#include "http_constants.h"
#include "http_encoding_type.h"
#include "http_request.h"
#include "network/transport/server_transport_decoder.h"

namespace koobika::hook::network::protocol::http::v11 {
// =============================================================================
// HttpRequestDecoder                                                  [ class ]
// -----------------------------------------------------------------------------
// This class is in charge of providing the default http request decoder class
// -----------------------------------------------------------------------------
// Template parameters:
//    RQty - http request type being used
// =============================================================================
template <typename RQty>
class HttpRequestDecoder : public transport::ServerTransportDecoder<RQty> {
 public:
  // ---------------------------------------------------------------------------
  // Constructors/Destructors                                         [ public ]
  // ---------------------------------------------------------------------------
  HttpRequestDecoder() = default;
  HttpRequestDecoder(const HttpRequestDecoder&) = delete;
  HttpRequestDecoder(HttpRequestDecoder&&) noexcept = delete;
  ~HttpRequestDecoder() = default;
  // ---------------------------------------------------------------------------
  // Operators                                                        [ public ]
  // ---------------------------------------------------------------------------
  HttpRequestDecoder& operator=(const HttpRequestDecoder&) = delete;
  HttpRequestDecoder& operator=(HttpRequestDecoder&&) noexcept = delete;
  // ---------------------------------------------------------------------------
  // Methods                                                          [ public ]
  // ---------------------------------------------------------------------------
  // Adds the specified buffer fragment to the internal decoder data.
  bool Add(void* buffer, const std::size_t& length) override {
    if (!decoding_body_part_) {
      // If required buffer size is greater than the maximum supported let's
      // return false in order to let the caller know for this error!
      if ((used_ + length) >= HttpConstants::Limits::kMaxHttpMessageLength) {
        return false;
      }
      memcpy(&buffer_[used_], buffer, length);
      used_ += length;

      /*
      pepe
      */

      used_ = 0;

      /*
      pepe fin
      */

    } else {
    }
    return true;
  }
  // Decodes currently stored data.
  bool Decode(
      const transport::ServerTransportDecoder<RQty>::OnErrorHandler& on_error)
      override {
    bool content_decoded = false;
    // first, let's check for <basic> message content decoding!
    if (!decoding_body_part_) {
      do {
        std::size_t cursor = 0, end = 0;
        auto empty_line_ptr =
            SearchFor_(buffer_, used_, HttpConstants::Strings::kEmptyLine,
                       HttpConstants::Strings::kEmptyLineLen);
        // if empty-line sequence (\r\n\r\n) was not found, let's
        // return control and wait for more data to arrive!
        if (empty_line_ptr == nullptr) break;
        // let's reset all internal parameters!
        content_decoded = true;
        used_ = 0;
      } while (false);
    }
    return content_decoded;
  }

 private:
  // ---------------------------------------------------------------------------
  // Methods                                                         [ private ]
  // ---------------------------------------------------------------------------
  // Searchs for the required string content within the provided buffer.
  const char* SearchFor_(const char* buffer, const std::size_t& length,
                         const char* str, const std::size_t& str_len) const {
    if (str_len > 0) {
      std::size_t s_off = 0, out_pos = 0;
      while (out_pos < length) {
        auto this_character = buffer[out_pos++];
        if (this_character == str[s_off]) {
          if (++s_off == str_len) {
            return &buffer[out_pos - str_len];
          }
          continue;
        }
        s_off = 0;
      }
    }
    return nullptr;
  }
  // ---------------------------------------------------------------------------
  // Attributes                                                      [ private ]
  // ---------------------------------------------------------------------------
  // internal buffer
  char buffer_[HttpConstants::Limits::kMaxHttpMessageLength] = {0};
  // internal buffer used size
  std::size_t used_ = 0;
  // flag used to determine if we're decoding the body segment
  bool decoding_body_part_ = false;
};
}  // namespace koobika::hook::network::protocol::http::v11

#endif