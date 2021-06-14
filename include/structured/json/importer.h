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
// structured/json/importer.h
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

#ifndef koobika_hook_structured_json_importer_h
#define koobika_hook_structured_json_importer_h

#include <algorithm>

#include "value.h"

namespace koobika::hook::structured::json {
// =============================================================================
// DefaultNumericParser                                               ( struct )
// -----------------------------------------------------------------------------
// This specification holds for JSON default numeric parser module.
// =============================================================================
struct DefaultNumericParser {
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  // Performs a parsing of the incoming string to return a json-number.
  Value Parse(const std::string& str) const {
    double number = atof(str.data());
    if (str.find(kDotCh_) == std::string::npos) {
      if (number >= kSCMin_ && number <= kSCMax_) {
        return (char)number;
      } else if (number >= kUCMin_ && number <= kUCMax_) {
        return (unsigned char)number;
      } else if (number >= kSSMin_ && number <= kSSMax_) {
        return (short)number;
      } else if (number >= kUSMin_ && number <= kUSMax_) {
        return (unsigned short)number;
      } else if (number >= kSIMin_ && number <= kSIMax_) {
        return (int)number;
      } else if (number >= kUIMin_ && number <= kUIMax_) {
        return (unsigned int)number;
      } else if (number >= kSLMin_ && number <= kSLMax_) {
        return (long)number;
      } else if (number >= kULMin_ && number <= kULMax_) {
        return (unsigned long)number;
      }
    }
    return number;
  }

 private:
  // ___________________________________________________________________________
  // CONSTANTs                                                       ( private )
  //
  // Structural-characters.
  static constexpr char kDotCh_ = 0x2E;
  // Thresholds.
  static constexpr long double kUCMin_ =
      std::numeric_limits<unsigned char>::min();
  static constexpr long double kUCMax_ =
      std::numeric_limits<unsigned char>::max();
  static constexpr long double kSCMin_ = std::numeric_limits<char>::min();
  static constexpr long double kSCMax_ = std::numeric_limits<char>::max();
  static constexpr long double kUSMin_ =
      std::numeric_limits<unsigned short>::min();
  static constexpr long double kUSMax_ =
      std::numeric_limits<unsigned short>::max();
  static constexpr long double kSSMin_ = std::numeric_limits<short>::min();
  static constexpr long double kSSMax_ = std::numeric_limits<short>::max();
  static constexpr long double kUIMin_ =
      std::numeric_limits<unsigned int>::min();
  static constexpr long double kUIMax_ =
      std::numeric_limits<unsigned int>::max();
  static constexpr long double kSIMin_ = std::numeric_limits<int>::min();
  static constexpr long double kSIMax_ = std::numeric_limits<int>::max();
  static constexpr long double kULMin_ =
      std::numeric_limits<unsigned long>::min();
  static constexpr long double kULMax_ =
      std::numeric_limits<unsigned long>::max();
  static constexpr long double kSLMin_ = std::numeric_limits<long>::min();
  static constexpr long double kSLMax_ = std::numeric_limits<long>::max();
};
// =============================================================================
// Importer                                                            ( class )
// -----------------------------------------------------------------------------
// This specification holds for JSON importer class.
// =============================================================================
class Importer {
 public:
  // ___________________________________________________________________________
  // STATIC-METHODs                                                   ( public )
  //
  // Parses the incoming string content to generate a json-value.
  template <typename NumericParser = DefaultNumericParser>
  static Value From(const char* content,
                    const NumericParser& numeric_parser = NumericParser()) {
    std::size_t index = 0, length = strlen(content);
    auto res = From(content, length, index, numeric_parser, false);
    if (index != length) {
      // ((Error)) -> unexpected value found!
      throw std::logic_error("Bad formed document!");
    }
    return res.has_value() ? res.value() : nullptr;
  }
  // Parses the incoming string content to generate a json-value.
  template <typename NumericParser = DefaultNumericParser>
  static Value From(const std::string& str,
                    const NumericParser& numeric_parser = NumericParser()) {
    return From(str.c_str(), numeric_parser);
  }
  // Parses the incoming string content to generate a json-value.
  template <typename NumericParser = DefaultNumericParser>
  static Value From(std::ifstream&& stream,
                    const NumericParser& numeric_parser = NumericParser()) {
    return From(base::AutoBuffer(stream).ToString(), numeric_parser);
  }
  // Parses the incoming string content to generate a json-value.
  template <typename NumericParser = DefaultNumericParser>
  static Value From(std::ifstream& stream,
                    const NumericParser& numeric_parser = NumericParser()) {
    return From(base::AutoBuffer(stream).ToString(), numeric_parser);
  }

