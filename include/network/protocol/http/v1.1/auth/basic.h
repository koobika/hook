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

#ifndef koobika_hook_network_protocol_http_v11_auth_basic_h
#define koobika_hook_network_protocol_http_v11_auth_basic_h

#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

#include "encoding/base64/decoder.h"
#include "encoding/base64/encoder.h"
#include "network/protocol/http/v1.1/http_auth_module.h"
#include "network/protocol/http/v1.1/http_routes_types.h"

namespace koobika::hook::network::protocol::http::v11::auth {
// =============================================================================
// Basic                                                               ( class )
// -----------------------------------------------------------------------------
// This specification holds for <basic-authorization> module
// =============================================================================
template <typename RQty, typename RSty,
          template <typename KEty, typename VAty> typename RPty>
class Basic : public HttpAuthModule<RQty, RSty> {
 public:
  // ---------------------------------------------------------------------------
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  // ---------------------------------------------------------------------------
  Basic(const double& session_time_out_in_secs = kDefaultTimeout)
      : timeout_in_seconds_{session_time_out_in_secs} {}
  Basic(const std::initializer_list<std::string>& in,
        const double& session_time_out_in_secs = kDefaultTimeout) {
    timeout_in_seconds_ = session_time_out_in_secs;
    for (auto const& element : in) {
      set(element);
    }
  }
  Basic(const std::initializer_list<std::pair<std::string, std::string>>& in,
        const double& session_time_out_in_secs = kDefaultTimeout) {
    timeout_in_seconds_ = session_time_out_in_secs;
    for (auto const& element : in) {
      set(element.first, element.second);
    }
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
  // Sets a new (clean) user/password tuple to the database.
  void Set(const std::string& user, const std::string& password) {
    std::stringstream dat;
    dat << user << ':' << password;
    repository_.set(encoding::base64::Encoder::Encode(dat.str()),
                    std::chrono::system_clock().now());
  }
  // Sets a new (encoded) user/password tuple to the database.
  void Set(const std::string& user_and_pass_encoded) {
    repository_.set(user_and_pass_encoded, std::chrono::system_clock().now());
  }
  // Clears user/password database.
  void Clear() { repository_.clear(); }
  // Checks for the incoming request to be an authorized one.
  bool Check(typename HttpRoutesTypes<RQty, RSty>::Request req,
             typename HttpRoutesTypes<RQty, RSty>::Response res) override {
    do {
      auto header_segment = req.Headers.get(kAuthorization);
      if (!header_segment.has_value()) break;
      std::string data(header_segment.value());
      std::size_t type_delimiter = data.find_first_of(" \t");
      if (type_delimiter == std::string::npos) break;
      std::string type = data.substr(0, type_delimiter);
      if (type != kBasic) break;
      std::string content = data.substr(type_delimiter + 1);
      if (!content.length()) break;
      auto epoch = repository_.get(content);
      if (!epoch.has_value()) break;
      if (timeout_in_seconds_ > 0.0) {
        auto current = std::chrono::system_clock().now();
        std::chrono::duration<double> diff = current - epoch.value();
        if (diff.count() > timeout_in_seconds_) break;
      }
      return true;
    } while (false);
    return false;
  }

 private:
  // ---------------------------------------------------------------------------
  // CONSTANTs                                                       ( private )
  // ---------------------------------------------------------------------------
  static constexpr char kBasic[] = "Basic";
  static constexpr char kAuthorization[] = "Authorization";
  static constexpr double kDefaultTimeout = -1.0;  // disabled by default
  // ---------------------------------------------------------------------------
  // ATTRIBUTEs                                                      ( private )
  // ---------------------------------------------------------------------------
  RPty<std::string, std::chrono::system_clock::time_point> repository_;
  double timeout_in_seconds_;
};
}  // namespace koobika::hook::network::protocol::http::v11::auth

#endif