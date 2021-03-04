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

#ifndef koobika_hook_structured_json_jsonarray_h
#define koobika_hook_structured_json_jsonarray_h

#include <sstream>
#include <stdexcept>
#include <vector>

#include "base/serializable.h"

namespace koobika::hook::structured::json {
// =============================================================================
// Types forwarding
// =============================================================================
struct ArrayDumper;
class JsonValue;
// =============================================================================
// JsonArrayGenericDumper                                              ( class )
// -----------------------------------------------------------------------------
// This specification holds for JSON array default class.
// =============================================================================
template <typename DUty = ArrayDumper>
class JsonArrayGenericDumper : public base::Serializable {
 public:
  // ---------------------------------------------------------------------------
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  // ---------------------------------------------------------------------------
  JsonArrayGenericDumper() = default;
  JsonArrayGenericDumper(const JsonArrayGenericDumper&) = default;
  JsonArrayGenericDumper(JsonArrayGenericDumper&&) noexcept = default;
  JsonArrayGenericDumper(const std::initializer_list<JsonValue>& in) {
    Assign_initializer_list_(in);
  }
  ~JsonArrayGenericDumper() = default;
  // ---------------------------------------------------------------------------
  // OPERATORs                                                        ( public )
  // ---------------------------------------------------------------------------
  JsonArrayGenericDumper& operator=(const JsonArrayGenericDumper&) = default;
  JsonArrayGenericDumper& operator=(JsonArrayGenericDumper&& in) noexcept =
      default;
  JsonArrayGenericDumper& operator=(
      const std::initializer_list<JsonValue>& in) {
    Assign_initializer_list_(in);
    return *this;
  }
  const JsonValue& operator[](const std::size_t& index) const {
    if (index >= vector_.size()) {
      // [error] -> out of bounds index!
      throw std::logic_error("out of bounds index!");
    }
    return *vector_[index];
  }
  JsonValue& operator[](const std::size_t& index) {
    if (index >= vector_.size()) {
      // [error] -> out of bounds index!
      throw std::logic_error("out of bounds index!");
    }
    return *vector_[index];
  }
  // ---------------------------------------------------------------------------
  // METHODs                                                          ( public )
  // ---------------------------------------------------------------------------
  // Adds a json-value to the current structure.
  JsonArrayGenericDumper& Add(const JsonValue& value) {
    vector_.push_back(std::make_shared<JsonValue>(value));
    return *this;
  }
  // Moves a json-value to the current structure.
  JsonArrayGenericDumper& Add(JsonValue&& value) noexcept {
    vector_.push_back(std::make_shared<JsonValue>(std::move(value)));
    return *this;
  }
  // Sets the specified array position with the incoming json-value.
  JsonArrayGenericDumper& Set(const std::size_t& index,
                              const JsonValue& value) {
    if (index >= vector_.size()) {
      // [error] -> out of bounds index!
      throw std::logic_error("out of bounds index!");
    }
    vector_[index] = std::make_shared<JsonValue>(value);
    return *this;
  }
  // Gets the json-value stored at the specified position.
  const JsonValue& Get(const std::size_t& index) const {
    return operator[](index);
  }
  // Dumps the current content to string.
  base::Stream Serialize() const override {
    base::Stream stream;
    stream.Write('[');
    for (auto itr = vector_.begin(); itr != vector_.end(); itr++) {
      if (itr != vector_.begin()) stream.Write(',');
      stream.Write(DUty::Serialize(*(*itr)));
    }
    stream.Write(']');
    return stream;
  }
  // Erase the json-value stored at the specified position.
  void Erase(const std::size_t& index) {
    if (index >= vector_.size()) {
      // [error] -> out of bounds index!
      throw std::logic_error("out of bounds index!");
    }
    vector_.erase(vector_.begin() + index);
  }
  // Erase the json-values stored within the specified range.
  void Erase(const std::size_t& from, const std::size_t& to) {
    if (from >= vector_.size() || (to + 1) >= vector_.size()) {
      // [error] -> out of bounds index!
      throw std::logic_error("out of bounds index!");
    }
    vector_.erase(vector_.begin() + from, vector_.begin() + (to + 1));
  }

 private:
  // ---------------------------------------------------------------------------
  // METHODs                                                         ( private )
  // ---------------------------------------------------------------------------
  // Copies an initializer list of json-values to the internal data.
  void Assign_initializer_list_(const std::initializer_list<JsonValue>& in) {
    for (auto const& element : in) {
      vector_.push_back(std::make_shared<JsonValue>(element));
    }
  }
  // ---------------------------------------------------------------------------
  // ATTRIBUTEs                                                      ( private )
  // ---------------------------------------------------------------------------
  std::vector<std::shared_ptr<JsonValue>> vector_;
};
using JsonArray = JsonArrayGenericDumper<ArrayDumper>;
}  // namespace koobika::hook::structured::json

#endif