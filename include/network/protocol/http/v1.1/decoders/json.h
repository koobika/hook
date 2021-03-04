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

#ifndef koobika_hook_network_protocol_http_v11_decoders_json_h
#define koobika_hook_network_protocol_http_v11_decoders_json_h

#include <stdexcept>
#include <string>

#include "base/stream.h"
#include "structured/json/json_value.h"

namespace koobika::hook::network::protocol::http::v11::decoders {
// =============================================================================
// Json                                                                ( class )
// -----------------------------------------------------------------------------
// This class is in charge of providing the default json body decoding
// =============================================================================
class Json {
 public:
  // ---------------------------------------------------------------------------
  // METHODs                                                          ( public )
  // ---------------------------------------------------------------------------
  // Decodes content stored within the provided body-stream.
  static std::optional<structured::json::JsonValue> Decode(
      const base::Stream& stream) {
    std::string content;
    stream.ReadAll(content);
    if (!content.length()) {
      // ((Error)) -> while trying to access buffer!
      throw std::logic_error(
          "Unable to read specified buffer: content is not available!");
    }
    return koobika::hook::structured::json::JsonValue::From(content);
  }
  // Decodes content stored within the provided body-stream.
  static std::optional<structured::json::JsonValue> Decode(
      const std::shared_ptr<base::Stream>& stream) {
    if (stream == nullptr) {
      // ((Error)) -> while trying to access an empty shared pointer!
      throw std::logic_error(
          "Unable to decode specified buffer: Invalid stream!");
    }
    return Decode(*stream);
  }
};
}  // namespace koobika::hook::network::protocol::http::v11::decoders

#endif