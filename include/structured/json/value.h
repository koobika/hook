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
// structured/json/value.h
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

#ifndef koobika_hook_structured_json_value_h
#define koobika_hook_structured_json_value_h

#include <unordered_map>
#include <variant>
#include <vector>

#include "base/serializable.h"

namespace koobika::hook::structured::json {
// =============================================================================
// USINGs
// =============================================================================
class Value;
using Map = std::unordered_map<std::string, Value>;
using Vector = std::vector<Value>;
using Container =
    std::variant<std::monostate, bool, unsigned char, char, unsigned short,
                 short, unsigned int, int, unsigned long, long, float, double,
                 std::string, Vector, Map>;
// =============================================================================
// FUNCTION-PROTOTYPEs
// =============================================================================
class Object;
class Array;
static void copy(Container&, const Object&);
static void copy(Container&, const Array&);
static void copy(Container&, const Value&);
// =============================================================================
// Value                                                               ( class )
// -----------------------------------------------------------------------------
// This specification holds for JSON value class.
// =============================================================================
class Value : public base::Serializable {
 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  Value() = default;
  template <typename DAty>
  Value(DAty&& in) {
    assign(in);
  }
  virtual ~Value() = default;
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  //
  template <typename DAty>
  Value& operator=(DAty&& in) {
    assign(in);
    return *this;
  }
  template <typename DAty>
  bool operator==(const DAty& in) const {
    return isEqual(in);
  }
  template <typename DAty>
  bool operator<(const DAty& in) const {
    return isLessThan(in);
  }
  template <typename DAty>
  bool operator>(const DAty& in) const {
    return isGreaterThan(in);
  }
  template <typename DAty>
  bool operator<=(const DAty& in) const {
    return !operator>(in);
  }
  template <typename DAty>
  bool operator>=(const DAty& in) const {
    return !operator<(in);
  }
  operator bool() const { return get<bool>({}); }
  operator unsigned char() const { return get<unsigned char>({}); }
  operator char() const { return get<char>({}); }
  operator unsigned short() const { return get<unsigned short>({}); }
  operator short() const { return get<short>({}); }
  operator unsigned int() const { return get<unsigned int>({}); }
  operator int() const { return get<int>({}); }
  operator unsigned long() const { return get<unsigned long>({}); }
  operator long() const { return get<long>({}); }
  operator float() const { return get<float>({}); }
  operator double() const { return get<double>({}); }
  operator std::string() const {
    if (data_->index() != kString_) {
      throw std::logic_error("Not an string!");
    }
    return std::get<std::string>(*data_);
  }
  operator Vector() const {
    if (data_->index() != kVector_) {
      throw std::logic_error("Not an array!");
    }
    return std::get<Vector>(*data_);
  }
  operator Map() const {
    if (data_->index() != kMap_) {
      throw std::logic_error("Not an object!");
    }
    return std::get<Map>(*data_);
  }
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  // Checks if the current value is a 'null' one.
  bool IsNull() const { return data_->index() == kNull_; }
  // Checks if the current value is a 'boolean' one.
  bool IsBool() const { return data_->index() == kBool_; }
  // Checks if the current value is a 'numeric' one.
  bool IsNumber() const {
    return data_->index() >= kUnsignedChar_ && data_->index() <= kDouble_;
  }
  // Checks if the current value is an 'unsigned char' one.
  bool IsUnsignedChar() const { return data_->index() == kUnsignedChar_; }
  // Checks if the current value is an 'char' one.
  bool IsChar() const { return data_->index() == kChar_; }
  // Checks if the current value is an 'unsigned short' one.
  bool IsUnsignedShort() const { return data_->index() == kUnsignedShort_; }
  // Checks if the current value is an 'short' one.
  bool IsShort() const { return data_->index() == kShort_; }
  // Checks if the current value is an 'unsigned int' one.
  bool IsUnsignedInt() const { return data_->index() == kUnsignedInt_; }
  // Checks if the current value is an 'int' one.
  bool IsInt() const { return data_->index() == kInt_; }
  // Checks if the current value is an 'unsigned long' one.
  bool IsUnsignedLong() const { return data_->index() == kUnsignedLong_; }
  // Checks if the current value is an 'long' one.
  bool IsLong() const { return data_->index() == kLong_; }
  // Checks if the current value is an 'float' one.
  bool IsFloat() const { return data_->index() == kFloat_; }
  // Checks if the current value is an 'double' one.
  bool IsDouble() const { return data_->index() == kDouble_; }
  // Checks if the current value is an 'double' one.
  bool IsString() const { return data_->index() == kString_; }
  // Checks if the current value is an 'array' one.
  bool IsArray() const { return data_->index() == kVector_; }
  // Checks if the current value is an 'object' one.
  bool IsObject() const { return data_->index() == kMap_; }
  // Dumps the current content to string.
  base::AutoBuffer Serialize() const override {
    base::AutoBuffer buffer;
    switch (data_->index()) {
      case kNull_:
        buffer.Write(kNullStr_);
        break;
      case kBool_:
        buffer.Write(std::get<bool>(*data_) ? kTrueStr_ : kFalseStr_);
        break;
      case kUnsignedChar_:
        buffer.Write(std::to_string(std::get<unsigned char>(*data_)));
        break;
      case kChar_:
        buffer.Write(std::to_string(std::get<char>(*data_)));
        break;
      case kUnsignedShort_:
        buffer.Write(std::to_string(std::get<unsigned short>(*data_)));
        break;
      case kShort_:
        buffer.Write(std::to_string(std::get<short>(*data_)));
        break;
      case kUnsignedInt_:
        buffer.Write(std::to_string(std::get<unsigned int>(*data_)));
        break;
      case kInt_:
        buffer.Write(std::to_string(std::get<int>(*data_)));
        break;
      case kUnsignedLong_:
        buffer.Write(std::to_string(std::get<unsigned long>(*data_)));
        break;
      case kLong_:
        buffer.Write(std::to_string(std::get<long>(*data_)));
        break;
      case kFloat_:
        buffer.Write(std::to_string(std::get<float>(*data_)));
        break;
      case kDouble_:
        buffer.Write(std::to_string(std::get<double>(*data_)));
        break;
      case kString_:
        buffer.Write("\"").Write(std::get<std::string>(*data_)).Write("\"");
        break;
      case kVector_: {
        auto& storage = std::get<Vector>(*data_);
        buffer.Write('[');
        for (auto itr = storage.begin(); itr != storage.end(); itr++) {
          if (itr != storage.begin()) buffer.Write(',');
          buffer.Write(itr->Serialize());
        }
        buffer.Write(']');
        break;
      }
      case kMap_: {
        auto& storage = std::get<Map>(*data_);
        buffer.Write('{');
        for (auto itr = storage.begin(); itr != storage.end(); itr++) {
          if (itr != storage.begin()) buffer.Write(',');
          buffer.Write('\"')
              .Write(itr->first)
              .Write("\":")
              .Write(itr->second.Serialize());
        }
        buffer.Write('}');
        break;
      }
      default:
        break;
    }
    return buffer;
  }