 private:
  // ___________________________________________________________________________
  // TYPEs                                                           ( private )
  //
  enum class ValueType {
    kNull,
    kTrue,
    kFalse,
    kNumber,
    kString,
    kArray,
    kObject
  };

  // ___________________________________________________________________________
  // STATIC-METHODs                                                  ( private )
  //
  // Parses (in a recursive way) the incoming string content!
  template <typename NumericParser>
  static std::optional<Value> From(const char* content,
                                   const std::size_t& length, std::size_t& idx,
                                   const NumericParser& numeric_parser,
                                   const bool& in_collection) {
    std::optional<Value> value = {};
    if (idx >= length) return value;
    // JSON-text = ws value ws
    skipWs(content, length, idx);
    auto valueType = checkValueType(content, length, idx);
    if (!valueType.has_value() && !in_collection) {
      // ((Error)) -> unexpected value found!
      throw std::logic_error("Bad formed document!");
    } else if (valueType.has_value()) {
      auto type = valueType.value();
      if (type == ValueType::kNull) {
        idx += strlen(kNullStr_);
        value = nullptr;
      } else if (type == ValueType::kTrue) {
        idx += strlen(kTrueStr_);
        value = true;
      } else if (type == ValueType::kFalse) {
        idx += strlen(kFalseStr_);
        value = false;
      } else if (type == ValueType::kNumber) {
        auto beg = idx;
        idx = getNextStructuralCharacter(content, length, idx);
        value = numeric_parser.Parse(std::string(&content[beg], idx - beg));
      } else if (type == ValueType::kString) {
        auto beg = ++idx;
        bool escaped = false;
        while (idx < length) {
          auto const& ch = content[idx++];
          if (!escaped) {
            if (ch == kReverseSolidusCh_) {
              escaped = true;
            } else if (ch == kQuotationMarkCh_) {
              value = std::string(&content[beg], idx - beg - 1);
              break;
            }
          } else {
            switch (ch) {
              case kQuotationMarkCh_:
              case kReverseSolidusCh_:
              case kSolidusCh_:
              case kBackspaceCh_:
              case kFormFeedCh_:
              case kLfCh_:
              case kCrCh_:
              case kHtCh_:
                break;
              case kUCh_:
                if (idx + 2 > length) {
                  // ((Error)) -> unexpected value found!
                  throw std::logic_error("Bad formed document!");
                }
                idx += 2;
                break;
              case kZeroCh_:
                if (idx + 1 > length) {
                  // ((Error)) -> unexpected value found!
                  throw std::logic_error("Bad formed document!");
                }
                idx++;
                break;
              default:
                // ((Error)) -> unexpected value found!
                throw std::logic_error("Bad formed document!");
            }
            escaped = false;
          }
        }
        if (!value.has_value()) {
          // ((Error)) -> unexpected value found!
          throw std::logic_error("Bad formed document!");
        }
      } else if (type == ValueType::kObject) {
        idx++;
        Object object;
        bool end_of_object_found = false;
        while (idx < length) {
          auto key = From(content, length, idx, numeric_parser, true);
          if (!key.has_value() || !key->IsString()) {
            // ((Error)) -> unexpected value found!
            throw std::logic_error("Bad formed document!");
          }
          auto ch = content[idx++];
          if (ch != kColonCh_) {
            // ((Error)) -> unexpected value found!
            throw std::logic_error("Bad formed document!");
          }
          auto element = From(content, length, idx, numeric_parser, true);
          if (!element.has_value()) {
            // ((Error)) -> unexpected value found!
            throw std::logic_error("Bad formed document!");
          }
          object[(std::string)key.value()] = std::move(element.value());
          ch = content[idx++];
          if (ch == kRightCurlyBracketCh_) {
            end_of_object_found = true;
            break;
          } else if (ch != kCommaCh_) {
            break;
          }
        }
        if (!end_of_object_found) {
          // ((Error)) -> unexpected value found!
          throw std::logic_error("Bad formed document!");
        }
        value = object;
      } else if (type == ValueType::kArray) {
        idx++;
        Array array;
        bool end_of_array_found = false;
        while (idx < length) {
          auto element = From(content, length, idx, numeric_parser, true);
          if (element.has_value()) {
            array.Add(std::move(element.value()));
          }
          auto const& ch = content[idx++];
          if (ch == kRightSquareBracketCh_) {
            end_of_array_found = true;
            break;
          } else if (ch != kCommaCh_) {
            break;
          }
        }
        if (!end_of_array_found) {
          // ((Error)) -> unexpected value found!
          throw std::logic_error("Bad formed document!");
        }
        value = array;
      }
    }
    skipWs(content, length, idx);
    return value;
  }
  // Skips all ws (white-space) characters.
  static void skipWs(const char* content, const std::size_t& length,
                     std::size_t& index) {
    while (index < length) {
      auto const& ch = content[index];
      if (ch != kSpaceCh_ && ch != kHtCh_ && ch != kCrCh_ && ch != kLfCh_)
        break;
      index++;
    }
  }
  // Gets the next character position (within the provided) string that is
  // an structural one.
  static std::size_t getNextStructuralCharacter(const char* content,
                                                const std::size_t& length,
                                                const std::size_t& index) {
    auto i = index;
    while (i < length) {
      auto const& ch = content[i];
      if (ch == kLeftSquareBracketCh_ || ch == kLeftCurlyBracketCh_ ||
          ch == kRightSquareBracketCh_ || ch == kRightCurlyBracketCh_ ||
          ch == kCommaCh_ || ch == kColonCh_ || ch == kQuotationMarkCh_)
        break;
      i++;
    }
    return i;
  }
  // Checks for the next element type (returns no value if unknown).
  static std::optional<ValueType> checkValueType(const char* content,
                                                 const std::size_t& length,
                                                 const std::size_t& index) {
    if (index < length) {
      if (content[index] == kLeftCurlyBracketCh_) {
        return ValueType::kObject;
      } else if (content[index] == kLeftSquareBracketCh_) {
        return ValueType::kArray;
      } else if (content[index] == kQuotationMarkCh_) {
        return ValueType::kString;
      } else {
        auto limiter = getNextStructuralCharacter(content, length, index);
        auto str = std::string(&content[index], limiter - index);
        if (str.length()) {
          std::transform(str.begin(), str.end(), str.begin(), ::tolower);
          if (!str.compare(kNullStr_)) {
            return ValueType::kNull;
          } else if (!str.compare(kTrueStr_)) {
            return ValueType::kTrue;
          } else if (!str.compare(kFalseStr_)) {
            return ValueType::kFalse;
          } else {
            return ValueType::kNumber;
          }
        }
      }
    }
    return {};
  }
  // ___________________________________________________________________________
  // CONSTANTs                                                       ( private )
  //
  // Structural-characters.
  static constexpr char kLeftSquareBracketCh_ = 0x5B;
  static constexpr char kLeftCurlyBracketCh_ = 0x7B;
  static constexpr char kRightSquareBracketCh_ = 0x5D;
  static constexpr char kRightCurlyBracketCh_ = 0x7D;
  static constexpr char kColonCh_ = 0x3A;
  static constexpr char kCommaCh_ = 0x2C;
  static constexpr char kQuotationMarkCh_ = 0x22;
  static constexpr char kMinusCh_ = 0x2D;
  // Ws-characters.
  static constexpr char kSpaceCh_ = 0x20;
  static constexpr char kHtCh_ = 0x09;
  static constexpr char kLfCh_ = 0x0A;
  static constexpr char kCrCh_ = 0x0D;
  // Others.
  static constexpr char kReverseSolidusCh_ = 0x5C;
  static constexpr char kSolidusCh_ = 0x2F;
  static constexpr char kBackspaceCh_ = 0x62;
  static constexpr char kFormFeedCh_ = 0x66;
  static constexpr char kUCh_ = 0x75;
  static constexpr char kZeroCh_ = 0x00;
  // String values for serialize functions.
  static constexpr char kNullStr_[] = "null";
  static constexpr char kFalseStr_[] = "false";
  static constexpr char kTrueStr_[] = "true";
};
}  // namespace koobika::hook::structured::json

#endif