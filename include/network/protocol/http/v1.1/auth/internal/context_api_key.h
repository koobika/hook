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

#ifndef koobika_hook_network_protocol_http_v11_auth_internal_contextapikey_h
#define koobika_hook_network_protocol_http_v11_auth_internal_contextapikey_h

#include "context.h"
#include "encoding/base64/decoder.h"
#include "mapper.h"
#include "network/protocol/http/v1.1/http_util.h"

namespace koobika::hook::network::protocol::http::v11::auth::internal {
// =============================================================================
// ContextApiKey                                                       ( class )
// -----------------------------------------------------------------------------
// This specification holds for <apikey-authorization> context module
// =============================================================================
class ContextApiKey : public Context, public Mapper {
 public:
  // ---------------------------------------------------------------------------
  // METHODs                                                          ( public )
  // ---------------------------------------------------------------------------
  // Tries to fill-up internal structures using the provided request.
  bool Map(typename HttpRoutesTypes::Request req) override {
    auto auth_field = req.Headers.Get(kApiKeyField);
    if (!auth_field.has_value()) {
      auth_field = req.Uri.GetQuery().Get(kApiKeyField);
      if (!auth_field.has_value()) return false;
    }
    Request = req;
    Token = auth_field.value();
    return true;
  }
  // ---------------------------------------------------------------------------
  // PROPERTIEs                                                       ( public )
  // ---------------------------------------------------------------------------
  std::string Token;

 private:
  // ---------------------------------------------------------------------------
  // CONSTANTs                                                       ( private )
  // ---------------------------------------------------------------------------
  static constexpr char kApiKeyField[] = "x-api-key";
};
}  // namespace koobika::hook::network::protocol::http::v11::auth::internal

#endif