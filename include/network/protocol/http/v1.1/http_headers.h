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

#ifndef koobika_hook_network_protocol_http_v11_httpheaders_h
#define koobika_hook_network_protocol_http_v11_httpheaders_h

#include <algorithm>

#include "base/serializable.h"
#include "base/variant.h"
#include "http_constants.h"
#include "http_util.h"

namespace koobika::hook::network::protocol::http::v11 {
// =============================================================================
// HttpHeaders                                                         [ class ]
// -----------------------------------------------------------------------------
// This class is in charge of providing the http headers class
// =============================================================================
class HttpHeaders : public base::Serializable {
 private:
  // ---------------------------------------------------------------------------
  // Usings                                                          [ private ]
  // ---------------------------------------------------------------------------
  using VariantList_ = std::initializer_list<base::Variant>;

 public:
  // ---------------------------------------------------------------------------
  // Constructors/Destructors                                         [ public ]
  // ---------------------------------------------------------------------------
  HttpHeaders() = default;
  HttpHeaders(const std::string& raw_content) { raw_.assign(raw_content); }
  HttpHeaders(const char* raw_content) { raw_.assign(raw_content); }
  HttpHeaders(const HttpHeaders&) = default;
  HttpHeaders(HttpHeaders&&) noexcept = default;
  ~HttpHeaders() = default;
  // ---------------------------------------------------------------------------
  // Operators                                                        [ public ]
  // ---------------------------------------------------------------------------
  HttpHeaders& operator=(const HttpHeaders&) = default;
  HttpHeaders& operator=(HttpHeaders&&) noexcept = default;
  // ---------------------------------------------------------------------------
  // Methods                                                          [ public ]
  // ---------------------------------------------------------------------------
  // Cleans up all stored information
  void Clear() { raw_.clear(); }
  // Sets a header entry (templated version)
  template <typename DAty>
  void Set(const char* name, const DAty& value) {
    Set(name, std::to_string(value).c_str());
  }
  // Sets a header entry
  void Set(const char* name, const std::string& value) {
    if (!HttpUtil::IsToken(name)) {
      // [error] -> specified name is NOT a token!
      // [to-do] -> raise an exception?
      return;
    }
    if (!HttpUtil::IsTEXT(value)) {
      // [error] -> specified value is NOT text!
      // [to-do] -> raise an exception?
      return;
    }
    SetFieldName_(name);
    SetFieldValue_(value);
  }
  // Sets a header entry
  void Set(const char* name, const char* value) {
    if (!HttpUtil::IsToken(name)) {
      // [error] -> specified name is NOT a token!
      // [to-do] -> raise an exception?
      return;
    }
    if (!HttpUtil::IsTEXT(value)) {
      // [error] -> specified value is NOT text!
      // [to-do] -> raise an exception?
      return;
    }
    SetFieldName_(name);
    SetFieldValue_(value);
  }
  // Sets a header entry (variant-list version)
  void Set(const std::string& name, const VariantList_& values) {
    if (!HttpUtil::IsToken(name)) {
      // [error] -> specified name is NOT a token!
      // [to-do] -> raise an exception?
      return;
    }
    std::size_t length = values.size(), i = 0;
    SetFieldName_(name);
    for (auto const& value : values) {
      auto const& element_value = value.GetAsString();
      if (!element_value.has_value()) continue;
      if (!HttpUtil::IsTEXT(element_value.value())) {
        // [error] -> specified value is NOT text!
        // [to-do] -> raise an exception?
        continue;
      }
      SetFieldValue_(element_value.value().data(), i++, i == length - 1);
    }
  }
  // Sets a header entry (initializer-list version)
  void Set(const std::pair<std::string, VariantList_>& elements) {
    Set(elements.first, elements.second);
  }
  // Returns header entry using its name (const reference&)
  std::optional<std::string> Get(const std::string& name) const {
    std::size_t begin = 0, end;
    std::string crlf(HttpConstants::Strings::kCrLf);
    while (true) {
      end = raw_.find(HttpConstants::Strings::kCrLf, begin);
      if (end == std::string::npos) break;
      auto found =
          std::search(raw_.begin() + begin, raw_.begin() + end, name.begin(),
                      name.end(), [](char ch1, char ch2) {
                        return std::toupper(ch1) == std::toupper(ch2);
                      });
      if (found != (raw_.begin() + end)) {
        std::size_t off = found - raw_.begin();
        auto sep = raw_.find(HttpConstants::Strings::kHeaderFieldNameSeparator,
                             found - raw_.begin());
        if (sep < end) {
          auto skipped = HttpUtil::SkipLWS(
              &raw_.data()
                   [sep + HttpConstants::Strings::kHeaderFieldNameSeparatorLen],
              end - sep);
          return std::string(
              &raw_.data()
                   [sep + HttpConstants::Strings::kHeaderFieldNameSeparatorLen +
                    skipped],
              end - sep - HttpConstants::Strings::kHeaderFieldNameSeparatorLen -
                  skipped);
        }
      }
      begin = end + HttpConstants::Strings::kCrLfLen;
    }
    return {};
  }
  // Returns all header entries (const reference&)
  base::Stream Serialize() const override { return raw_; }
  // Checks for the specified header entry (using its name)
  bool Exist(const std::string& name) const {
    std::size_t begin = 0, end;
    std::string crlf(HttpConstants::Strings::kCrLf);
    while (true) {
      end = raw_.find(HttpConstants::Strings::kCrLf, begin);
      if (end == std::string::npos) break;
      auto found =
          std::search(raw_.begin() + begin, raw_.begin() + end, name.begin(),
                      name.end(), [](char ch1, char ch2) {
                        return std::toupper(ch1) == std::toupper(ch2);
                      });
      if (found != (raw_.begin() + end)) {
        return true;
      }
      begin = end + HttpConstants::Strings::kCrLfLen;
    }
  }

