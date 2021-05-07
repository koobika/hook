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
// network/protocol/http/http_request.h
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

#ifndef koobika_hook_network_protocol_http_httprequest_h
#define koobika_hook_network_protocol_http_httprequest_h

#include "base/auto_buffer.h"
#include "base/uri.h"
#include "http_headers.h"
#include "http_method.h"

namespace koobika::hook::network::protocol::http {
// =============================================================================
// HttpRequest                                                         ( class )
// -----------------------------------------------------------------------------
// This class is in charge of providing the http request class
// =============================================================================
class HttpRequest {
 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  // 
  HttpRequest() = default;
  HttpRequest(base::Uri&& uri, HttpMethod&& method, HttpHeaders&& headers,
              base::AutoBuffer&& body)
      : Uri{std::move(uri)},
        Method{std::move(method)},
        Headers{std::move(headers)},
        Body{std::move(body)} {}
  HttpRequest(const HttpRequest&) = default;
  HttpRequest(HttpRequest&&) noexcept = default;
  ~HttpRequest() = default;
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  // 
  HttpRequest& operator=(const HttpRequest&) = default;
  HttpRequest& operator=(HttpRequest&&) noexcept = default;
  // ___________________________________________________________________________
  // PROPERTIEs                                                       ( public )
  // 
  base::Uri Uri;
  HttpMethod Method;
  HttpHeaders Headers;
  base::AutoBuffer Body;
};
}  // namespace koobika::hook::network::protocol::http

#endif