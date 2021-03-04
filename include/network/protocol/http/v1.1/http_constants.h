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

#ifndef koobika_hook_network_protocol_http_v11_httpconstants_h
#define koobika_hook_network_protocol_http_v11_httpconstants_h

#include <cstddef>

#include "http_method_value.h"

namespace koobika::hook::network::protocol::http::v11 {
// =============================================================================
// HttpConstants                                                       ( class )
// -----------------------------------------------------------------------------
// This specification holds for all http-module constants.
// =============================================================================
class HttpConstants {
 public:
  // ---------------------------------------------------------------------------
  // LIMITs                                                           ( public )
  // ---------------------------------------------------------------------------
  class Limits {
   public:
    // Maximum length for the http message section (body excluded)! (in bytes).
    static constexpr std::size_t kMaxHttpMessageLength = 16384;
  };
  // ---------------------------------------------------------------------------
  // STRINGs                                                          ( public )
  // ---------------------------------------------------------------------------
  class Strings {
   public:
    // Http-version.
    static constexpr char kHttpVersion[] = "HTTP/1.1";
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
    static constexpr char kHeaderFieldNameSeparator[] = ":";
    static constexpr std::size_t kHeaderFieldNameSeparatorLen = 1;
    // Header (field-value) separator.
    static constexpr char kHeaderFieldValueSeparator[] = ",";
    static constexpr std::size_t kHeaderFieldValueSeparatorLen = 1;
    // Header (chunked-content) end delimiter.
    static constexpr char kHeaderChunkedContentEnd[] = "0\r\n\r\n";
    static constexpr std::size_t kHeaderChunkedContentEndLen = 5;
  };
  // ---------------------------------------------------------------------------
  // CHARACTERs                                                       ( public )
  // ---------------------------------------------------------------------------
  class Characters {
   public:
    // Space.
    static constexpr char kSpace = ' ';
    // Tab.
    static constexpr char kHt = '\t';
    // Carriage return.
    static constexpr char kCr = '\r';
    // Line feed.
    static constexpr char kLf = '\n';
  };
  // ---------------------------------------------------------------------------
  // HEADERs                                                          ( public )
  // ---------------------------------------------------------------------------
  class Headers {
   public:
    // Content-type.
    static constexpr char kContentType[] = "Content-Type";
    static constexpr std::size_t kContentTypeLen = 12;
    // Content-length.
    static constexpr char kContentLength[] = "Content-Length";
    static constexpr std::size_t kContentLengthLen = 14;
    // Transfer-encoding.
    static constexpr char kTransferEncoding[] = "Transfer-Encoding";
    static constexpr std::size_t kTransferEncodingLen = 17;
  };
  // ---------------------------------------------------------------------------
  // METHODs                                                          ( public )
  // ---------------------------------------------------------------------------
  class Methods {
   public:
    // Options.
    static constexpr HttpMethodValue kOptions = 1;
    // Get.
    static constexpr HttpMethodValue kGet = 2;
    // Head.
    static constexpr HttpMethodValue kHead = 4;
    // Post.
    static constexpr HttpMethodValue kPost = 8;
    // Put.
    static constexpr HttpMethodValue kPut = 16;
    // Delete.
    static constexpr HttpMethodValue kDelete = 32;
    // Trace.
    static constexpr HttpMethodValue kTrace = 64;
    // Connect.
    static constexpr HttpMethodValue kConnect = 128;
    // Extension.
    static constexpr HttpMethodValue kExtension = 256;
    // All.
    static constexpr HttpMethodValue kAll = kOptions | kGet | kHead | kPost |
                                            kPut | kDelete | kTrace | kConnect |
                                            kExtension;
  };
};
}  // namespace koobika::hook::network::protocol::http::v11

#endif