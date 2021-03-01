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

#ifndef koobika_hook_structured_json_jsonvalue_h
#define koobika_hook_structured_json_jsonvalue_h

#include <limits>
#include <optional>
#include <string>
#include <variant>

#include "base/serializable.h"
#include "json_array.h"
#include "json_false.h"
#include "json_null.h"
#include "json_number.h"
#include "json_object.h"
#include "json_string.h"
#include "json_true.h"
#include "json_value_type.h"

namespace koobika::hook::structured::json {

// =============================================================================
// DefaultNumericParser                                               [ struct ]
// -----------------------------------------------------------------------------
// This specification holds for JSON default numeric parser module
// =============================================================================
struct DefaultNumericParser {
  // ---------------------------------------------------------------------------
  // Methods                                                          [ public ]
  // ---------------------------------------------------------------------------
  // Performs a parsing of the incoming string to return a json-number
  JsonNumber Parse(const std::string& str) const {
    double number = atof(str.data());
    if (str.find(kDotCh) != std::string::npos) {
      return JsonNumber(number);
    } else {
      if (number >= kSCMin && number <= kSCMax) {
        return JsonNumber(char(number));
      } else if (number >= kUCMin && number <= kUCMax) {
        return JsonNumber(unsigned char(number));
      } else if (number >= kSSMin && number <= kSSMax) {
        return JsonNumber(short(number));
      } else if (number >= kUSMin && number <= kUSMax) {
        return JsonNumber(unsigned short(number));
      } else if (number >= kSIMin && number <= kSIMax) {
        return JsonNumber(int(number));
      } else if (number >= kUIMin && number <= kUIMax) {
        return JsonNumber(unsigned int(number));
      } else if (number >= kSLMin && number <= kSLMax) {
        return JsonNumber(long(number));
      } else if (number >= kULMin && number <= kULMax) {
        return JsonNumber(unsigned long(number));
      } else {
        return JsonNumber(number);
      }
    }
  }

 private:
  // ---------------------------------------------------------------------------
  // Constants                                                       [ private ]
  // ---------------------------------------------------------------------------
  // structural-characters
  static constexpr char kDotCh = 0x2E;
  // thresholds
  static constexpr double kUCMin = std::numeric_limits<unsigned char>::min();
  static constexpr double kUCMax = std::numeric_limits<unsigned char>::max();
  static constexpr double kSCMin = std::numeric_limits<char>::min();
  static constexpr double kSCMax = std::numeric_limits<char>::max();
  static constexpr double kUSMin = std::numeric_limits<unsigned short>::min();
  static constexpr double kUSMax = std::numeric_limits<unsigned short>::max();
  static constexpr double kSSMin = std::numeric_limits<short>::min();
  static constexpr double kSSMax = std::numeric_limits<short>::max();
  static constexpr double kUIMin = std::numeric_limits<unsigned int>::min();
  static constexpr double kUIMax = std::numeric_limits<unsigned int>::max();
  static constexpr double kSIMin = std::numeric_limits<int>::min();
  static constexpr double kSIMax = std::numeric_limits<int>::max();
  static constexpr double kULMin = std::numeric_limits<unsigned long>::min();
  static constexpr double kULMax = std::numeric_limits<unsigned long>::max();
  static constexpr double kSLMin = std::numeric_limits<long>::min();
  static constexpr double kSLMax = std::numeric_limits<long>::max();
};

// =============================================================================
// JsonValue                                                           [ class ]
// -----------------------------------------------------------------------------
// This specification holds for JSON value default class
// =============================================================================
class JsonValue : public base::Serializable {
 private:
  // ---------------------------------------------------------------------------
  // Types                                                           [ private ]
  // ---------------------------------------------------------------------------
  using InternalType = std::variant<JsonNull, JsonTrue, JsonFalse, JsonNumber,
                                    JsonString, JsonObject, JsonArray>;

