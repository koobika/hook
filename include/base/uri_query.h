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
// base/uri_query.h
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

#ifndef koobika_hook_base_uriquery_h
#define koobika_hook_base_uriquery_h

#include "platform.h"

namespace koobika::hook::base {
// =============================================================================
// UriQuery                                                            ( class )
// -----------------------------------------------------------------------------
// This class holds for the default uri query implementation.
// =============================================================================
class UriQuery {
 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  UriQuery() = default;
  UriQuery(const std::string& content) : content_{content} {}
  UriQuery(const char* content) : content_{content} {}
  UriQuery(const UriQuery&) = default;
  UriQuery(UriQuery&&) noexcept = default;
  ~UriQuery() = default;
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  //
  UriQuery& operator=(const UriQuery&) = default;
  UriQuery& operator=(UriQuery&&) noexcept = default;
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  // Sets the stored content.
  void Set(const std::string& content) { content_ = content; }
  // Returns the stored content as an string.
  std::string Get() const { return content_; }
  // Returns the content associated to a parameter key (if exists).
  std::optional<std::string> Get(const std::string& key) {
    std::size_t pos = 1;
    do {
      if (pos >= content_.length()) break;
      auto amp_pos = content_.find(kAmpersand_, pos);
      if (amp_pos == std::string::npos) amp_pos = content_.length();
      auto sub = content_.substr(pos, amp_pos - pos);
      auto equ_pos = sub.find(kEquals_);
      if (equ_pos != std::string::npos) {
        auto nam = sub.substr(0, equ_pos);
        auto val = sub.substr(equ_pos + 1);
        if (!nam.compare(key)) {
          return val;
        }
      }
      pos = amp_pos + 1;
    } while (true);
    return {};
  }

 private:
  // ___________________________________________________________________________
  // CONSTANTs                                                       ( private )
  //
  static constexpr char kAmpersand_ = '&';
  static constexpr char kEquals_ = '=';
  // ___________________________________________________________________________
  // ATTRIBUTEs                                                      ( private )
  //
  std::string content_;
};
}  // namespace koobika::hook::base

#endif