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
// network/protocol/http/http_method.h
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

#ifndef koobika_hook_network_protocol_http_httpmethod_h
#define koobika_hook_network_protocol_http_httpmethod_h

#include <algorithm>
#include <string>
#include <unordered_map>

#include "constants/methods.h"

namespace koobika::hook::network::protocol::http {
// =============================================================================
// HttpMethod                                                          ( class )
// -----------------------------------------------------------------------------
// This class is in charge of providing the http method class
// =============================================================================
class HttpMethod {
 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  HttpMethod() = default;
  HttpMethod(const std::string& method) {
    static const std::unordered_map<std::string, int> known_methods = {
        {kOptStr_, constants::Methods::kOptions},
        {kGetStr_, constants::Methods::kGet},
        {kHeaStr_, constants::Methods::kHead},
        {kPosStr_, constants::Methods::kPost},
        {kPutStr_, constants::Methods::kPut},
        {kDelStr_, constants::Methods::kDelete},
        {kTraStr_, constants::Methods::kTrace},
        {kConStr_, constants::Methods::kConnect}};
    st_.assign(method);
    std::transform(st_.begin(), st_.end(), st_.begin(), ::tolower);
    auto itr = known_methods.find(st_);
    cd_ = itr != known_methods.end() ? itr->second
                                     : constants::Methods::kExtension;
  }
  HttpMethod(const HttpMethod&) = default;
  HttpMethod(HttpMethod&&) noexcept = default;
  ~HttpMethod() = default;
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  //
  HttpMethod& operator=(const HttpMethod&) = default;
  HttpMethod& operator=(HttpMethod&&) noexcept = default;
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  // Returns the method code (numeric value).
  HttpMethodValue GetCode() const { return cd_; }
  // Returns the method description (string value).
  std::string GetDescription() const { return st_; }
  // Checks if current method is 'OPTIONS'.
  bool IsOptions() const { return cd_ == constants::Methods::kOptions; }
  // Checks if current method is 'GET'.
  bool IsGet() const { return cd_ == constants::Methods::kGet; }
  // Checks if current method is 'HEAD'.
  bool IsHead() const { return cd_ == constants::Methods::kHead; }
  // Checks if current method is 'POST'.
  bool IsPost() const { return cd_ == constants::Methods::kPost; }
  // Checks if current method is 'PUT>'.
  bool IsPut() const { return cd_ == constants::Methods::kPut; }
  // Checks if current method is 'DELETE'.
  bool IsDelete() const { return cd_ == constants::Methods::kDelete; }
  // Checks if current method is 'TRACE'.
  bool IsTrace() const { return cd_ == constants::Methods::kTrace; }
  // Checks if current method is 'CONNECT'.
  bool IsConnect() const { return cd_ == constants::Methods::kConnect; }
  // Checks if current method is 'EXTENSION'.
  bool IsExtension() const { return cd_ == constants::Methods::kExtension; }

 private:
  // ___________________________________________________________________________
  // CONSTANTs                                                       ( private )
  //
  static constexpr char kOptStr_[] = "options";
  static constexpr char kGetStr_[] = "get";
  static constexpr char kHeaStr_[] = "head";
  static constexpr char kPosStr_[] = "post";
  static constexpr char kPutStr_[] = "put";
  static constexpr char kDelStr_[] = "delete";
  static constexpr char kTraStr_[] = "trace";
  static constexpr char kConStr_[] = "connect";
  // ___________________________________________________________________________
  // ATTRIBUTEs                                                      ( private )
  //
  HttpMethodValue cd_ = constants::Methods::kExtension;
  std::string st_;
};
}  // namespace koobika::hook::network::protocol::http

#endif