 protected:
  // ___________________________________________________________________________
  // ATTRIBUTEs                                                    ( protected )
  //
  std::shared_ptr<Container> data_ = std::make_shared<Container>();
  // ___________________________________________________________________________
  // METHODs                                                       ( protected )
  //
  // Assigns the incoming generic type to the current data storage.
  template <typename DAty>
  void assign(const DAty& in) {
    *data_ = in;
  }
  // Assigns the incoming nullptr_t to the current data storage.
  void assign(const std::nullptr_t&) { *data_ = std::monostate(); }
  // Assigns the incoming 'c-style' string to the current data storage.
  void assign(const char* in) { *data_ = std::string{in}; }
  // Assigns the incoming array to the current data storage.
  void assign(const Value& value) { copy(*data_, value); }
  // Assigns the incoming array to the current data storage.
  void assign(const Array& array) { copy(*data_, array); }
  // Assigns the incoming object to the current data storage.
  void assign(const Object& object) { copy(*data_, object); }
  // Returns the stored (numeric) element using its type.
  template <typename OUty>
  OUty get(OUty&& in) const {
    long double up_sized_value = {};
    switch (data_->index()) {
      case kBool_:
        up_sized_value = std::get<bool>(*data_);
        break;
      case kUnsignedChar_:
        up_sized_value = std::get<unsigned char>(*data_);
        break;
      case kChar_:
        up_sized_value = std::get<char>(*data_);
        break;
      case kUnsignedShort_:
        up_sized_value = std::get<unsigned short>(*data_);
        break;
      case kShort_:
        up_sized_value = std::get<short>(*data_);
        break;
      case kUnsignedInt_:
        up_sized_value = std::get<unsigned int>(*data_);
        break;
      case kInt_:
        up_sized_value = std::get<int>(*data_);
        break;
      case kUnsignedLong_:
        up_sized_value = std::get<unsigned long>(*data_);
        break;
      case kLong_:
        up_sized_value = std::get<long>(*data_);
        break;
      case kFloat_:
        up_sized_value = std::get<float>(*data_);
        break;
      case kDouble_:
        up_sized_value = std::get<double>(*data_);
        break;
      case kNull_:
      case kString_:
      case kVector_:
      case kMap_:
      default:
        // ((Error)) -> while trying to process a non-number value!
        throw std::logic_error("Not a number!");
        break;
    }
    constexpr long double min_value = std::numeric_limits<OUty>::min();
    constexpr long double max_value = std::numeric_limits<OUty>::max();
    if (up_sized_value < min_value || up_sized_value > max_value) {
      throw std::logic_error("Stored value will underflow/overflow target!");
    }
    return (OUty)up_sized_value;
  }
  // Checks for the provided value to be equal than the stored one.
  template <typename DAty>
  bool isEqual(const DAty& in) const {
    return this->operator DAty() == in;
  }
  // Checks for the provided value to be equal than the stored one.
  bool isEqual(const Value& in) const {
    return data_ != nullptr && *data_ == *in.data_;
  }
  // Checks for the provided value to be less than the stored one.
  template <typename DAty>
  bool isLessThan(const DAty& in) const {
    return data_ != nullptr && this->operator DAty() < in;
  }
  // Checks for the provided value to be less than the stored one.
  bool isLessThan(const Value& in) const {
    bool less_than = false;
    if (IsNumber() && in.IsNumber()) {
      less_than = this->operator double() < in.operator double();
    } else if (IsString() && in.IsString()) {
      less_than = this->operator std::string() < in.operator std::string();
    } else {
      // ((Error)) -> while comparing incompatible types!
      throw std::logic_error("Trying to compare incompatible types!");
    }
    return less_than;
  }
  // Checks for the provided value to be greater than the stored one.
  template <typename DAty>
  bool isGreaterThan(const DAty& in) const {
    return this->operator DAty() > in;
  }
  // Checks for the provided value to be greater than the stored one.
  bool isGreaterThan(const Value& in) const {
    bool less_than = false;
    if (IsNumber() && in.IsNumber()) {
      less_than = this->operator double() > in.operator double();
    } else if (IsString() && in.IsString()) {
      less_than = this->operator std::string() > in.operator std::string();
    } else {
      // ((Error)) -> while comparing incompatible types!
      throw std::logic_error("Trying to compare incompatible types!");
    }
    return less_than;
  }
  // ___________________________________________________________________________
  // CONSTANTs                                                     ( protected )
  //
  // Variant integral values.
  static const std::size_t kNull_ = 0;
  static const std::size_t kBool_ = 1;
  static const std::size_t kUnsignedChar_ = 2;
  static const std::size_t kChar_ = 3;
  static const std::size_t kUnsignedShort_ = 4;
  static const std::size_t kShort_ = 5;
  static const std::size_t kUnsignedInt_ = 6;
  static const std::size_t kInt_ = 7;
  static const std::size_t kUnsignedLong_ = 8;
  static const std::size_t kLong_ = 9;
  static const std::size_t kFloat_ = 10;
  static const std::size_t kDouble_ = 11;
  static const std::size_t kString_ = 12;
  static const std::size_t kVector_ = 13;
  static const std::size_t kMap_ = 14;
  // String values for serialize functions.
  static constexpr char kNullStr_[] = "null";
  static constexpr char kFalseStr_[] = "false";
  static constexpr char kTrueStr_[] = "true";
  // ___________________________________________________________________________
  // FRIENDs                                                       ( protected )
  //
  friend void copy(Container&, const Object&);
  friend void copy(Container&, const Array&);
  friend void copy(Container&, const Value&);
};
// =============================================================================
// Object                                                              ( class )
// -----------------------------------------------------------------------------
// This specification holds for JSON object class.
// =============================================================================
class Object : public Value {
 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  Object() { *data_ = Map(); }
  Object(const std::initializer_list<std::pair<std::string, Value>>& list)
      : Object() {
    assign(list);
  }
  virtual ~Object() = default;
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  //
  Object& operator=(
      const std::initializer_list<std::pair<std::string, Value>>& list) {
    assign(list);
    return *this;
  }
  const Value& operator[](const char* key) const { return Get(key); }
  Value& operator[](const char* key) { return Get(key); }
  const Value& operator[](const std::string& key) const { return Get(key); }
  Value& operator[](const std::string& key) { return Get(key); }
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  // Adds the specified tuple to the object.
  Value& Add(const std::pair<std::string, Value>& in) {
    Get(in.first) = in.second;
    return *this;
  }
  // Erases the specified value (located by key) from the object.
  Value& Erase(const std::string& key) {
    auto& container = std::get<Map>(*data_);
    auto itr = container.find(key);
    if (itr == container.end()) {
      // ((Error)) -> while querying for a non-existent key!
      throw std::logic_error("Out of bounds!");
    }
    container.erase(key);
    return *this;
  }
  // Checks for an existent key.
  bool Exist(const std::string& key) const {
    auto const& container = std::get<Map>(*data_);
    return container.find(key) != container.end();
  }
  // Returns the associated value for an existent key.
  const Value& Get(const std::string& key) const {
    auto const& container = std::get<Map>(*data_);
    auto itr = container.find(key);
    if (itr == container.end()) {
      // ((Error)) -> while querying for a non-existent key!
      throw std::logic_error("Key not found!");
    }
    return itr->second;
  }
  // Returns the associated value for an existent key.
  Value& Get(const std::string& key) { return std::get<Map>(*data_)[key]; }
  // Returns the associated value for an existent key.
  const Value& At(const std::string& key) const {
    auto const& container = std::get<Map>(*data_);
    auto const itr = container.find(key);
    if (itr == container.end()) {
      // ((Error)) -> while querying for a non-existent key!
      throw std::logic_error("Not found!");
    }
    return itr->second;
  }
  // Returns the associated value for an existent key.
  Value& At(const std::string& key) {
    auto& container = std::get<Map>(*data_);
    auto itr = container.find(key);
    if (itr == container.end()) {
      // ((Error)) -> while querying for a non-existent key!
      throw std::logic_error("Not found!");
    }
    return itr->second;
  }
  // Just to allow range-based iteration.
  auto begin() { return std::get<Map>(*data_).begin(); }
  // Just to allow range-based iteration.
  const auto begin() const { return std::get<Map>(*data_).begin(); }
  // Just to allow range-based iteration.
  auto end() { return std::get<Map>(*data_).end(); }
  // Just to allow range-based iteration.
  const auto end() const { return std::get<Map>(*data_).end(); }

