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

#ifndef koobika_hook_network_protocol_http_headers_h
#define koobika_hook_network_protocol_http_headers_h

#include <string>
#include <unordered_map>

#include "constants/strings.h"

namespace koobika::hook::network::protocol::http {
// =============================================================================
// Headers                                                             ( class )
// -----------------------------------------------------------------------------
// This class is in charge of providing the http headers class.
// =============================================================================
class Headers {
 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  Headers() : src_buffer_{nullptr}, src_buffer_len_{0} {}
  Headers(const char* buffer, const std::size_t& len) { From(buffer, len); }
  Headers(const Headers&) = default;
  Headers(Headers&&) noexcept = default;
  ~Headers() = default;
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  //
  Headers& operator=(const Headers&) = default;
  Headers& operator=(Headers&&) noexcept = default;
  const std::string& operator[](const std::string& key) { return Get(key); }
  const std::string& operator[](const std::string& key) const {
    return Get(key);
  }
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  // Cleans up all stored information.
  void Clear() {
    map_.clear();
    src_buffer_ = nullptr;
    src_buffer_len_ = 0;
  }
  // Sets headers content from specified string.
  void From(const char* str, const std::size_t& len) {
    src_buffer_ = str;
    src_buffer_len_ = len;
  }
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
    setupFromSrcBuffer();
    map_[name] = value;
  }
  // Adds a header entry (templated version).
  template <typename DAty>
  void Add(const char* name, const DAty& value) {
    Add(name, std::to_string(value).c_str());
  }
  // Adds a header entry.
  void Add(const char* name, const std::string& value) {
    Add(name, value.c_str());
  }
  // Adds a header entry.
  void Add(const char* name, const char* value) {
    setupFromSrcBuffer();
    auto itr = map_.find(name);
    if (itr != map_.end()) {
      itr->second.append(value);
      return;
    }
    Set(name, value);
  }
  // Returns header entry using its name (const reference&).
  std::string& Get(const std::string& name) { return Get(name.c_str()); }
  // Returns header entry using its name (const reference&).
  std::string& Get(const char* name) {
    setupFromSrcBuffer();
    auto itr = map_.find(name);
    if (itr == map_.end()) {
      // ((Error)) -> no field matching specified name!
      throw std::logic_error("There is no field matching specified name!");
    }
    return map_.find(name)->second;
  }
  // Returns header entry using its name (const reference&).
  const std::string& Get(const std::string& name) const {
    return Get(name.c_str());
  }
  // Returns header entry using its name (const reference&).
  const std::string& Get(const char* name) const {
    setupFromSrcBuffer();
    auto const itr = map_.find(name);
    if (itr == map_.end()) {
      // ((Error)) -> no field matching specified name!
      throw std::logic_error("There is no field matching specified name!");
    }
    return map_.find(name)->second;
  }
  // Checks for the specified header entry (using its name).
  bool Exist(const std::string& name) const {
    setupFromSrcBuffer();
    return map_.find(name) != map_.end();
  }
  // Dumps internal content to the specified string.
  void DumpTo(base::AutoBuffer& out) const {
    if (src_buffer_) {
      out.Write(src_buffer_, src_buffer_len_);
      return;
    }
    for (auto const& field : map_) {
      out.Write(field.first)
          .Write(constants::Strings::kColon)
          .Write(constants::Strings::kSpace)
          .Write(field.second)
          .Write(constants::Strings::kCrLf);
    }
    out.Write(constants::Strings::kCrLf);
  }

 private:
  // ___________________________________________________________________________
  // METHODs                                                         ( private )
  //
  // Setups internal structure using the associated src-buffer.
  inline void setupFromSrcBuffer() const {
    if (src_buffer_) {
      /*
      pepe -> implementar "parseo" del string a mapa!
      */

      /*
      pepe fin
      */

      src_buffer_ = nullptr;
      src_buffer_len_ = 0;
    }
  }
  // ___________________________________________________________________________
  // ATTRIBUTEs                                                      ( private )
  //
  std::unordered_map<std::string, std::string> map_;
  mutable const char* src_buffer_;
  mutable std::size_t src_buffer_len_;
};
}  // namespace koobika::hook::network::protocol::http

#endif