 public:
  // ---------------------------------------------------------------------------
  // Constructors/Destructors                                         [ public ]
  // ---------------------------------------------------------------------------
  JsonValue(const JsonValue&) = default;
  JsonValue(JsonValue&&) noexcept = default;
  JsonValue(const JsonNull& in) : data_{in} {}
  JsonValue(const JsonTrue& in) : data_{in} {}
  JsonValue(const JsonFalse& in) : data_{in} {}
  JsonValue(const JsonNumber& in) : data_{in} {}
  JsonValue(const JsonString& in) : data_{in} {}
  JsonValue(const JsonObject& in) : data_{in} {}
  JsonValue(const JsonArray& in) : data_{in} {}
  JsonValue(JsonNull&& in) noexcept : data_{std::move(in)} {}
  JsonValue(JsonTrue&& in) noexcept : data_{std::move(in)} {}
  JsonValue(JsonFalse&& in) noexcept : data_{std::move(in)} {}
  JsonValue(JsonNumber&& in) noexcept : data_{std::move(in)} {}
  JsonValue(JsonString&& in) noexcept : data_{std::move(in)} {}
  JsonValue(JsonObject&& in) noexcept : data_{std::move(in)} {}
  JsonValue(JsonArray&& in) noexcept : data_{std::move(in)} {}
  JsonValue(const nullptr_t& in) { Assign_null_(in); }
  JsonValue(const bool& in) { Assign_bool_(in); }
  JsonValue(const unsigned char& in) { Assign_number_(in); }
  JsonValue(const char& in) { Assign_number_(in); }
  JsonValue(const unsigned short& in) { Assign_number_(in); }
  JsonValue(const short& in) { Assign_number_(in); }
  JsonValue(const unsigned int& in) { Assign_number_(in); }
  JsonValue(const int& in) { Assign_number_(in); }
  JsonValue(const unsigned long& in) { Assign_number_(in); }
  JsonValue(const long& in) { Assign_number_(in); }
  JsonValue(const float& in) { Assign_number_(in); }
  JsonValue(const double& in) { Assign_number_(in); }
  JsonValue(const std::string& in) { Assign_string_(in); }
  JsonValue(const char* in) { Assign_string_(in); }
  ~JsonValue() = default;
  // ---------------------------------------------------------------------------
  // Operators                                                        [ public ]
  // ---------------------------------------------------------------------------
  JsonValue& operator=(const JsonValue& in) {
    data_ = in.data_;
    return *this;
  }
  JsonValue& operator=(JsonValue&& in) noexcept {
    data_ = std::move(in.data_);
    return *this;
  }
  JsonValue& operator=(const nullptr_t& in) {
    Assign_null_(in);
    return *this;
  }
  JsonValue& operator=(const bool& in) {
    Assign_bool_(in);
    return *this;
  }
  JsonValue& operator=(const unsigned char& in) {
    Assign_number_(in);
    return *this;
  }
  JsonValue& operator=(const char& in) {
    Assign_number_(in);
    return *this;
  }
  JsonValue& operator=(const unsigned short& in) {
    Assign_number_(in);
    return *this;
  }
  JsonValue& operator=(const short& in) {
    Assign_number_(in);
    return *this;
  }
  JsonValue& operator=(const unsigned int& in) {
    Assign_number_(in);
    return *this;
  }
  JsonValue& operator=(const int& in) {
    Assign_number_(in);
    return *this;
  }
  JsonValue& operator=(const unsigned long& in) {
    Assign_number_(in);
    return *this;
  }
  JsonValue& operator=(const long& in) {
    Assign_number_(in);
    return *this;
  }
  JsonValue& operator=(const float& in) {
    Assign_number_(in);
    return *this;
  }
  JsonValue& operator=(const double& in) {
    Assign_number_(in);
    return *this;
  }
  JsonValue& operator=(const std::string& in) {
    Assign_string_(in);
    return *this;
  }
  JsonValue& operator=(const char* in) {
    Assign_string_(in);
    return *this;
  }
  JsonValue& operator=(const JsonNull& in) {
    Assign_value_(in);
    return *this;
  }
  JsonValue& operator=(const JsonTrue& in) {
    Assign_value_(in);
    return *this;
  }
  JsonValue& operator=(const JsonFalse& in) {
    Assign_value_(in);
    return *this;
  }
  JsonValue& operator=(const JsonNumber& in) {
    Assign_value_(in);
    return *this;
  }
  JsonValue& operator=(const JsonString& in) {
    Assign_value_(in);
    return *this;
  }
  JsonValue& operator=(const JsonObject& in) {
    Assign_value_(in);
    return *this;
  }
  JsonValue& operator=(const JsonArray& in) {
    Assign_value_(in);
    return *this;
  }
  JsonValue& operator=(JsonNull&& in) noexcept {
    Assign_value_(std::move(in));
    return *this;
  }
  JsonValue& operator=(JsonTrue&& in) noexcept {
    Assign_value_(std::move(in));
    return *this;
  }
  JsonValue& operator=(JsonFalse&& in) noexcept {
    Assign_value_(std::move(in));
    return *this;
  }
  JsonValue& operator=(JsonNumber&& in) noexcept {
    Assign_value_(std::move(in));
    return *this;
  }
  JsonValue& operator=(JsonString&& in) noexcept {
    Assign_value_(std::move(in));
    return *this;
  }
  JsonValue& operator=(JsonObject&& in) noexcept {
    Assign_value_(std::move(in));
    return *this;
  }
  JsonValue& operator=(JsonArray&& in) noexcept {
    Assign_value_(std::move(in));
    return *this;
  }
  operator JsonNull() const { return std::get<JsonNull>(data_); }
  operator JsonTrue() const { return std::get<JsonTrue>(data_); }
  operator JsonFalse() const { return std::get<JsonFalse>(data_); }
  operator JsonNumber() const { return std::get<JsonNumber>(data_); }
  operator JsonString() const { return std::get<JsonString>(data_); }
  operator JsonObject() const { return std::get<JsonObject>(data_); }
  operator JsonArray() const { return std::get<JsonArray>(data_); }
  operator JsonNull&() { return std::get<JsonNull>(data_); }
  operator JsonTrue&() { return std::get<JsonTrue>(data_); }
  operator JsonFalse&() { return std::get<JsonFalse>(data_); }
  operator JsonNumber&() { return std::get<JsonNumber>(data_); }
  operator JsonString&() { return std::get<JsonString>(data_); }
  operator JsonObject&() { return std::get<JsonObject>(data_); }
  operator JsonArray&() { return std::get<JsonArray>(data_); }
  // ---------------------------------------------------------------------------
  // Methods                                                          [ public ]
  // ---------------------------------------------------------------------------
  // Returns the type of the json-value
  JsonValueType Type() const {
    switch (data_.index()) {
      case 0:
        return JsonValueType::kNull;
      case 1:
        return JsonValueType::kTrue;
      case 2:
        return JsonValueType::kFalse;
      case 3:
        return JsonValueType::kNumber;
      case 4:
        return JsonValueType::kString;
      case 5:
        return JsonValueType::kObject;
      case 6:
      default:
        return JsonValueType::kArray;
    }
  }
  // Dumps the current content to string
  base::Stream Serialize() const override {
    switch (data_.index()) {
      case 0:
        return std::get<JsonNull>(data_).Serialize();
      case 1:
        return std::get<JsonTrue>(data_).Serialize();
      case 2:
        return std::get<JsonFalse>(data_).Serialize();
      case 3:
        return std::get<JsonNumber>(data_).Serialize();
      case 4:
        return std::get<JsonString>(data_).Serialize();
      case 5:
        return std::get<JsonObject>(data_).Serialize();
      case 6:
      default:
        return std::get<JsonArray>(data_).Serialize();
    }
  }
  // Parses the incoming string content to generate a json-value
  template <typename NumericParser = DefaultNumericParser>
  static std::optional<JsonValue> From(
      const char* json_content,
      const NumericParser& numeric_parser = DefaultNumericParser()) {
    return From(std::string(json_content, strlen(json_content)));
  }
  // Parses the incoming string content to generate a json-value
  template <typename NumericParser = DefaultNumericParser>
  static std::optional<JsonValue> From(
      const std::string& json_content,
      const NumericParser& numeric_parser = DefaultNumericParser()) {
    std::size_t index = 0;
    auto result = From_(json_content, index, numeric_parser, false);
    if (index != json_content.length()) {
      // [error] -> unexpected value found!
      throw std::logic_error("unexpected value found!");
    }
    return result;
  }