 protected:
  // ___________________________________________________________________________
  // METHODs                                                       ( protected )
  //
  // Assigns a list of elements.
  void assign(
      const std::initializer_list<std::pair<std::string, Value>>& list) {
    for (auto const& element : list) {
      operator[](element.first) = element.second;
    }
  }
};
// =============================================================================
// Array                                                               ( class )
// -----------------------------------------------------------------------------
// This specification holds for JSON array class.
// =============================================================================
class Array : public Value {
 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  Array() { *data_ = Vector(); }
  Array(const Array& in) : Array() { *data_ = *in.data_; }
  Array(Array&&) noexcept = delete;
  Array(const std::initializer_list<Value>& list) : Array() { assign(list); }
  virtual ~Array() = default;
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  //
  Array& operator=(const Array& in) {
    *data_ = *in.data_;
    return *this;
  }
  Array& operator=(Array&&) = delete;
  Array& operator=(const std::initializer_list<Value>& list) {
    assign(list);
    return *this;
  }
  const Value& operator[](const std::size_t& idx) const { return At(idx); }
  Value& operator[](const std::size_t& idx) { return At(idx); }
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  // Adds the specified value to the array.
  Value& Add(const Value& in) {
    std::get<Vector>(*data_).push_back(in);
    return *this;
  }
  // Erases the specified value (located by index) from the array.
  Value& Erase(const std::size_t& position) {
    auto& container = std::get<Vector>(*data_);
    if (position >= container.size()) {
      // ((Error)) -> while querying for an out-of-bounds index!
      throw std::logic_error("Out of bounds!");
    }
    container.erase(container.begin() + position);
    return *this;
  }
  // Erases the specified range (located by indices) from the array.
  Value& Erase(const std::size_t& first, const std::size_t& last) {
    auto& container = std::get<Vector>(*data_);
    if (first >= container.size() || last >= container.size()) {
      // ((Error)) -> while querying for an out-of-bounds index!
      throw std::logic_error("Out of bounds!");
    }
    if (first >= last) {
      // ((Error)) -> while querying for an invalid range!
      throw std::logic_error("Invalid range!");
    }
    container.erase(container.begin() + first, container.begin() + last);
    return *this;
  }
  // Returns the associated value for an existent index.
  const Value& At(const std::size_t& idx) const {
    auto const& container = std::get<Vector>(*data_);
    if (idx >= container.size()) {
      // ((Error)) -> while querying for an out-of-bounds index!
      throw std::logic_error("Out of bounds!");
    }
    return container.at(idx);
  }
  // Returns the associated value for an existent key.
  Value& At(const std::size_t& idx) {
    auto& container = std::get<Vector>(*data_);
    if (idx >= container.size()) {
      // ((Error)) -> while querying for an out-of-bounds index!
      throw std::logic_error("Out of bounds!");
    }
    return container[idx];
  }
  // Just to allow range-based iteration.
  auto begin() { return std::get<Vector>(*data_).begin(); }
  // Just to allow range-based iteration.
  const auto begin() const { return std::get<Vector>(*data_).begin(); }
  // Just to allow range-based iteration.
  auto end() { return std::get<Vector>(*data_).end(); }
  // Just to allow range-based iteration.
  const auto end() const { return std::get<Vector>(*data_).end(); }

 protected:
  // ___________________________________________________________________________
  // METHODs                                                       ( protected )
  //
  // Assigns a list of elements.
  void assign(const std::initializer_list<Value>& list) {
    for (auto const& element : list) {
      Add(element);
    }
  }
};

// =============================================================================
// FUNCTIONs-IMPLEMENTATIONs
// =============================================================================
void copy(Container& out, const Object& in) { copy(out, (const Value&)in); }
void copy(Container& out, const Array& in) { copy(out, (const Value&)in); }
void copy(Container& out, const Value& in) { out = *in.data_; }
}  // namespace koobika::hook::structured::json

#endif