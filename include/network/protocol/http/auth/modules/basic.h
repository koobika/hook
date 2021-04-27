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

#ifndef koobika_hook_network_protocol_http_auth_basic_h
#define koobika_hook_network_protocol_http_auth_basic_h

#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>

#include "auth/controller.h"
#include "encoding/base64/decoder.h"
#include "encoding/base64/encoder.h"
#include "network/protocol/http/auth/context_basic.h"
#include "network/protocol/http/auth/controller.h"
#include "network/protocol/http/http_controller.h"
#include "network/protocol/http/http_routes_types.h"
#include "network/protocol/http/http_util.h"

namespace koobika::hook::network::protocol::http::auth::modules {
// =============================================================================
// Basic                                                               ( class )
// -----------------------------------------------------------------------------
// This specification holds for <basic-auth> module
// =============================================================================
class Basic : public Controller<ContextBasic> {
 public:
  // ---------------------------------------------------------------------------
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  // ---------------------------------------------------------------------------
  Basic(const typename Checker& user_checker = nullptr,
        const double session_timeout_in_seconds = kDefaultTimeout)
      : timeout_in_seconds_{session_timeout_in_seconds} {
    this->Check = (user_checker != nullptr)
                      ? user_checker
                      : [this](const Context& context) -> bool {
      do {
        auto itr = data_.find(context.Username);
        if (itr == data_.end() || itr->second.first != context.Password) break;
        if (timeout_in_seconds_ > 0.0) {
          auto current = std::chrono::system_clock().now();
          std::chrono::duration<double> diff = current - itr->second.second;
          if (diff.count() >= timeout_in_seconds_) break;
        }
        return true;
      } while (false);
      return false;
    };
  }
  Basic(const Basic&) = default;
  Basic(Basic&&) noexcept = default;
  ~Basic() = default;
  // ---------------------------------------------------------------------------
  // OPERATORs                                                        ( public )
  // ---------------------------------------------------------------------------
  Basic& operator=(const Basic&) = default;
  Basic& operator=(Basic&&) noexcept = default;
  // ---------------------------------------------------------------------------
  // METHODs                                                          ( public )
  // ---------------------------------------------------------------------------
  // Sets user/password tuple within the database.
  void Set(const std::string& user, const std::string& password) {
    data_[user] = std::make_pair(password, std::chrono::system_clock().now());
  }
  // Clears specific user from database.
  void Clear(const std::string& user) { data_.erase(user); }
  // Clears entire user/password database.
  void Clear() { data_.clear(); }

 protected:
  // ---------------------------------------------------------------------------
  // CONSTANTs                                                     ( protected )
  // ---------------------------------------------------------------------------
  static constexpr double kDefaultTimeout = -1.0;  // disabled by default
  // ---------------------------------------------------------------------------
  // ATTRIBUTEs                                                    ( protected )
  // ---------------------------------------------------------------------------
  std::unordered_map<
      std::string,
      std::pair<std::string, std::chrono::system_clock::time_point>>
      data_;
  double timeout_in_seconds_;
};
}  // namespace koobika::hook::network::protocol::http::auth::modules

#endif