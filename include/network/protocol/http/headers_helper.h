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
// network/protocol/http/headers.h
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

#ifndef koobika_hook_network_protocol_http_headershelper_h
#define koobika_hook_network_protocol_http_headershelper_h

#include <string>
#include <unordered_map>
#include <algorithm>

#include "base/auto_buffer.h"
#include "base/case_insensitive_comparator.h"
#include "base/case_insensitive_hash.h"
#include "constants/characters.h"
#include "constants/strings.h"

namespace koobika::hook::network::protocol::http {
//! @brief Http headers (helper) class
class HeadersHelper {
 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  HeadersHelper() = default;
  HeadersHelper(const HeadersHelper&) = default;
  HeadersHelper(HeadersHelper&&) noexcept = default;
  ~HeadersHelper() = default;
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  //
  HeadersHelper& operator=(const HeadersHelper&) = default;
  HeadersHelper& operator=(HeadersHelper&&) noexcept = default;
  //! @brief Gets the (modifiable) associated field value identified by the
  //! provided key. Header field keys are <em>case-insensitive</em>.
  //! @param[in] key Header field key
  std::string& operator[](const std::string& key) { return Get(key); }
  //! @brief Gets the (non-modifiable) associated field value identified by the
  //! provided key. Header field keys are <em>case-insensitive</em>.
  //! @param[in] key Header field key
  const std::string& operator[](const std::string& key) const {
    return Get(key);
  }
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  //! @brief Cleans up all currently stored information
  void Clear() { map_.clear(); }
  //! @brief Sets headers content from the specified memory buffer
  //! @param[in] str C-string memory buffer
  //! @param[in] len Memory buffer length
  void From(const char* str, const std::size_t& len) {
    auto ptr = str;
    auto end = ptr + len;
    auto last = ptr;
    bool field_name_ok = false;
    const char* colon_delimiter = nullptr;
    while (ptr != end) {
      if (field_name_ok) {
        if ((ptr + 1) != end && *ptr == constants::Characters::kCr &&
            *(ptr + 1) == constants::Characters::kLf) {
          if ((ptr + 2) == end ||
              (*(ptr + 2) != constants::Characters::kSpace &&
               *(ptr + 2) != constants::Characters::kHt)) {
            while (*colon_delimiter == constants::Characters::kColon ||
                   *colon_delimiter == constants::Characters::kSpace ||
                   *colon_delimiter == constants::Characters::kHt)
              colon_delimiter++;
            Set(std::string(last, colon_delimiter - last - 2),
                std::string(colon_delimiter, ptr - colon_delimiter));
            ptr += 2;
            last = ptr;
            field_name_ok = false;
            continue;
          }
        }
      } else {
        if ((field_name_ok = *ptr == constants::Characters::kColon)) {
          colon_delimiter = ptr;
        }
      }
      ptr++;
    }
  }
  //! @brief Sets a header field content
  //! @tparam DAty Datatype to be set (it must be convertible to std::string)
  //! @param[in] name Header field name
  //! @param[in] value Header field value
  template <typename DAty>
  void Set(const char* name, const DAty& value) {
    Set(name, std::to_string(value).c_str());
  }
  //! @brief Sets a header field content
  //! @param[in] name Header field name
  //! @param[in] value Header field value
  void Set(const char* name, const std::string& value) {
    Set(name, value.c_str());
  }
  //! @brief Sets a header field content
  //! @param[in] name Header field name
  //! @param[in] value Header field value
  void Set(const char* name, const char* value) { map_[name] = value; }
  //! @brief Sets a header field content
  //! @param[in] name Header field name
  //! @param[in] value Header field value
  void Set(const std::string& name, const std::string& value) {
    Set(name.c_str(), value.c_str());
  }
  //! @brief Adds content to a header field
  //! @tparam DAty Datatype to be added (it must be convertible to std::string)
  //! @param[in] name Header field name
  //! @param[in] value Header field value
  template <typename DAty>
  void Add(const char* name, const DAty& value) {
    Add(name, std::to_string(value).c_str());
  }
  //! @brief Adds content to a header field
  //! @param[in] name Header field name
  //! @param[in] value Header field value
  void Add(const char* name, const std::string& value) {
    Add(name, value.c_str());
  }
  //! @brief Adds content to a header field
  //! @param[in] name Header field name
  //! @param[in] value Header field value
  void Add(const char* name, const char* value) {
    auto itr = map_.find(name);
    if (itr != map_.end()) {
      itr->second.append(value);
      return;
    }
    Set(name, value);
  }
  //! @brief Gets the (modifiable) associated field value identified by the
  //! provided name. Header field keys are <em>case-insensitive</em>.
  //! @param[in] name Header field name
  std::string& Get(const std::string& name) { return Get(name.c_str()); }
  //! @brief Gets the (modifiable) associated field value identified by the
  //! provided name. Header field keys are <em>case-insensitive</em>.
  //! @param[in] name Header field name
  std::string& Get(const char* name) {
    auto itr = map_.find(name);
    if (itr == map_.end()) {
      // ((Error)) -> no field matching the specified key!
      throw std::logic_error("There is no field matching tñhe specified key!");
    }
    return map_.find(name)->second;
  }
  //! @brief Gets the (non-modifiable) associated field value identified by the
  //! provided name. Header field keys are <em>case-insensitive</em>.
  //! @param[in] name Header field name
  const std::string& Get(const std::string& name) const {
    return Get(name.c_str());
  }
  //! @brief Gets the (non-modifiable) associated field value identified by the
  //! provided name. Header field keys are <em>case-insensitive</em>.
  //! @param[in] name Header field name
  const std::string& Get(const char* name) const {
    auto const itr = map_.find(name);
    if (itr == map_.end()) {
      // ((Error)) -> no field matching the specified key!
      throw std::logic_error("There is no field matching the specified key!");
    }
    return map_.find(name)->second;
  }
  //! @brief Checks if the specified field name exists
  //! @param[in] name Header field name
  //! @returns bool <em>true</em> if exists <em>false</em> otherwise
  bool Exist(const std::string& name) const {
    return map_.find(name) != map_.end();
  }
  //! @brief Dumps internal content to the specified string
  //! @param[out] out Autobuffer containing string-based serialized content.
  void DumpTo(base::AutoBuffer& out) const {
    for (auto const& field : map_) {
      out.Write(field.first)
          .Write(constants::Strings::kHeaderFieldSeparator)
          .Write(field.second)
          .Write(constants::Strings::kCrLf);
    }
    out.Write(constants::Strings::kCrLf);
  }
  // Just to allow range-based iteration.
  auto begin() { return map_.begin(); }
  // Just to allow range-based iteration.
  const auto begin() const { return map_.begin(); }
  // Just to allow range-based iteration.
  auto end() { return map_.end(); }
  // Just to allow range-based iteration.
  const auto end() const { return map_.end(); }

 private:
  // ___________________________________________________________________________
  // ATTRIBUTEs                                                      ( private )
  //
  std::unordered_map<std::string, std::string, base::CaseInsensitiveHash,
                     base::CaseInsensitiveComparator>
      map_;
};
}  // namespace koobika::hook::network::protocol::http

#endif
