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
// network/protocol/http/method_helper.h
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

#ifndef koobika_hook_network_protocol_http_methodhelper_h
#define koobika_hook_network_protocol_http_methodhelper_h

#include <algorithm>
#include <string>
#include <unordered_map>

#include "base/case_insensitive_comparator.h"
#include "base/case_insensitive_hash.h"
#include "constants/methods.h"

namespace koobika::hook::network::protocol::http {
//! @brief Http method (helper) class
class MethodHelper {
 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  MethodHelper() = default;
  MethodHelper(const MethodHelper&) = default;
  MethodHelper(MethodHelper&&) noexcept = default;
  ~MethodHelper() = default;
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  //
  MethodHelper& operator=(const MethodHelper&) = default;
  MethodHelper& operator=(MethodHelper&&) noexcept = default;
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  //! @brief Sets method content from the specified memory buffer
  //! @param[in] str C-string memory buffer
  //! @param[in] len Memory buffer length
  void From(const char* str, const std::size_t& len) {
    static const std::unordered_map<std::string, int, base::CaseInsensitiveHash,
                                    base::CaseInsensitiveComparator>
        known_methods = {{kOptStr_, constants::Methods::kOptions},
                         {kGetStr_, constants::Methods::kGet},
                         {kHeaStr_, constants::Methods::kHead},
                         {kPosStr_, constants::Methods::kPost},
                         {kPutStr_, constants::Methods::kPut},
                         {kDelStr_, constants::Methods::kDelete},
                         {kTraStr_, constants::Methods::kTrace},
                         {kConStr_, constants::Methods::kConnect}};
    auto itr = known_methods.find(std::string(str, len));
    cd_ = itr != known_methods.end() ? itr->second
                                     : constants::Methods::kExtension;
  }
  //! @brief Gets the method numerical code
  //! @returns MethodValue Method numerical code
  //! @remarks <em>MethodValue</em> is just an alias for <em>int</em>
  MethodValue GetCode() const { return cd_; }
  //! @brief Gets the method string-based description
  //! @returns std::string Method string-based description
  std::string GetDescription() const { return st_; }
  //! @brief Checks if current method is an OPTIONS one
  //! @returns bool <em>true</em> if OPTIONS <em>false</em> otherwise
  bool IsOptions() const { return cd_ == constants::Methods::kOptions; }
  //! @brief Checks if current method is a GET one
  //! @returns bool <em>true</em> if GET <em>false</em> otherwise
  bool IsGet() const { return cd_ == constants::Methods::kGet; }
  //! @brief Checks if current method is a HEAD one
  //! @returns bool <em>true</em> if HEAD <em>false</em> otherwise
  bool IsHead() const { return cd_ == constants::Methods::kHead; }
  //! @brief Checks if current method is a POST one
  //! @returns bool <em>true</em> if POST <em>false</em> otherwise
  bool IsPost() const { return cd_ == constants::Methods::kPost; }
  //! @brief Checks if current method is a PUT one
  //! @returns bool <em>true</em> if PUT <em>false</em> otherwise
  bool IsPut() const { return cd_ == constants::Methods::kPut; }
  //! @brief Checks if current method is a DELETE one
  //! @returns bool <em>true</em> if DELETE <em>false</em> otherwise
  bool IsDelete() const { return cd_ == constants::Methods::kDelete; }
  //! @brief Checks if current method is a TRACE one
  //! @returns bool <em>true</em> if TRACE <em>false</em> otherwise
  bool IsTrace() const { return cd_ == constants::Methods::kTrace; }
  //! @brief Checks if current method is a CONNECT one
  //! @returns bool <em>true</em> if CONNECT <em>false</em> otherwise
  bool IsConnect() const { return cd_ == constants::Methods::kConnect; }
  //! @brief Checks if current method is a EXTENSION one
  //! @returns bool <em>true</em> if EXTENSION <em>false</em> otherwise
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
  MethodValue cd_ = constants::Methods::kExtension;
  std::string st_;
};
}  // namespace koobika::hook::network::protocol::http

#endif