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
#include <utility>
#include <unordered_map>

#include "base/associative_container.h"

namespace koobika::hook::network::protocol::http {
// =============================================================================
// HttpParameters                                                      ( class )
// -----------------------------------------------------------------------------
// This specification holds for http routing parameters.
// =============================================================================
class HttpParameters
    : public base::AssociativeContainer<std::string, std::string> {
  // ___________________________________________________________________________
  // USINGs                                                          ( private )
  //
  using Base = base::AssociativeContainer<std::string, std::string>;

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
  virtual const Value& operator[](Key&& key) const override { return Get(key); }
  virtual const Value& operator[](const Key& key) const override {
    return Get(key);
  }
  virtual Value& operator[](Key&& key) override { return Get(key); }
  virtual Value& operator[](const Key& key) override { return Get(key); }
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  // Adds the specified tuple to the object.
  virtual void Add(const std::pair<Key, Value>& data) override {
    data_.insert(data);
  }
  virtual void Add(std::pair<Key, Value>&& data) override {
    data_.insert(data);
  }
  // Erases the specified value (located by key) from the object.
  virtual void Erase(const Key& key) override { data_.erase(key); }
  virtual void Erase(Key&& key) override { data_.erase(key); }
  // Checks for an existent key.
  virtual bool Exist(const Key& key) const override {
    return data_.find(key) != data_.end();
  }
  virtual bool Exist(Key&& key) const override {
    return data_.find(key) != data_.end();
  }
  // Returns the associated value for an existent key.
  virtual const Value& Get(const Key& key) const override {
    auto itr = data_.find(key);
    if (itr == data_.end()) {
      // ((Error)) -> There is no entry with the specified key!
    }
    return itr->second;
  }
  virtual const Value& Get(Key&& key) const override {
    auto itr = data_.find(key);
    if (itr == data_.end()) {
      // ((Error)) -> There is no entry with the specified key!
    }
    return itr->second;
  }
  // Returns the associated value for an existent key.
  virtual Value& Get(const Key& key) override {
    auto itr = data_.find(key);
    if (itr == data_.end()) {
      itr = data_.insert(std::make_pair(key, Value())).first;
    }
    return itr->second;
  }
  virtual Value& Get(Key&& key) override {
    auto itr = data_.find(key);
    if (itr == data_.end()) {
      itr = data_.insert(std::make_pair(key, Value())).first;
    }
    return itr->second;
  }

 private:
  // ___________________________________________________________________________
  // ATTRIBUTEs                                                      ( private )
  //
  std::unordered_map<Key, Value> data_;
};
}  // namespace koobika::hook::network::protocol::http

#endif