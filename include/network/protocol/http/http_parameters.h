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
// network/protocol/http/http_parameters.h
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

#ifndef koobika_hook_network_protocol_http_httpparameters_h
#define koobika_hook_network_protocol_http_httpparameters_h

#include <string>
#include <unordered_map>

namespace koobika::hook::network::protocol::http {
// =============================================================================
// HttpParameters                                                      ( class )
// -----------------------------------------------------------------------------
// This specification holds for http routing parameters.
// =============================================================================
class HttpParameters {
 private:
  // ___________________________________________________________________________
  // USINGs                                                           ( public )
  //
  using Value = std::string;

 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  HttpParameters() = default;
  HttpParameters(const HttpParameters&) = default;
  HttpParameters(HttpParameters&&) noexcept = default;
  ~HttpParameters() = default;
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  //
  HttpParameters& operator=(const HttpParameters&) = default;
  HttpParameters& operator=(HttpParameters&&) noexcept = default;
  const Value& operator[](const char* key) const { return Get(key); }
  Value& operator[](const char* key) { return Get(key); }
  const Value& operator[](const Value& key) const { return Get(key); }
  Value& operator[](const std::string& key) { return Get(key); }
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  // Adds the specified tuple to the object.
  HttpParameters& Add(const std::pair<std::string, Value>& in) {
    Get(in.first) = in.second;
    return *this;
  }
  // Erases the specified value (located by key) from the object.
  HttpParameters& Erase(const std::string& key) {
    auto itr = data_.find(key);
    if (itr == data_.end()) {
      // ((Error)) -> while querying for a non-existent key!
      throw std::logic_error("Out of bounds!");
    }
    data_.erase(key);
    return *this;
  }
  // Checks for an existent key.
  bool Exist(const std::string& key) const {
    return data_.find(key) != data_.end();
  }
  // Returns the associated value for an existent key.
  const Value& Get(const std::string& key) const {
    auto itr = data_.find(key);
    if (itr == data_.end()) {
      // ((Error)) -> while querying for a non-existent key!
      throw std::logic_error("Key not found!");
    }
    return itr->second;
  }
  // Returns the associated value for an existent key.
  Value& Get(const std::string& key) { return data_[key]; }
  // Returns the associated value for an existent key.
  const Value& At(const std::string& key) const {
    auto const itr = data_.find(key);
    if (itr == data_.end()) {
      // ((Error)) -> while querying for a non-existent key!
      throw std::logic_error("Not found!");
    }
    return itr->second;
  }
  // Returns the associated value for an existent key.
  Value& At(const std::string& key) {
    auto itr = data_.find(key);
    if (itr == data_.end()) {
      // ((Error)) -> while querying for a non-existent key!
      throw std::logic_error("Not found!");
    }
    return itr->second;
  }

 private:
  // ___________________________________________________________________________
  // ATTRIBUTEs                                                      ( private )
  //
  std::unordered_map<std::string, Value> data_;
};
}  // namespace koobika::hook::network::protocol::http

#endif