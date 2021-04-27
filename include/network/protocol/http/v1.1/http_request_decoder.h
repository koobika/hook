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

#include "base/auto_buffer.h"
#include "network/protocol/http/http_constants.h"
#include "network/protocol/http/http_encoding_type.h"
#include "network/protocol/http/http_request.h"
#include "network/transport/server_transport_decoder.h"

namespace koobika::hook::network::protocol::http::v11 {
// =============================================================================
// PERFORM_ERROR_HANDLING                                              ( macro )
// =============================================================================
#define PERFORM_ERROR_HANDLING \
  {                            \
    error_handler();           \
    reset();                   \
    return;                    \
  }
// =============================================================================
// GET_REQUEST_LINE_FIELD                                              ( macro )
// =============================================================================
#define GET_REQUEST_LINE_FIELD(field, delimiter, delimiter_length)  \
  {                                                                 \
    auto ptr = searchFor(&buffer_[cursor], end - cursor, delimiter, \
                         delimiter_length);                         \
    if (ptr == nullptr) PERFORM_ERROR_HANDLING                      \
    auto length = ptr - &buffer_[cursor];                           \
    field.assign(&buffer_[cursor], length);                         \
    cursor += length + delimiter_length;                            \
  }
// =============================================================================
// GET_HEADER_FIELD_NAME                                               ( macro )
// =============================================================================
#define GET_HEADER_FIELD_NAME(name)                            \
  {                                                            \
    auto ptr = searchFor(&buffer_[cursor], end - cursor,       \
                         HttpConstants::Strings::kColon,       \
                         HttpConstants::Strings::kColonLen);   \
    if (ptr == nullptr) PERFORM_ERROR_HANDLING                 \
    name = {cursor, ptr - &buffer_[cursor]};                   \
    cursor += name.second + HttpConstants::Strings::kColonLen; \
  }
// =============================================================================
// GET_HEADER_FIELD_VALUES                                             ( macro )
// =============================================================================
#define GET_HEADER_FIELD_VALUES(values)                              \
  values.clear();                                                    \
  while (cursor < end) {                                             \
    auto ptr = searchFor(&buffer_[cursor], end - cursor,             \
                         HttpConstants::Strings::kCrLf,              \
                         HttpConstants::Strings::kCrLfLen);          \
    if (ptr == nullptr) PERFORM_ERROR_HANDLING                       \
    Indices field_value = {cursor, ptr - &buffer_[cursor]};          \
    cursor += field_value.second + HttpConstants::Strings::kCrLfLen; \
    if (field_value.second) {                                        \
      trim(field_value);                                             \
      values.push_back(field_value);                                 \
    }                                                                \
    if (!HttpUtil::SkipLWS(&buffer_[cursor], end - cursor)) break;   \
  }
// =============================================================================
// HttpRequestDecoder                                                  ( class )
// -----------------------------------------------------------------------------
// This class is in charge of providing the default http request decoder class.
// =============================================================================
class HttpRequestDecoder
    : public transport::ServerTransportDecoder<HttpRequest> {
  // ---------------------------------------------------------------------------
  // USINGs                                                          ( private )
  // ---------------------------------------------------------------------------
  using Indices = std::pair<std::size_t, std::size_t>;

 public:
  // ---------------------------------------------------------------------------
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  // ---------------------------------------------------------------------------
  HttpRequestDecoder() = default;
  HttpRequestDecoder(const HttpRequestDecoder&) = delete;
  HttpRequestDecoder(HttpRequestDecoder&&) noexcept = delete;
  ~HttpRequestDecoder() = default;
  // ---------------------------------------------------------------------------
  // OPERATORs                                                        ( public )
  // ---------------------------------------------------------------------------
  HttpRequestDecoder& operator=(const HttpRequestDecoder&) = delete;
  HttpRequestDecoder& operator=(HttpRequestDecoder&&) noexcept = delete;
  // ---------------------------------------------------------------------------
  // METHODs                                                          ( public )
  // ---------------------------------------------------------------------------
  // Adds the specified buffer fragment to the internal decoder data.
  bool Add(void* buffer, const std::size_t& length) override {
    if (!decoding_body_part_) {
      // if required buffer size is greater than the maximum supported let's
      // return false in order to notify caller for error!
      if ((used_ + length) >= HttpConstants::Limits::kMaxHttpMessageLength) {
        return false;
      }
      memcpy(&buffer_[used_], buffer, length);
      used_ += length;
    } else {
      body_.Write(buffer, length);
    }
    return true;
  }
  // Decodes currently stored data.
  void Decode(
      const transport::ServerTransportDecoder<HttpRequest>::RequestHandler&
          request_handler,
      const transport::ServerTransportDecoder<HttpRequest>::ErrorHandler&
          error_handler,
      const transport::ServerTransportDecoder<HttpRequest>::Sender& sender) {
    bool dispatch_request = false;
    // first, let's check for <basic> message content decoding!
    if (!decoding_body_part_) {
      std::size_t cursor = 0, end = 0;
      auto empty_line_ptr = searchFor(&buffer_[cursor], used_ - cursor,
                                      HttpConstants::Strings::kEmptyLine,
                                      HttpConstants::Strings::kEmptyLineLen);
      // if empty-line sequence (\r\n\r\n) was not found, let's
      // return control and wait for more data to arrive!
      if (empty_line_ptr == nullptr) return;
      // update main section limtis (request-line + headers)!
      end += (empty_line_ptr - &buffer_[cursor]) +
             HttpConstants::Strings::kEmptyLineLen;
      // let's decode the <request-line> section!
      GET_REQUEST_LINE_FIELD(method_, HttpConstants::Strings::kSpace,
                             HttpConstants::Strings::kSpaceLen)
      GET_REQUEST_LINE_FIELD(request_uri_, HttpConstants::Strings::kSpace,
                             HttpConstants::Strings::kSpaceLen)
      GET_REQUEST_LINE_FIELD(http_version_, HttpConstants::Strings::kCrLf,
                             HttpConstants::Strings::kCrLfLen)
      // let's decode the <headers> section!
      headers_.assign(&buffer_[cursor], end - cursor);
      Indices field_name;
      std::vector<Indices> field_values;
      while (cursor < end) {
        GET_HEADER_FIELD_NAME(field_name)
        GET_HEADER_FIELD_VALUES(field_values)
        for (auto const& value : field_values) {
          if (isHeaderFieldName(field_name,
                                HttpConstants::Headers::kContentLength,
                                HttpConstants::Headers::kContentLengthLen)) {
            // [content-length]
            encoding_type_ = HttpEncodingType::kContentLength;
            auto val = std::string(&buffer_[value.first], value.second);
            content_length_ = atol(val.data());
          } else if (isHeaderFieldName(
                         field_name, HttpConstants::Headers::kContentType,
                         HttpConstants::Headers::kContentTypeLen)) {
            // [content-type]
            // ((To-Do)) implement it!
          }
        }
      }
      // we're ready with all <basic> message information!
      auto pending = used_ - end;
      if (pending > 0) {
        if (content_length_) {
          body_.Write((void*)&buffer_[end], pending);
          used_ = 0;
        } else {
          memmove(buffer_, &buffer_[end], pending);
          used_ = pending;
        }
      } else {
        used_ = 0;
      }
      // if we've reach this point then body decoding should be on!
      decoding_body_part_ = true;
    }
    // second, process the body section (if present)..
    if (decoding_body_part_) {
      switch (encoding_type_) {
        case HttpEncodingType::kContentLength:
          dispatch_request = body_.Length() == content_length_;
          break;
        case HttpEncodingType::kMultipart:
          // ((To-Do)) -> to be implemented!
          break;
        case HttpEncodingType::kTransferEncoding:
          // ((To-Do)) -> to be implemented!
          break;
        case HttpEncodingType::kNone:
          dispatch_request = true;
          break;
      }
    }
    // third, prepare request..
    if (dispatch_request) {
      request_handler(
          HttpRequest(std::move(base::Uri(request_uri_)),
                      std::move(HttpMethod(method_)),
                      std::move(HttpHeaders(headers_)), std::move(body_)),
          sender);
      reset();
    }
  }

 private:
  // ---------------------------------------------------------------------------
  // METHODs                                                         ( private )
  // ---------------------------------------------------------------------------
  // Searchs for the required string content within the provided buffer.
  const char* searchFor(const char* buffer, const std::size_t& length,
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
  // Checks if the specified sub-string is a header field name.
  bool isHeaderFieldName(const Indices& delimiters, const char* buffer,
                         const std::size_t& length) {
    if (delimiters.second != length) return false;
    for (std::size_t i = 0; i < length; i++) {
      if (tolower(buffer[i]) != tolower(buffer_[delimiters.first + i])) {
        return false;
      }
    }
    return true;
  }
  // Checks if the specified sub-string is a header field name.
  void trim(Indices& delimiters) {
    bool perform_ltrim = true, perform_rtrim = true;
    auto original_offset = delimiters.first;
    auto original_length = delimiters.second;
    // perform [left && right] trim..
    std::size_t i = 0;
    while (i < original_length && (perform_ltrim || perform_rtrim)) {
      if (perform_ltrim) {
        if (buffer_[original_offset + i] != HttpConstants::Characters::kSpace &&
            buffer_[original_offset + i] != HttpConstants::Characters::kHt) {
          perform_ltrim = false;
        } else {
          delimiters.first++;
          delimiters.second--;
        }
      }
      if (perform_rtrim) {
        if (buffer_[original_offset + (original_length - 1 - i)] !=
                HttpConstants::Characters::kSpace &&
            buffer_[original_offset + (original_length - 1 - i)] !=
                HttpConstants::Characters::kHt) {
          perform_rtrim = false;
        } else {
          delimiters.second--;
        }
      }
      i++;
    }
  }
  // Resets all the internal decoder variables to the initial state.
  void reset() {
    encoding_type_ = HttpEncodingType::kNone;
    decoding_body_part_ = false;
    content_length_ = 0;
  }
  // ---------------------------------------------------------------------------
  // ATTRIBUTEs                                                      ( private )
  // ---------------------------------------------------------------------------
  // internal buffer
  char buffer_[HttpConstants::Limits::kMaxHttpMessageLength] = {0};
  // internal buffer used size
  std::size_t used_ = 0;
  // content-length
  std::size_t content_length_ = 0;
  // body encoding type
  HttpEncodingType encoding_type_ = HttpEncodingType::kNone;
  // body content
  base::AutoBuffer body_;
  // flag used to determine if we're decoding the body segment
  bool decoding_body_part_ = false;
  // http-method
  std::string method_;
  // http-request-uri
  std::string request_uri_;
  // http-version
  std::string http_version_;
  // http-headers
  std::string headers_;
};
}  // namespace koobika::hook::network::protocol::http::v11

#endif