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
//! @brief Http transfer encoding writer
class TransferEncoding {
 public:
  // ___________________________________________________________________________
  // CONSTANTs                                                        ( public )
  //
  //! @brief Data is sent in a series of chunks. The <em>Content-Length</em>
  //! header is omitted in this case and at the beginning of each chunk you
  //! need to add the length of the current chunk in hexadecimal format,
  //! followed by
  //! '\\r\\n' and then the chunk itself, followed by another '\\r\\n'. The
  //! terminating chunk is a regular chunk, with the exception that its length
  //! is zero. It is followed by the trailer, which consists of a (possibly
  //! empty) sequence of header fields.
  static constexpr int kChunked = 1;
  //! @brief A format using the <em>Lempel-Ziv-Welch</em>
  //! (https://en.wikipedia.org/wiki/LZW) LZW algorithm. The value name was
  //! taken from the UNIX compress program, which implemented this algorithm.
  //! Like the compress program, which has disappeared from most UNIX
  //! distributions, this content - encoding is used by almost no browsers
  //! today, partly because of a patent issue(which expired in 2003).
  static constexpr int kCompress = 2;
  //! @brief Using the <em>zlib</em> (https://en.wikipedia.org/wiki/Zlib)
  //! structure (defined in RFC 1950 ->
  //! https://datatracker.ietf.org/doc/html/rfc1950), with the deflate
  //! (https://en.wikipedia.org/wiki/DEFLATE) compression algorithm (defined
  //! in RFC 1951 -> https://datatracker.ietf.org/doc/html/rfc1952).
  static constexpr int kDeflate = 4;
  //! @brief A format using the <em>Lempel-Ziv</em>
  //! (https://en.wikipedia.org/wiki/LZ77_and_LZ78#LZ77) coding (LZ77), with a
  //! 32-bit CRC. This is originally the format of the UNIX gzip program. The
  //! HTTP/1.1 standard also recommends that the servers supporting this
  //! content-encoding should recognize x-gzip as an alias, for compatibility
  //! purposes.
  static constexpr int kGzip = 8;
  //! @brief Indicates the identity function(i.e.no compression, nor
  //! modification).This token, except if explicitly specified, is always
  //! deemed acceptable.
  static constexpr int kIdentity = 16;
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  //! @brief Creates a transfer encoding object instance using the provided
  //! configuration flags.
  //! @param[in] flags configuration flags. A combination of <em>kChunked</em>,
  //! <em>kCompress</em>, <em>kDeflate</em>, <em>kGzip</em> and
  //! <em>kIdentity</em> values.
  //! @see TransferEncoding::kChunked TransferEncoding::kCompress
  //! TransferEncoding::kDeflate TransferEncoding::kGzip
  //! TransferEncoding::kIdentity
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
  //! @brief Enables chunked mode
  //! @returns TransferEncoding& current object reference
  //! @see TransferEncoding::kChunked
  TransferEncoding& EnableChunked() {
    flags_ |= kChunked;
    return *this;
  }
  //! @brief Disables chunked mode
  //! @returns TransferEncoding& current object reference
  //! @see TransferEncoding::kChunked
  TransferEncoding& DisableChunked() {
    flags_ &= ~kChunked;
    return *this;
  }
  //! @brief Enables compress mode
  //! @returns TransferEncoding& current object reference
  //! @see TransferEncoding::kCompress
  //! @remarks <b>Currently NOT supported</b>
  TransferEncoding& EnableCompress() {
    flags_ |= kCompress;
    return *this;
  }
  //! @brief Disables compress mode
  //! @returns TransferEncoding& current object reference
  //! @see TransferEncoding::kCompress
  //! @remarks <b>Currently NOT supported</b>
  TransferEncoding& DisableCompress() {
    flags_ &= ~kCompress;
    return *this;
  }
  //! @brief Enables deflate mode
  //! @returns TransferEncoding& current object reference
  //! @see TransferEncoding::kDeflate
  //! @remarks <b>Currently NOT supported</b>
  TransferEncoding& EnableDeflate() {
    flags_ |= kDeflate;
    return *this;
  }
  //! @brief Disables deflate mode
  //! @returns TransferEncoding& current object reference
  //! @see TransferEncoding::kDeflate
  //! @remarks <b>Currently NOT supported</b>
  TransferEncoding& DisableDeflate() {
    flags_ &= ~kCompress;
    return *this;
  }
  //! @brief Enables gzip mode
  //! @returns TransferEncoding& current object reference
  //! @see TransferEncoding::kGzip
  //! @remarks <b>Currently NOT supported</b>
  TransferEncoding& EnableGzip() {
    flags_ |= kGzip;
    return *this;
  }
  //! @brief Disables gzip mode
  //! @returns TransferEncoding& current object reference
  //! @see TransferEncoding::kGzip
  //! @remarks <b>Currently NOT supported</b>
  TransferEncoding& DisableGzip() {
    flags_ &= ~kGzip;
    return *this;
  }
  //! @brief Enables identity mode
  //! @returns TransferEncoding& current object reference
  //! @see TransferEncoding::kIdentity
  TransferEncoding& EnableIdentity() {
    flags_ |= kIdentity;
    return *this;
  }
  //! @brief Disables identity mode
  //! @returns TransferEncoding& current object reference
  //! @see TransferEncoding::kIdentity
  TransferEncoding& DisableIdentity() {
    flags_ &= ~kGzip;
    return *this;
  }
  //! @brief Writes content provided by <em>buffer</em> to the <em>res</em>
  //! object using Transfer-Encoding mode
  //! @param[out] res Response to update
  //! @param[in] buffer Autobuffer containing the data to write
  //! @remarks <b>kCompress, kDeflate and kGzip modes currently NOT
  //! supported</b>
  //! @section Example
  //! @snippet network/http_server_response_builder_transfer_encoding.cpp Ex
  void Write(http::Response& res, const base::AutoBuffer& buffer) const {
    bool using_compression = false;
    if (flags_ & kIdentity) {
      res.Body.Write(buffer);
      return;
    }
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