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

#ifndef koobika_hook_network_protocol_http_v11_httpmethod_h
#define koobika_hook_network_protocol_http_v11_httpmethod_h

#include <algorithm>
#include <string>
#include <unordered_map>

#include "http_constants.h"
#include "http_method_value.h"

namespace koobika::hook::network::protocol::http::v11 {
// =============================================================================
// HttpMethod                                                          [ class ]
// -----------------------------------------------------------------------------
// This class is in charge of providing the http method class
// =============================================================================
class HttpMethod {
 public:
  // ---------------------------------------------------------------------------
  // Constructors/Destructors                                         [ public ]
  // ---------------------------------------------------------------------------
  HttpMethod(const std::string& method) {
    static const std::unordered_map<std::string, int> known_methods = {
        {kOptStr_, HttpConstants::Methods::kOptions},
        {kGetStr_, HttpConstants::Methods::kGet},
        {kHeaStr_, HttpConstants::Methods::kHead},
        {kPosStr_, HttpConstants::Methods::kPost},
        {kPutStr_, HttpConstants::Methods::kPut},
        {kDelStr_, HttpConstants::Methods::kDelete},
        {kTraStr_, HttpConstants::Methods::kTrace},
        {kConStr_, HttpConstants::Methods::kConnect}};
    st_.assign(method);
    std::transform(st_.begin(), st_.end(), st_.begin(), ::tolower);
    auto itr = known_methods.find(st_);
    cd_ = itr != known_methods.end() ? itr->second
                                     : HttpConstants::Methods::kExtension;
  }
  HttpMethod(const HttpMethod&) = default;
  HttpMethod(HttpMethod&&) noexcept = default;
  ~HttpMethod() = default;
  // ---------------------------------------------------------------------------
  // Operators                                                        [ public ]
  // ---------------------------------------------------------------------------
  HttpMethod& operator=(const HttpMethod&) = default;
  HttpMethod& operator=(HttpMethod&&) noexcept = default;
  // ---------------------------------------------------------------------------
  // Methods                                                          [ public ]
  // ---------------------------------------------------------------------------
  // Returns the method code (numeric value)
  HttpMethodValue GetCode() const { return cd_; }
  // Returns the method description (string value)
  std::string GetDescription() const { return st_; }
  // Checks if current method is <OPTIONS>
  bool IsOptions() const { return cd_ == HttpConstants::Methods::kOptions; }
  // Checks if current method is <GET>
  bool IsGet() const { return cd_ == HttpConstants::Methods::kGet; }
  // Checks if current method is <HEAD>
  bool IsHead() const { return cd_ == HttpConstants::Methods::kHead; }
  // Checks if current method is <POST>
  bool IsPost() const { return cd_ == HttpConstants::Methods::kPost; }
  // Checks if current method is <PUT>
  bool IsPut() const { return cd_ == HttpConstants::Methods::kPut; }
  // Checks if current method is <DELETE>
  bool IsDelete() const { return cd_ == HttpConstants::Methods::kDelete; }
  // Checks if current method is <TRACE>
  bool IsTrace() const { return cd_ == HttpConstants::Methods::kTrace; }
  // Checks if current method is <CONNECT>
  bool IsConnect() const { return cd_ == HttpConstants::Methods::kConnect; }
  // Checks if current method is <EXTENSION>
  bool IsExtension() const { return cd_ == HttpConstants::Methods::kExtension; }

 private:
  // ---------------------------------------------------------------------------
  // Constants                                                       [ private ]
  // ---------------------------------------------------------------------------
  static constexpr char kOptStr_[] = "options";
  static constexpr char kGetStr_[] = "get";
  static constexpr char kHeaStr_[] = "head";
  static constexpr char kPosStr_[] = "post";
  static constexpr char kPutStr_[] = "put";
  static constexpr char kDelStr_[] = "delete";
  static constexpr char kTraStr_[] = "trace";
  static constexpr char kConStr_[] = "connect";
  // ---------------------------------------------------------------------------
  // Attributes                                                      [ private ]
  // ---------------------------------------------------------------------------
  HttpMethodValue cd_ = HttpConstants::Methods::kExtension;
  std::string st_;
};
}  // namespace koobika::hook::network::protocol::http::v11

#endif