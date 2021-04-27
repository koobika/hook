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

#ifndef koobika_hook_structured_json_jsonnumber_h
#define koobika_hook_structured_json_jsonnumber_h

#include <variant>

#include "base/serializable.h"
#include "json_number_type.h"

namespace koobika::hook::structured::json {
// =============================================================================
// JsonNumber                                                          ( class )
// -----------------------------------------------------------------------------
// This specification holds for JSON number default class.
// =============================================================================
class JsonNumber : public base::Serializable {
 public:
  // ---------------------------------------------------------------------------
  // Constructors/Destructors                                         ( public )
  // ---------------------------------------------------------------------------
  JsonNumber(const JsonNumber&) = default;
  JsonNumber(JsonNumber&&) noexcept = default;
  JsonNumber(const unsigned char& in) : value_{in} {}
  JsonNumber(const char& in) : value_{in} {}
  JsonNumber(const unsigned short& in) : value_{in} {}
  JsonNumber(const short& in) : value_{in} {}
  JsonNumber(const unsigned int& in) : value_{in} {}
  JsonNumber(const int& in) : value_{in} {}
  JsonNumber(const unsigned long& in) : value_{in} {}
  JsonNumber(const long& in) : value_{in} {}
  JsonNumber(const float& in) : value_{in} {}
  JsonNumber(const double& in) : value_{in} {}
  ~JsonNumber() = default;
  // ---------------------------------------------------------------------------
  // Operators                                                        ( public )
  // ---------------------------------------------------------------------------
  JsonNumber& operator=(const JsonNumber& in) {
    value_ = in.value_;
    return *this;
  }
  JsonNumber& operator=(JsonNumber&& in) noexcept {
    value_ = std::move(in.value_);
    return *this;
  }
  template <typename VAty>
  JsonNumber& operator=(const VAty& in) {
    value_ = in;
    return *this;
  }
  // ---------------------------------------------------------------------------
  // Methods                                                          ( public )
  // ---------------------------------------------------------------------------
  // Returns the subtype of the current number.
  JsonNumberType Type() const {
    switch (value_.index()) {
      case 0:
        return JsonNumberType::kUnsignedChar;
      case 1:
        return JsonNumberType::kChar;
      case 2:
        return JsonNumberType::kUnsignedShort;
      case 3:
        return JsonNumberType::kShort;
      case 4:
        return JsonNumberType::kUnsignedInt;
      case 5:
        return JsonNumberType::kInt;
      case 6:
        return JsonNumberType::kUnsignedLong;
      case 7:
        return JsonNumberType::kLong;
      case 8:
        return JsonNumberType::kFloat;
      case 9:
      default:
        return JsonNumberType::kDouble;
    }
  }
  // Sets the json-value with any of the supported numeric types.
  template <typename VAty>
  JsonNumber& Set(const VAty& in) {
    value_ = in;
    return *this;
  }
  // Gets the json-value using its numeric container.
  template <typename VAty>
  VAty Get() const {
    if (std::holds_alternative<VAty>(value_)) {
      return std::get<VAty>(value_);
    }
    switch (value_.index()) {
      case 0:
        return VAty(std::get<unsigned char>(value_));
      case 1:
        return VAty(std::get<char>(value_));
      case 2:
        return VAty(std::get<unsigned short>(value_));
      case 3:
        return VAty(std::get<short>(value_));
      case 4:
        return VAty(std::get<unsigned int>(value_));
      case 5:
        return VAty(std::get<int>(value_));
      case 6:
        return VAty(std::get<unsigned long>(value_));
      case 7:
        return VAty(std::get<long>(value_));
      case 8:
        return VAty(std::get<float>(value_));
      case 9:
      default:
        return VAty(std::get<double>(value_));
    }
  }
  // Dumps the current content to string.
  base::AutoBuffer Serialize() const override {
    switch (value_.index()) {
      case 0:
        return std::to_string(std::get<unsigned char>(value_));
      case 1:
        return std::to_string(std::get<char>(value_));
      case 2:
        return std::to_string(std::get<unsigned short>(value_));
      case 3:
        return std::to_string(std::get<short>(value_));
      case 4:
        return std::to_string(std::get<unsigned int>(value_));
      case 5:
        return std::to_string(std::get<int>(value_));
      case 6:
        return std::to_string(std::get<unsigned long>(value_));
      case 7:
        return std::to_string(std::get<long>(value_));
      case 8:
        return std::to_string(std::get<float>(value_));
      case 9:
        return std::to_string(std::get<double>(value_));
    }
    return {};
  }

 private:
  // ---------------------------------------------------------------------------
  // ATTRIBUTEs                                                      ( private )
  // ---------------------------------------------------------------------------
  std::variant<unsigned char, char, unsigned short, short, unsigned int, int,
               unsigned long, long, float, double>
      value_;
};
}  // namespace koobika::hook::structured::json

#endif