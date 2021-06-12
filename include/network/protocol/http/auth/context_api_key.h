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
// network/protocol/http/auth/context_api_key.h
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

#ifndef koobika_hook_network_protocol_http_auth_contextapikey_h
#define koobika_hook_network_protocol_http_auth_contextapikey_h

#include "mapper.h"
#include "context.h"
#include "encoding/base64/decoder.h"
#include "network/protocol/http/http_util.h"

namespace koobika::hook::network::protocol::http::auth {
// =============================================================================
// ContextApiKey                                                       ( class )
// -----------------------------------------------------------------------------
// This specification holds for <apikey-authorization> context module
// =============================================================================
class ContextApiKey : public Context, public Mapper {
 public:
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  // 
  // Tries to fill-up internal structures using the provided request.
  bool Map(const HttpRequest& req) override {
    if (!req.Headers.Exist(kApiKeyField)) {
      return false;
    }
    Token = req.Headers.Get(kApiKeyField);
    return true;
  }
  // ___________________________________________________________________________
  // PROPERTIEs                                                       ( public )
  // 
  std::string Token;

 private:
  // ___________________________________________________________________________
  // CONSTANTs                                                       ( private )
  // 
  static constexpr char kApiKeyField[] = "x-api-key";
};
}  // namespace koobika::hook::network::protocol::http::auth

#endif