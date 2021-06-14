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
// network/protocol/http/response_writers/transfer_encoding.h
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

#ifndef koobika_hook_network_protocol_http_responsewriters_transferencoding_h
#define koobika_hook_network_protocol_http_responsewriters_transferencoding_h

#include "base/auto_buffer.h"
#include "network/protocol/http/response.h"

namespace koobika::hook::network::protocol::http::response_writers {
// =============================================================================
// TransferEncoding                                                    ( class )
// -----------------------------------------------------------------------------
// This specification holds for the transfer-encoding http response writer class
// =============================================================================
class TransferEncoding {
 public:
  // ___________________________________________________________________________
  // CONSTANTs                                                        ( public )
  //
  static constexpr int kChunked = 1;
  static constexpr int kCompress = 2;
  static constexpr int kDeflate = 4;
  static constexpr int kGzip = 8;
  static constexpr int kIdentity = 16;
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  TransferEncoding(const int& flags = kChunked) : flags_{flags} {}
  TransferEncoding(const TransferEncoding&) = default;
  TransferEncoding(TransferEncoding&&) noexcept = default;
  ~TransferEncoding() = default;
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  //
  TransferEncoding& operator=(const TransferEncoding&) = default;
  TransferEncoding& operator=(TransferEncoding&&) noexcept = default;
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  // Enables chunked mode!
  TransferEncoding& EnableChunked() {
    flags_ |= kChunked;
    return *this;
  }
  // Disables chunked mode!
  TransferEncoding& DisableChunked() {
    flags_ &= ~kChunked;
    return *this;
  }
  // Writes content to an auto-buffer without performing formatting.
  void Write(http::Response& res, const base::AutoBuffer& buffer) const {
    bool using_compression = false;
    if (flags_ & kCompress) {
      using_compression = true;
      // ((To-Do)) -> implement it!
    }
    if (!using_compression && flags_ & kDeflate) {
      using_compression = true;
      // ((To-Do)) -> implement it!
    }
    if (!using_compression && flags_ & kGzip) {
      using_compression = true;
      // ((To-Do)) -> implement it!
    }
    if (flags_ & kChunked) {
      res.Headers.Set(constants::Headers::kTransferEncoding, kChunkedStr);
      std::size_t length = buffer.Length(), offset = 0;
      char* tmp = new char[chunk_size_];
      while (offset < length) {
        auto read = buffer.ReadSome(tmp, chunk_size_);
        std::stringstream ss;
        ss << std::hex << read;
        res.Body.Write(ss.str())
            .Write(constants::Strings::kCrLf)
            .Write(tmp, read)
            .Write(constants::Strings::kCrLf);
        offset += read;
      }
      res.Body.Write("0").Write(constants::Strings::kEmptyLine);
      delete[] tmp;
    }
  }

 protected:
  // ___________________________________________________________________________
  // CONSTANTs                                                        ( public )
  //
  static constexpr std::size_t kDefaultChunkSize = 65536;
  static constexpr char kChunkedStr[] = "chunked";
  // ___________________________________________________________________________
  // ATTRIBUTEs                                                       ( public )
  // 
  int flags_;
  std::size_t chunk_size_ = kDefaultChunkSize;
};
}  // namespace koobika::hook::network::protocol::http::response_writers

#endif