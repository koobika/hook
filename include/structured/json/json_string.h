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

#ifndef koobika_hook_structured_json_jsonstring_h
#define koobika_hook_structured_json_jsonstring_h

#include "base/serializable.h"

namespace koobika::hook::structured::json {
// =============================================================================
// JsonString                                                          ( class )
// -----------------------------------------------------------------------------
// This specification holds for JSON string default class.
// =============================================================================
class JsonString : public base::Serializable {
 public:
  // ---------------------------------------------------------------------------
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  // ---------------------------------------------------------------------------
  JsonString() = default;
  JsonString(const JsonString&) = default;
  JsonString(JsonString&&) noexcept = default;
  JsonString(const std::string& in) : value_{in} {}
  JsonString(const char* in) : value_{in} {}
  ~JsonString() = default;
  // ---------------------------------------------------------------------------
  // OPERATORs                                                        ( public )
  // ---------------------------------------------------------------------------
  JsonString& operator=(const JsonString& in) {
    value_ = in.value_;
    return *this;
  }
  JsonString& operator=(JsonString&& in) noexcept {
    value_ = std::move(in.value_);
    return *this;
  }
  // ---------------------------------------------------------------------------
  // METHODs                                                          ( public )
  // ---------------------------------------------------------------------------
  // Sets the json-value with the specified string.
  JsonString& Set(const std::string& in) {
    value_ = in;
    return *this;
  }
  // Gets the stored json-value.
  std::string Get() const { return value_; }
  // Dumps the current content to string.
  base::Stream Serialize() const override {
    return base::Stream("\"").Write(value_).Write("\"");
  }

 private:
  // ---------------------------------------------------------------------------
  // ATTRIBUTEs                                                      ( private )
  // ---------------------------------------------------------------------------
  std::string value_;
};
}  // namespace koobika::hook::structured::json

#endif