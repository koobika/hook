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
// network/protocol/http/http_headers.h
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

#ifndef koobika_hook_network_protocol_http_httpheaders_h
#define koobika_hook_network_protocol_http_httpheaders_h

#include <string>
#include <unordered_map>

#include "base/serializable.h"
#include "base/variant.h"
#include "constants/strings.h"
#include "http_util.h"

namespace koobika::hook::network::protocol::http {
// =============================================================================
// HttpHeaders                                                         ( class )
// -----------------------------------------------------------------------------
// This class is in charge of providing the http headers class.
// =============================================================================
class HttpHeaders {
  // ___________________________________________________________________________
  // USINGs                                                          ( private )
  //
  using VariantList = std::initializer_list<base::Variant>;

 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  HttpHeaders() = default;
  HttpHeaders(const std::string& raw_content) {}
  HttpHeaders(const char* raw_content) {}
  HttpHeaders(const HttpHeaders&) = default;
  HttpHeaders(HttpHeaders&&) noexcept = default;
  ~HttpHeaders() = default;
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  //
  HttpHeaders& operator=(const HttpHeaders&) = default;
  HttpHeaders& operator=(HttpHeaders&&) noexcept = default;
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  // Cleans up all stored information.
  void Clear() { map_.clear(); }
  // Sets a header entry (templated version).
  template <typename DAty>
  void Set(const char* name, const DAty& value) {
    Set(name, std::to_string(value).c_str());
  }
  // Sets a header entry.
  void Set(const char* name, const std::string& value) {
    Set(name, value.c_str());
  }
  // Sets a header entry.
  void Set(const char* name, const char* value) {
    if (!HttpUtil::IsToken(name)) {
      // ((Error)) -> specified name is NOT a token!
      // ((To-Do)) -> raise an exception?
      return;
    }
    if (!HttpUtil::IsTEXT(value)) {
      // ((Error)) -> specified value is NOT text!
      // ((To-Do)) -> raise an exception?
      return;
    }
    map_[name] = value;
  }
  // Sets a header entry (variant-list version).
  void Set(const std::string& name, const VariantList& values) {
    if (!HttpUtil::IsToken(name)) {
      // ((Error)) -> specified name is NOT a token!
      // ((To-Do)) -> raise an exception?
      return;
    }
    auto& field = map_[name];
    for (auto const& value : values) {
      auto const& element_value = value.GetAsString();
      if (!element_value.has_value()) continue;
      if (!HttpUtil::IsTEXT(element_value.value())) {
        // ((Error)) -> specified value is NOT text!
        // ((To-Do)) -> raise an exception?
        continue;
      }
      field = element_value.value().c_str();
    }
  }
  // Sets a header entry (initializer-list version).
  void Set(const std::pair<std::string, VariantList>& elements) {
    Set(elements.first, elements.second);
  }
  // Returns header entry using its name (const reference&).
  std::optional<std::string> Get(const std::string& name) const {
    auto itr = map_.find(name);
    return itr != map_.end() ? itr->second : std::optional<std::string>();
  }
  // Checks for the specified header entry (using its name).
  bool Exist(const std::string& name) const {
    return map_.find(name) != map_.end();
  }
  // Dumps the associated internal content to the output string.
  std::string Dump() const {
    std::string out;
    for (auto const& field : map_) {
      out.append(field.first)
          .append(constants::Strings::kColon)
          .append(constants::Strings::kSpace)
          .append(field.second)
          .append(constants::Strings::kCrLf);
    }
    return out;
  }

 private:
  // ___________________________________________________________________________
  // ATTRIBUTEs                                                      ( private )
  //
  std::unordered_map<std::string, std::string> map_;
};
}  // namespace koobika::hook::network::protocol::http

#endif