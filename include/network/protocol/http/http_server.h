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
// network/protocol/http/http_server.h
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

#ifndef koobika_hook_network_protocol_http_httpserver_h
#define koobika_hook_network_protocol_http_httpserver_h

#include "http_request.h"
#include "http_response.h"
#include "http_router.h"
#include "http_server_base.h"
#include "v1.1/http_request_decoder.h"
#include "network/transport/tcpip/server_transport_tcp_ip.h"

namespace koobika::hook::network::protocol::http {
// =============================================================================
// HttpServer                                                          ( class )
// -----------------------------------------------------------------------------
// This class is in charge of providing the default http server base class.
// =============================================================================
class HttpServer
    : public HttpServerBase<
          transport::tcpip::ServerTransportTcpIp<v11::HttpRequestDecoder>,
          HttpRouter> {
 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  // 
  HttpServer(const int& workers_number) : HttpServerBase(workers_number) {}
  HttpServer(const structured::json::Object& configuration)
      : HttpServerBase(configuration) {}
  HttpServer(const HttpServer&) = delete;
  HttpServer(HttpServer&&) noexcept = delete;
  ~HttpServer() = default;
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  // 
  HttpServer& operator=(const HttpServer&) = delete;
  HttpServer& operator=(HttpServer&&) noexcept = delete;
};
}  // namespace koobika::hook::network::protocol::http

#endif