﻿// /////////////////////////////////////////////////////////////////////////////
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

#ifndef koobika_hook_network_protocol_http_v11_httprequest_h
#define koobika_hook_network_protocol_http_v11_httprequest_h

#include "base/stream.h"
#include "base/uri.h"
#include "http_headers.h"
#include "http_method.h"

namespace koobika::hook::network::protocol::http::v11 {
// =============================================================================
// HttpRequest                                                         [ class ]
// -----------------------------------------------------------------------------
// This class is in charge of providing the http request class
// =============================================================================
class HttpRequest {
 public:
  // ---------------------------------------------------------------------------
  // Constructors/Destructors                                         [ public ]
  // ---------------------------------------------------------------------------
  HttpRequest(base::Uri&& uri, HttpMethod&& method, HttpHeaders&& headers,
              base::Stream&& body)
      : Uri{std::move(uri)},
        Method{std::move(method)},
        Headers{std::move(headers)},
        Body{std::move(body)} {}
  HttpRequest(const HttpRequest&) = delete;
  HttpRequest(HttpRequest&&) noexcept = delete;
  ~HttpRequest() = default;
  // ---------------------------------------------------------------------------
  // Operators                                                        [ public ]
  // ---------------------------------------------------------------------------
  HttpRequest& operator=(const HttpRequest&) = delete;
  HttpRequest& operator=(HttpRequest&&) noexcept = delete;
  // ---------------------------------------------------------------------------
  // Attributes                                                       [ public ]
  // ---------------------------------------------------------------------------
  base::Uri Uri;
  HttpMethod Method;
  HttpHeaders Headers;
  base::Stream Body;
};
}  // namespace koobika::hook::network::protocol::http::v11

#endif