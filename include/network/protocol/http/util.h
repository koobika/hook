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
// network/protocol/http/util.h
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

#ifndef koobika_hook_network_protocol_http_util_h
#define koobika_hook_network_protocol_http_util_h

#include <string>

#include "constants/characters.h"
#include "constants/strings.h"

namespace koobika::hook::network::protocol::http {
// =============================================================================
// Util                                                                ( class )
// -----------------------------------------------------------------------------
// This specification holds for http utility class.
// =============================================================================
class Util {
 public:
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  // Separators = "(" | ")" | "<" | ">" | "@" | "," | ";" | ":" | "\" | <"> |
  //              "/" | "[" | "]" | "?" | "=" | "{" | "}" | SP | HT
  static bool IsSeparator(const char& character) {
    return character == '(' || character == ')' || character == '<' ||
           character == '>' || character == '@' || character == ',' ||
           character == ';' || character == ':' || character == '\\' ||
           character == '"' || character == '/' || character == '[' ||
           character == ']' || character == '?' || character == '=' ||
           character == '{' || character == '}' ||
           character == constants::Characters::kSpace ||
           character == constants::Characters::kHt;
  }
  // CTL = <any US-ASCII control character (octets 0 - 31) and DEL (127)>
  static bool IsCTL(const char& character) {
    return (character >= 0 && character <= 31) || character == 127;
  }
  // LWS = [CRLF] 1*( SP | HT )
  static std::size_t SkipLWS(const char* str, const std::size_t& length) {
    std::size_t i = 0;
    do {
      if (length > 1) {
        if (str[0] == constants::Characters::kCr) {
          if (str[1] != constants::Characters::kLf) {
            break;
          }
          i += 2;
        }
      }
      while (i < length) {
        if (str[i] != constants::Characters::kSpace &&
            str[i] != constants::Characters::kHt) {
          break;
        }
        i++;
      }
    } while (false);
    return i;
  }
  // Token = 1*<any CHAR except CTLs or separators>
  static bool IsToken(const std::string& str) {
    if (!str.length()) return false;
    for (std::size_t i = 0; i < str.length(); i++) {
      if (IsCTL(str[i]) || IsSeparator(str[i])) return false;
    }
    return true;
  }
  // TEXT = <any OCTET except CTLs, but including LWS>
  static bool IsTEXT(const std::string& str) {
    for (std::size_t i = 0; i < str.length(); i++) {
      if (IsCTL(str[i])) return false;
    }
    return true;
  }
  // LWS = [CRLF] 1*( SP | HT )
  static std::string& RemoveCRLFs(std::string&& str) {
    while (true) {
      auto crlf = str.find(constants::Strings::kCrLf);
      if (crlf == std::string::npos) break;
      auto start = crlf + strlen(constants::Strings::kCrLf), src = start;
      while (str[src] == constants::Characters::kSpace ||
             str[src] == constants::Characters::kHt) {
        src++;
      }
      str.erase(crlf, src - crlf);
    }
    return str;
  }
  // Compares (case-sensitive/insensitive mode) two strings.
  static bool Compare(const std::string& s, const std::string& t,
                      const bool& case_sensitive = true) {
    return std::equal(s.begin(), s.end(), t.begin(), t.end(),
                      [&case_sensitive](char sc, char tc) {
                        return case_sensitive ? sc == tc
                                              : tolower(sc) == tolower(tc);
                      });
  }
};
}  // namespace koobika::hook::network::protocol::http

#endif