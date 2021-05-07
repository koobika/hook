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
// network/protocol/http/http_controller_handler.h
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

#ifndef koobika_hook_network_protocol_http_httpcontrollerhandler_h
#define koobika_hook_network_protocol_http_httpcontrollerhandler_h

#include <regex>
#include <string>

#include "http_router.h"

namespace koobika::hook::network::protocol::http {
// =============================================================================
// HttpControllerHandler                                               ( class )
// -----------------------------------------------------------------------------
// This class is in charge of providing the http controller handler class.
// =============================================================================
class HttpControllerHandler {
 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  // 
  HttpControllerHandler(HttpRouter* parent, const std::string& route,
                        const typename HttpRoutesTypes::Handler& handler,
                        const HttpMethodValue& method) {
    parent->Handle(route, handler, method);
  }
  HttpControllerHandler(HttpRouter* parent, const std::regex& regex,
                        const typename HttpRoutesTypes::Handler& handler,
                        const HttpMethodValue& method) {
    parent->Handle(regex, handler, method);
  }
  HttpControllerHandler(const HttpControllerHandler&) = delete;
  HttpControllerHandler(HttpControllerHandler&&) noexcept = delete;
  virtual ~HttpControllerHandler() = default;
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  // 
  HttpControllerHandler& operator=(const HttpControllerHandler&) = delete;
  HttpControllerHandler& operator=(HttpControllerHandler&&) noexcept = delete;
};

// =============================================================================
// HTTP_CONTROLLER_DEF                                                 ( macro )
// -----------------------------------------------------------------------------
// This macro will help creating shortcuts for handlers.
// =============================================================================
#define HTTP_CONTROLLER_DEF(METHOD)                                          \
  class HttpController##METHOD : public HttpControllerHandler {              \
   public:                                                                   \
    HttpController##METHOD(HttpRouter* parent, const std::string& route,     \
                           const typename HttpRoutesTypes::Handler& handler) \
        : HttpControllerHandler(parent, route, handler,                      \
                                HttpConstants::Methods::k##METHOD) {}        \
    HttpController##METHOD(HttpRouter* parent, const std::regex& regex,      \
                           const typename HttpRoutesTypes::Handler& handler) \
        : HttpControllerHandler(parent, regex, handler,                      \
                                HttpConstants::Methods::k##METHOD) {}        \
  };
// =============================================================================
// Helpers (shortcuts)
// =============================================================================
HTTP_CONTROLLER_DEF(Options)
HTTP_CONTROLLER_DEF(Get)
HTTP_CONTROLLER_DEF(Head)
HTTP_CONTROLLER_DEF(Post)
HTTP_CONTROLLER_DEF(Put)
HTTP_CONTROLLER_DEF(Delete)
HTTP_CONTROLLER_DEF(Trace)
HTTP_CONTROLLER_DEF(Connect)
HTTP_CONTROLLER_DEF(Extension)
}  // namespace koobika::hook::network::protocol::http

#endif