 private:
  // ---------------------------------------------------------------------------
  // Methods                                                         [ private ]
  // ---------------------------------------------------------------------------
  // Sets header field name
  void SetFieldName_(const std::string& name) {
    std::size_t begin = 0, end;
    std::string crlf(HttpConstants::Strings::kCrLf);
    while (true) {
      end = raw_.find(HttpConstants::Strings::kCrLf, begin);
      if (end == std::string::npos) break;
      auto found =
          std::search(raw_.begin() + begin, raw_.begin() + end, name.begin(),
                      name.end(), [](char ch1, char ch2) {
                        return std::toupper(ch1) == std::toupper(ch2);
                      });
      if (found != (raw_.begin() + end)) {
        raw_.erase(begin, (end - begin) + HttpConstants::Strings::kCrLfLen);
      }
      begin = end + HttpConstants::Strings::kCrLfLen;
    } 
    raw_.append(name);
    raw_.append(HttpConstants::Strings::kHeaderFieldNameSeparator);
    raw_.append(HttpConstants::Strings::kSpace);
  }
  // Sets header field value
  void SetFieldValue_(const std::string& val, const bool& add_pre_comma = false,
                      const bool& add_trailing_crlf = true) {
    if (add_pre_comma) {
      raw_.append(HttpConstants::Strings::kHeaderFieldValueSeparator);
      raw_.append(HttpConstants::Strings::kSpace);
    }
    raw_.append(HttpUtil::RemoveCRLFs(std::move(std::string(val))).data());
    if (add_trailing_crlf) {
      raw_.append(HttpConstants::Strings::kCrLf);
    }
  }
  // ---------------------------------------------------------------------------
  // Attributes                                                      [ private ]
  // ---------------------------------------------------------------------------
  std::string raw_;
};
}  // namespace koobika::hook::network::protocol::http::v11

#endif