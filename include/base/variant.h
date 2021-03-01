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

#ifndef koobika_hook_encoding_base64_encodertype_h
#define koobika_hook_encoding_base64_encodertype_h

namespace koobika::hook::encoding::base64 {
// =============================================================================
// EncoderType                                                    [ enum class ]
// -----------------------------------------------------------------------------
// This specification holds for the base64 encoder types
// =============================================================================
enum class EncoderType {
  Base64,    // Standard <base64> dictionary.
  Base64Url  // Url <base64> dictionary.
};
}  // namespace koobika::hook::encoding::base64

#endif

#ifndef koobika_hook_base_variant_h
#define koobika_hook_base_variant_h

#include <optional>
#include <string>
#include <variant>

namespace koobika::hook::base {
// =============================================================================
// Variant                                                             [ class ]
// -----------------------------------------------------------------------------
// This specification holds for the default variant type.
// =============================================================================
class Variant {
 public:
  // ---------------------------------------------------------------------------
  // Constructors/Destructors                                         [ public ]
  // ---------------------------------------------------------------------------
  Variant() : data_{} {}
  Variant(const Variant& in) { Set_(in); }
  Variant(Variant&& in) noexcept { Set_(std::move(in)); }
  Variant(const bool& in) { operator=(in); }
  Variant(const unsigned char& in) { operator=(in); }
  Variant(const char& in) { operator=(in); }
  Variant(const unsigned short& in) { operator=(in); }
  Variant(const short& in) { operator=(in); }
  Variant(const unsigned int& in) { operator=(in); }
  Variant(const int& in) { operator=(in); }
  Variant(const unsigned long& in) { operator=(in); }
  Variant(const long& in) { operator=(in); }
  Variant(const float& in) { operator=(in); }
  Variant(const double& in) { operator=(in); }
  Variant(const char* in) { operator=(in); }
  Variant(const std::string& in) { operator=(in); }
  ~Variant() = default;
  // ---------------------------------------------------------------------------
  // Operators                                                        [ public ]
  // ---------------------------------------------------------------------------
  Variant& operator=(const Variant& in) { return Set_(in); }
  Variant& operator=(Variant&& in) noexcept { return Set_(std::move(in)); }
  Variant& operator=(const bool& value) { return Set_(value); }
  Variant& operator=(const unsigned char& value) { return Set_(value); }
  Variant& operator=(const char& value) { return Set_(value); }
  Variant& operator=(const unsigned short& value) { return Set_(value); }
  Variant& operator=(const short& value) { return Set_(value); }
  Variant& operator=(const unsigned int& value) { return Set_(value); }
  Variant& operator=(const int& value) { return Set_(value); }
  Variant& operator=(const unsigned long& value) { return Set_(value); }
  Variant& operator=(const long& value) { return Set_(value); }
  Variant& operator=(const float& value) { return Set_(value); }
  Variant& operator=(const double& value) { return Set_(value); }
  Variant& operator=(const char* value) { return Set_(value); }
  Variant& operator=(const std::string& value) { return Set_(value); }
  // ---------------------------------------------------------------------------
  // Methods                                                          [ public ]
  // ---------------------------------------------------------------------------
  // Returns the stored value (if any) using the provided type specifier.
  template <typename OUty>
  std::optional<OUty> Get() const {
    auto ptr = std::get_if<OUty>(&data_);
    return ptr != nullptr ? OUty(*ptr) : std::optional<OUty>{};
  }
  // Returns the stored value (if any) as string.
  std::optional<std::string> GetAsString() const {
    switch (data_.index()) {
      case kBool_:
        return std::get<kBool_>(data_) ? "true" : "false";
      case kUnsignedChar_:
        return std::to_string(std::get<kUnsignedChar_>(data_));
      case kChar_:
        return std::to_string(std::get<kChar_>(data_));
      case kUnsignedShort_:
        return std::to_string(std::get<kUnsignedShort_>(data_));
      case kShort_:
        return std::to_string(std::get<kShort_>(data_));
      case kUnsignedInt_:
        return std::to_string(std::get<kUnsignedInt_>(data_));
      case kInt_:
        return std::to_string(std::get<kInt_>(data_));
      case kUnsignedLong_:
        return std::to_string(std::get<kUnsignedLong_>(data_));
      case kLong_:
        return std::to_string(std::get<kLong_>(data_));
      case kFloat_:
        return std::to_string(std::get<kFloat_>(data_));
      case kDouble_:
        return std::to_string(std::get<kDouble_>(data_));
      case kString_:
        return std::get<kString_>(data_);
    }
    return {};
  }

 private:
  // ---------------------------------------------------------------------------
  // Methods                                                         [ private ]
  // ---------------------------------------------------------------------------
  // Copies provided variant to the internal one.
  Variant& Set_(const Variant& in) {
    data_ = in.data_;
    return *this;
  }
  // Moves the provided variant to the internal one.
  Variant& Set_(Variant&& in) noexcept {
    data_ = std::move(in.data_);
    return *this;
  }
  // Copies the provided element to the internal data.
  template <typename DAty>
  Variant& Set_(const DAty& in) {
    data_ = in;
    return *this;
  }
  // Copies the provided string to the internal data.
  Variant& Set_(const char* in) {
    data_ = std::string(in);
    return *this;
  }
  // Copies the provided string to the internal data.
  Variant& Set_(const std::string& in) {
    data_ = in;
    return *this;
  }
  // ---------------------------------------------------------------------------
  // Constants                                                       [ private ]
  // ---------------------------------------------------------------------------
  static constexpr int kMonostate_ = 0;
  static constexpr int kBool_ = 1;
  static constexpr int kUnsignedChar_ = 2;
  static constexpr int kChar_ = 3;
  static constexpr int kUnsignedShort_ = 4;
  static constexpr int kShort_ = 5;
  static constexpr int kUnsignedInt_ = 6;
  static constexpr int kInt_ = 7;
  static constexpr int kUnsignedLong_ = 8;
  static constexpr int kLong_ = 9;
  static constexpr int kFloat_ = 10;
  static constexpr int kDouble_ = 11;
  static constexpr int kString_ = 12;
  // ---------------------------------------------------------------------------
  // Atributes                                                       [ private ]
  // ---------------------------------------------------------------------------
  std::variant<std::monostate, bool, unsigned char, char, unsigned short, short,
               unsigned int, int, unsigned long, long, float, double,
               std::string>
      data_;
};
}  // namespace koobika::hook::base

#endif