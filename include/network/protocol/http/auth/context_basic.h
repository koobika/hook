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
// network/protocol/http/auth/context_basic.h
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

#ifndef koobika_hook_network_protocol_http_auth_contextbasic_h
#define koobika_hook_network_protocol_http_auth_contextbasic_h

#include <regex>

#include "mapper.h"
#include "context.h"
#include "encoding/base64/decoder.h"
#include "network/protocol/http/request.h"
#include "network/protocol/http/util.h"
#include "network/protocol/http/constants/characters.h"

namespace koobika::hook::network::protocol::http::auth {
// =============================================================================
// ContextBasic                                                        ( class )
// -----------------------------------------------------------------------------
// This specification holds for <basic-authorization> context module
// =============================================================================
class ContextBasic : public Context, public Mapper {
 public:
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  // Tries to fill-up internal structures using the provided request.
  bool Map(const Request& req) override {
    if (!req.Headers.Exist(kAuthorization)) return false;
    auto auth = req.Headers.Get(kAuthorization);
    std::regex reg("\\s+");
    std::sregex_token_iterator iter(auth.begin(), auth.end(), reg, -1), end;
    std::vector<std::string> vec(iter, end);
    if (vec.size() != 0x2) return false;  // "Basic <base64-encoded-data>"..
    if (!Util::Compare(vec[0], kBasic, false)) return false;
    auto decoded = encoding::base64::Decoder::Decode(vec[1]);
    auto colon_pos = decoded.find(constants::Characters::kColon);
    if (colon_pos == std::string::npos) return false;
    Username = decoded.substr(0, colon_pos);
    Password = decoded.substr(colon_pos + 1);
    return true;
  }
  // ___________________________________________________________________________
  // PROPERTIEs                                                       ( public )
  //
  std::string Username;
  std::string Password;

 private:
  // ___________________________________________________________________________
  // CONSTANTs                                                       ( private )
  //
  static constexpr char kBasic[] = "Basic";
  static constexpr char kAuthorization[] = "Authorization";
};
}  // namespace koobika::hook::network::protocol::http::auth

#endif