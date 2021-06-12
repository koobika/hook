﻿// /////////////////////////////////////////////////////////////////////////////
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
// network/protocol/http/auth/modules/api_key.h
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

#ifndef koobika_hook_network_protocol_http_auth_modules_apikey_h
#define koobika_hook_network_protocol_http_auth_modules_apikey_h

#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>

#include "encoding/base64/decoder.h"
#include "encoding/base64/encoder.h"
#include "network/protocol/http/auth/context_api_key.h"
#include "network/protocol/http/auth/controller.h"
#include "network/protocol/http/routes_controller.h"
#include "network/protocol/http/util.h"

namespace koobika::hook::network::protocol::http::auth::modules {
// =============================================================================
// ApiKey                                                              ( class )
// -----------------------------------------------------------------------------
// This specification holds for <apikey-auth> module
// =============================================================================
class ApiKey : public Controller<ContextApiKey> {
 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  ApiKey(const Checker& user_checker = nullptr,
         const double session_timeout_in_seconds = kDefaultTimeout)
      : timeout_in_seconds_{session_timeout_in_seconds} {
    this->Check = (user_checker != nullptr)
                      ? user_checker
                      : [this](const Context& context) -> bool {
      do {
        auto itr = data_.find(context.Token);
        if (itr == data_.end()) break;
        if (timeout_in_seconds_ > 0.0) {
          auto current = std::chrono::system_clock().now();
          std::chrono::duration<double> diff = current - itr->second;
          if (diff.count() >= timeout_in_seconds_) break;
        }
        return true;
      } while (false);
      return false;
    };
  }
  ApiKey(const ApiKey&) = default;
  ApiKey(ApiKey&&) noexcept = default;
  ~ApiKey() = default;
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  //
  ApiKey& operator=(const ApiKey&) = default;
  ApiKey& operator=(ApiKey&&) noexcept = default;
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  // Sets token within the database.
  void Set(const std::string& token) {
    data_[token] = std::chrono::system_clock().now();
  }
  // Clears specific token from database.
  void Clear(const std::string& token) { data_.erase(token); }
  // Clears entire tokens database.
  void Clear() { data_.clear(); }

 protected:
  // ___________________________________________________________________________
  // CONSTANTs                                                     ( protected )
  //
  static constexpr double kDefaultTimeout = -1.0;  // disabled by default
  // ___________________________________________________________________________
  // ATTRIBUTEs                                                    ( protected )
  //
  std::unordered_map<std::string, std::chrono::system_clock::time_point> data_;
  double timeout_in_seconds_;
};
}  // namespace koobika::hook::network::protocol::http::auth::modules

#endif