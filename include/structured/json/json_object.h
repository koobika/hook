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

#ifndef koobika_hook_structured_json_jsonobject_h
#define koobika_hook_structured_json_jsonobject_h

#include <sstream>
#include <stdexcept>
#include <unordered_map>

#include "base/serializable.h"
#include "json_id.h"
#include "json_string.h"

namespace koobika::hook::structured::json {
// =============================================================================
// Types forwarding
// =============================================================================
struct ObjectDumper;
class JsonValue;
// =============================================================================
// JsonObjectGenericDumper                                             [ class ]
// -----------------------------------------------------------------------------
// This specification holds for JSON object default class
// =============================================================================
template <typename DUty = ObjectDumper>
class JsonObjectGenericDumper : public base::Serializable {
 public:
  // ---------------------------------------------------------------------------
  // Constructors/Destructors                                         [ public ]
  // ---------------------------------------------------------------------------
  JsonObjectGenericDumper() = default;
  JsonObjectGenericDumper(const JsonObjectGenericDumper&) = default;
  JsonObjectGenericDumper(JsonObjectGenericDumper&&) noexcept = default;
  JsonObjectGenericDumper(
      const std::initializer_list<std::pair<JsonString, JsonValue>>& in) {
    Assign_initializer_list_(in);
  }
  ~JsonObjectGenericDumper() = default;
  // ---------------------------------------------------------------------------
  // Operators                                                        [ public ]
  // ---------------------------------------------------------------------------
  JsonObjectGenericDumper& operator=(const JsonObjectGenericDumper& in) {
    map_ = in.map_;
    return *this;
  }
  JsonObjectGenericDumper& operator=(JsonObjectGenericDumper&& in) noexcept {
    map_ = std::move(in.map_);
    return *this;
  }
  JsonObjectGenericDumper& operator=(
      const std::initializer_list<std::pair<JsonString, JsonValue>>& in) {
    Assign_initializer_list_(in);
    return *this;
  }
  const JsonValue& operator[](const JsonId& id) const {
    auto const itr = map_.find(id);
    if (itr == map_.end()) {
      // [error] -> specified id does not exist into object!
      throw std::logic_error("specified id does not exist into object!");
    }
    return *itr->second;
  }
  JsonValue& operator[](const JsonId& id) {
    auto itr = map_.find(id);
    if (itr == map_.end()) {
      auto data = std::make_pair(id, std::make_shared<JsonValue>(JsonNull()));
      itr = map_.insert(std::move(data)).first;
    }
    return *itr->second;
  }
  // ---------------------------------------------------------------------------
  // Methods                                                          [ public ]
  // ---------------------------------------------------------------------------
  // Sets the specified object id with the incoming json-value
  JsonObjectGenericDumper& Set(const JsonId& id, const JsonValue& value) {
    map_[id] = std::make_shared<JsonValue>(value);
    return *this;
  }
  // Gets the json-value stored at the specified id
  const JsonValue& Get(const JsonId& id) const { return operator[](id); }
  // Dumps the current content to string
  base::Stream Serialize() const override {
    base::Stream stream;
    stream.Write('{');
    for (auto itr = map_.begin(); itr != map_.end(); itr++) {
      if (itr != map_.begin()) stream.Write(',');
      stream.Write('\"')
          .Write(itr->first)
          .Write("\":")
          .Write(DUty::Serialize(*itr->second));
    }
    stream.Write('}');
    return stream;
  }
  // Returns if the specified id exists into the current object
  bool Exist(const JsonId& id) { return map_.find(id) != map_.end(); }
  // Erase the json-value stored at the specified id
  void Erase(const JsonId& id) { map_.erase(id); }
  // Erase the json-values stored at the specified initializer_list
  void Erase(const std::initializer_list<JsonId>& ids) {
    for (auto const& id : ids) {
      Erase(id);
    }
  }

 private:
  // ---------------------------------------------------------------------------
  // Methods                                                         [ private ]
  // ---------------------------------------------------------------------------
  // Copies an initializer list of json-values to the internal data!
  void Assign_initializer_list_(
      const std::initializer_list<std::pair<JsonString, JsonValue>>& in) {
    for (auto const& element : in) {
      map_[element.first.Get()] = std::make_shared<JsonValue>(element.second);
    }
  }
  // ---------------------------------------------------------------------------
  // Attributes                                                      [ private ]
  // ---------------------------------------------------------------------------
  std::unordered_map<JsonId, std::shared_ptr<JsonValue>> map_;
};
using JsonObject = JsonObjectGenericDumper<ObjectDumper>;
}  // namespace koobika::hook::structured::json

#endif