 private:
  // ---------------------------------------------------------------------------
  // Methods                                                         [ private ]
  // ---------------------------------------------------------------------------
  // Parses (in a recursive way) the incoming string content to
  // generate a json-value
  template <typename NumericParser>
  static std::optional<JsonValue> From_(const std::string& json_content,
                                        std::size_t& idx,
                                        const NumericParser& numeric_parser,
                                        const bool& in_collection) {
    std::optional<JsonValue> json_value = {};
    if (idx >= json_content.length()) return json_value;
    // JSON-text = ws value ws
    SkipWs_(json_content, idx);
    auto valueType = CheckValueType_(json_content, idx);
    if (!valueType.has_value() && !in_collection) {
      // [error] -> unexpected value found!
      throw std::logic_error("unexpected value found!");
    } else if (valueType.has_value()) {
      auto type = valueType.value();
      if (type == JsonValueType::kNull) {
        idx += strlen(kNullStr);
        json_value = JsonNull();
      } else if (type == JsonValueType::kTrue) {
        idx += strlen(kTrueStr);
        json_value = JsonTrue();
      } else if (type == JsonValueType::kFalse) {
        idx += strlen(kFalseStr);
        json_value = JsonFalse();
      } else if (type == JsonValueType::kNumber) {
        auto beg = idx;
        idx = GetNextStructuralCharacter_(json_content, idx);
        json_value = numeric_parser.Parse(json_content.substr(beg, idx - beg));
      } else if (type == JsonValueType::kString) {
        auto beg = ++idx;
        bool escaped = false;
        while (idx < json_content.length()) {
          auto const& ch = json_content[idx++];
          if (!escaped) {
            if (ch == kReverseSolidusCh) {
              escaped = true;
            } else if (ch == kQuotationMarkCh) {
              json_value = JsonString(json_content.substr(beg, idx - beg - 1));
              break;
            }
          } else {
            switch (ch) {
              case kQuotationMarkCh:
              case kReverseSolidusCh:
              case kSolidusCh:
              case kBackspaceCh:
              case kFormFeedCh:
              case kLfCh:
              case kCrCh:
              case kHtCh:
                break;
              case kUCh:
                if (idx + 2 > json_content.length()) {
                  // [error] -> unexpected value found!
                  throw std::logic_error("unexpected value found!");
                }
                idx += 2;
                break;
              case kZeroCh:
                if (idx + 1 > json_content.length()) {
                  // [error] -> unexpected value found!
                  throw std::logic_error("unexpected value found!");
                }
                idx++;
                break;
              default:
                // [error] -> unexpected value found!
                throw std::logic_error("unexpected value found!");
            }
            escaped = false;
          }
        }
        if (!json_value.has_value()) {
          // [error] -> unexpected value found!
          throw std::logic_error("unexpected value found!");
        }
      } else if (type == JsonValueType::kObject) {
        idx++;
        JsonObject object;
        bool end_of_object_found = false;
        while (idx < json_content.length()) {
          auto key = From_(json_content, idx, numeric_parser, true);
          if (!key.has_value() || key->Type() != JsonValueType::kString) {
            // [error] -> unexpected value found!
            throw std::logic_error("unexpected value found!");
          }
          auto ch = json_content[idx++];
          if (ch != kColonCh) {
            // [error] -> unexpected value found!
            throw std::logic_error("unexpected value found!");
          }
          auto element = From_(json_content, idx, numeric_parser, true);
          if (!element.has_value()) {
            // [error] -> unexpected value found!
            throw std::logic_error("unexpected value found!");
          }
          object[((JsonString&)key.value()).Get()] = std::move(element.value());
          ch = json_content[idx++];
          if (ch == kRightCurlyBracketCh) {
            end_of_object_found = true;
            break;
          } else if (ch != kCommaCh) {
            break;
          }
        }
        if (!end_of_object_found) {
          // [error] -> unexpected value found!
          throw std::logic_error("unexpected value found!");
        }
        json_value = object;
      } else if (type == JsonValueType::kArray) {
        idx++;
        JsonArray array;
        bool end_of_array_found = false;
        while (idx < json_content.length()) {
          auto element = From_(json_content, idx, numeric_parser, true);
          if (element.has_value()) {
            array.Add(std::move(element.value()));
          }
          auto const& ch = json_content[idx++];
          if (ch == kRightSquareBracketCh) {
            end_of_array_found = true;
            break;
          } else if (ch != kCommaCh) {
            break;
          }
        }
        if (!end_of_array_found) {
          // [error] -> unexpected value found!
          throw std::logic_error("unexpected value found!");
        }
        json_value = array;
      }
    }
    SkipWs_(json_content, idx);
    return json_value;
  }
  // Skips all ws (white-space) characters
  static void SkipWs_(const std::string& json_content, std::size_t& index) {
    while (index < json_content.length()) {
      auto const& ch = json_content[index];
      if (ch != kSpaceCh && ch != kHtCh && ch != kCrCh && ch != kLfCh) break;
      index++;
    }
  }
  // Gets the next character position (within the provided) string that is
  // an structural one
  static std::size_t GetNextStructuralCharacter_(
      const std::string& json_content, const std::size_t& index) {
    auto i = index;
    while (i < json_content.length()) {
      auto const& ch = json_content[i];
      if (ch == kLeftSquareBracketCh || ch == kLeftCurlyBracketCh ||
          ch == kRightSquareBracketCh || ch == kRightCurlyBracketCh ||
          ch == kCommaCh || ch == kColonCh || ch == kQuotationMarkCh)
        break;
      i++;
    }
    return i;
  }
  // Checks for the next element type (returns no value if unknown)
  static std::optional<JsonValueType> CheckValueType_(
      const std::string& json_content, const std::size_t& index) {
    if (json_content[index] == kLeftCurlyBracketCh) {
      return JsonValueType::kObject;
    } else if (json_content[index] == kLeftSquareBracketCh) {
      return JsonValueType::kArray;
    } else if (json_content[index] == kQuotationMarkCh) {
      return JsonValueType::kString;
    } else {
      auto limiter = GetNextStructuralCharacter_(json_content, index);
      auto str = json_content.substr(index, limiter - index);
      if (!str.compare(kNullStr)) {
        return JsonValueType::kNull;
      } else if (!str.compare(kTrueStr)) {
        return JsonValueType::kTrue;
      } else if (!str.compare(kFalseStr)) {
        return JsonValueType::kFalse;
      } else {
        return JsonValueType::kNumber;
      }
    }
    return {};
  }
  // Assigns a <nullptr> value to the current json_value
  void Assign_null_(const std::nullptr_t& in) { data_ = JsonNull(); }
  // Assigns a <bool> value to the current json_value
  void Assign_bool_(const bool& in) {
    if (in) {
      data_ = JsonTrue();
    } else {
      data_ = JsonFalse();
    }
  }
  // Assigns a <numeric> value to the current json_value
  template <typename DAty>
  void Assign_number_(const DAty& in) {
    data_ = JsonNumber(in);
  }
  // Assigns an <string> value to the current json_value
  void Assign_string_(const std::string& in) { data_ = JsonString(in); }
  // Assigns an <string> value to the current json_value
  void Assign_string_(const char* in) { data_ = JsonString(in); }
  // Copies the incoming value to the internal data
  template <typename VAty>
  void Assign_value_(const VAty& in) {
    data_ = in;
  }
  // Moves the incoming value to the internal data
  template <typename VAty>
  void Assign_value_(VAty&& in) noexcept {
    data_ = std::move(in);
  }
  // ---------------------------------------------------------------------------
  // Constants                                                       [ private ]
  // ---------------------------------------------------------------------------
  // Structural-characters
  static constexpr char kLeftSquareBracketCh = 0x5B;
  static constexpr char kLeftCurlyBracketCh = 0x7B;
  static constexpr char kRightSquareBracketCh = 0x5D;
  static constexpr char kRightCurlyBracketCh = 0x7D;
  static constexpr char kColonCh = 0x3A;
  static constexpr char kCommaCh = 0x2C;
  static constexpr char kQuotationMarkCh = 0x22;
  static constexpr char kMinusCh = 0x2D;
  // Ws-characters
  static constexpr char kSpaceCh = 0x20;
  static constexpr char kHtCh = 0x09;
  static constexpr char kLfCh = 0x0A;
  static constexpr char kCrCh = 0x0D;
  // Others
  static constexpr char kReverseSolidusCh = 0x5C;
  static constexpr char kSolidusCh = 0x2F;
  static constexpr char kBackspaceCh = 0x62;
  static constexpr char kFormFeedCh = 0x66;
  static constexpr char kUCh = 0x75;
  static constexpr char kZeroCh = 0x00;
  // Strings
  static constexpr char kNullStr[] = "null";
  static constexpr char kTrueStr[] = "true";
  static constexpr char kFalseStr[] = "false";
  // ---------------------------------------------------------------------------
  // Attributes                                                      [ private ]
  // ---------------------------------------------------------------------------
  InternalType data_;
};
struct ObjectDumper {
  static base::Stream Serialize(const JsonValue& in) { return in.Serialize(); }
};
struct ArrayDumper {
  static base::Stream Serialize(const JsonValue& in) { return in.Serialize(); }
};
}  // namespace koobika::hook::structured::json

#endif