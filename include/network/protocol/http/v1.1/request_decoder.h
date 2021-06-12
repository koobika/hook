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
// network/protocol/http/v1.1/request_decoder.h
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

#ifndef koobika_hook_network_protocol_http_v11_requestdecoder_h
#define koobika_hook_network_protocol_http_v11_requestdecoder_h

#include <utility>
#include <functional>
#include <string>

#include "base/auto_buffer.h"
#include "network/protocol/http/constants/characters.h"
#include "network/protocol/http/constants/headers.h"
#include "network/protocol/http/constants/limits.h"
#include "network/protocol/http/constants/strings.h"
#include "network/protocol/http/encoding_type.h"
#include "network/protocol/http/request.h"
#include "network/protocol/http/response.h"
#include "network/transport/server_decoder.h"

namespace koobika::hook::network::protocol::http::v11 {
// =============================================================================
// RequestDecoder                                                      ( class )
// -----------------------------------------------------------------------------
// This class is in charge of providing the default http request decoder class.
// =============================================================================
class RequestDecoder : public transport::ServerDecoder<Request, Response> {
 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  RequestDecoder() {
    buffer_ = new char[constants::Limits::kMaxHttpMessageLength];
    request_ = std::make_unique<Request>();
    response_ = std::make_unique<Response>();
  }
  RequestDecoder(const RequestDecoder&) = delete;
  RequestDecoder(RequestDecoder&&) noexcept = delete;
  ~RequestDecoder() { delete[] buffer_; }
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  //
  RequestDecoder& operator=(const RequestDecoder&) = delete;
  RequestDecoder& operator=(RequestDecoder&&) noexcept = delete;
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  // Adds the specified buffer fragment to the internal decoder data.
  bool Add(void* buffer, const std::size_t& length) override {
    if (!decoding_body_part_) {
      // if required buffer size is greater or equal than the maximum supported
      // let's return false in order to notify caller for error!
      if ((used_ + length) >= constants::Limits::kMaxHttpMessageLength) {
        return false;
      }
      memcpy(&buffer_[used_], buffer, length);
      used_ += length;
    } else {
      request_->Body.Write(buffer, length);
    }
    return true;
  }
  // Decodes currently stored data.
  void Decode(
      const transport::ServerDecoder<Request, Response>::RequestHandler& func,
      const transport::ServerDecoder<Request, Response>::ErrorHandler& err,
      const transport::ServerDecoder<Request, Response>::Sender& sender) {
    do {
      if (decoding_body_part_) break;
      buffer_[used_] = 0;
      auto section = buffer_;
      // decoding <request-line:method>..
      auto cur = strchr(section, constants::Characters::kSpace);
      if (!cur) break;
      request_->Method.From(section, cur - section);
      section = ++cur;
      // decoding <request-line:uri>..
      cur = strchr(section, constants::Characters::kSpace);
      if (!cur) break;
      request_->Uri.From(section, cur - section);
      section = ++cur;
      // decoding <request-line:uri>..
      cur = strchr(section, constants::Characters::kCr);
      if (!cur || *(cur + 1) != constants::Characters::kLf) break;
      request_->HttpVersion.assign(section, cur - section);
      section = cur + constants::Strings::kCrLfLen;
      // decoding <headers>..
      cur = strstr(section, constants::Strings::kEmptyLine);
      if (!cur) break;
      request_->Headers.From(section, cur - section);
      section = cur + constants::Strings::kEmptyLineLen;
      // ok!, we're ready with all <basic> message information!
      decoding_body_part_ = true;
      auto end = section - buffer_;
      auto pending = used_ - end;
      if (pending <= 0) {
        used_ = 0;
        break;
      }
      if (!content_length_) {
        memmove(buffer_, &buffer_[end], pending);
        used_ = pending;
        break;
      }
      request_->Body.Write((void*)&buffer_[end], pending);
      used_ = 0;
    } while (false);
    // second, let's check for the <body-decoding> state..
    if (!decoding_body_part_) return;
    bool dispatch_request = false;
    switch (encoding_type_) {
      case EncodingType::kContentLength:
        dispatch_request = request_->Body.Length() == content_length_;
        break;
      case EncodingType::kMultipart:
        // ((To-Do)) -> to be implemented!
        break;
      case EncodingType::kTransferEncoding:
        // ((To-Do)) -> to be implemented!
        break;
      case EncodingType::kNone:
        dispatch_request = true;
        break;
    }
    // third, call the request handler..
    if (!dispatch_request) return;
    func(*request_, *response_, sender);
    // fourth, reset internal objects..
    encoding_type_ = EncodingType::kNone;
    decoding_body_part_ = false;
    content_length_ = 0;
    request_->Body.Clear();
    request_->Headers.Clear();
    response_->Body.Clear();
    response_->Raw.Clear();
    response_->Headers.Clear();
  }

 private:
  // ___________________________________________________________________________
  // ATTRIBUTEs                                                      ( private )
  //
  // internal buffer
  char* buffer_ = nullptr;
  // internal buffer used size
  std::size_t used_ = 0;
  // content-length
  std::size_t content_length_ = 0;
  // body encoding type
  EncodingType encoding_type_ = EncodingType::kNone;
  // flag used to determine if we're decoding the body segment
  bool decoding_body_part_ = false;
  // http-request
  std::unique_ptr<Request> request_;
  // http-response
  std::unique_ptr<Response> response_;
};
}  // namespace koobika::hook::network::protocol::http::v11

#endif