﻿// /////////////////////////////////////////////////////////////////////////////
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
// RFC2396 - Uri specification
// A. Collected BNF for URI
//
// URI-reference = [ absoluteURI | relativeURI ] [ "#" fragment ]
// absoluteURI   = scheme ":" ( hier_part | opaque_part )
// relativeURI   = ( net_path | abs_path | rel_path ) [ "?" query ]
// hier_part     = ( net_path | abs_path ) [ "?" query ]
// opaque_part   = uric_no_slash *uric
// uric_no_slash = unreserved | escaped | ";" | "?" | ":" | "@" |
//                 "&" | "=" | "+" | "$" | ","
// net_path      = "//" authority [ abs_path ]
// abs_path      = "/"  path_segments
// rel_path      = rel_segment [ abs_path ]
// rel_segment   = 1*( unreserved | escaped |
//                 ";" | "@" | "&" | "=" | "+" | "$" | "," )
// scheme        = alpha *( alpha | digit | "+" | "-" | "." )
// authority     = server | reg_name
// reg_name      = 1*( unreserved | escaped | "$" | "," |
//                 ";" | ":" | "@" | "&" | "=" | "+" )
// server        = [ [ userinfo "@" ] hostport ]
// userinfo      = *( unreserved | escaped |
//                 ";" | ":" | "&" | "=" | "+" | "$" | "," )
// hostport      = host [ ":" port ]
// host          = hostname | IPv4address
// hostname      = *( domainlabel "." ) toplabel [ "." ]
// domainlabel   = alphanum | alphanum *( alphanum | "-" ) alphanum
// toplabel      = alpha | alpha *( alphanum | "-" ) alphanum
// IPv4address   = 1*digit "." 1*digit "." 1*digit "." 1*digit
// port          = *digit
// path          = [ abs_path | opaque_part ]
// path_segments = segment *( "/" segment )
// segment       = *pchar *( ";" param )
// param         = *pchar
// pchar         = unreserved | escaped |
//                 ":" | "@" | "&" | "=" | "+" | "$" | ","
// query         = *uric
// fragment      = *uric
// uric          = reserved | unreserved | escaped
// reserved      = ";" | "/" | "?" | ":" | "@" | "&" | "=" | "+" |
//                 "$" | ","
// unreserved    = alphanum | mark
// mark          = "-" | "_" | "." | "!" | "~" | "*" | "'" |
//                 "(" | ")"
// escaped       = "%" hex hex
// hex           = digit | "A" | "B" | "C" | "D" | "E" | "F" |
//                 "a" | "b" | "c" | "d" | "e" | "f"
// alphanum      = alpha | digit
// alpha         = lowalpha | upalpha
// lowalpha      = "a" | "b" | "c" | "d" | "e" | "f" | "g" | "h" | "i" |
//                 "j" | "k" | "l" | "m" | "n" | "o" | "p" | "q" | "r" |
//                 "s" | "t" | "u" | "v" | "w" | "x" | "y" | "z"
// upalpha       = "A" | "B" | "C" | "D" | "E" | "F" | "G" | "H" | "I" |
//                 "J" | "K" | "L" | "M" | "N" | "O" | "P" | "Q" | "R" |
//                 "S" | "T" | "U" | "V" | "W" | "X" | "Y" | "Z"
// digit         = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" |
//                 "8" | "9"
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

#ifndef koobika_hook_base_uri_h
#define koobika_hook_base_uri_h

#include "uri_reader.h"

namespace koobika::hook::base {
// =============================================================================
// Uri                                                                 ( class )
// -----------------------------------------------------------------------------
// This class holds for the default uri implementation.
// =============================================================================
class Uri : public UriReader {
 public:
  // ---------------------------------------------------------------------------
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  // ---------------------------------------------------------------------------
  Uri() = default;
  Uri(const std::string& str) { parseFull(str.data(), str.length()); }
  Uri(const char* str) { parseFull(str, strlen(str)); }
  Uri(const Uri&) = default;
  Uri(Uri&&) noexcept = default;
  ~Uri() = default;
  // ---------------------------------------------------------------------------
  // OPERATORs                                                        ( public )
  // ---------------------------------------------------------------------------
  Uri& operator=(const Uri&) = default;
  Uri& operator=(Uri&&) noexcept = default;
  // ---------------------------------------------------------------------------
  // Methods                                                          ( public )
  // ---------------------------------------------------------------------------
  // Returns the stored 'scheme' part.
  std::string GetScheme() const override { return scheme_; }
  // Returns the stored 'scheme-specific' part.
  std::string GetSchemeSpecificPart() const override { return specific_; }
  // Returns the stored 'authority' part.
  std::string GetAuthority() const override { return authority_; }
  // Returns the stored 'path' part.
  std::string GetPath() const override { return path_; }
  // Returns the stored 'opaque' part.
  std::string GetOpaque() const override { return opaque_; }
  // Returns the stored 'query' part.
  UriQuery GetQuery() const override { return query_; }

 private:
  // ---------------------------------------------------------------------------
  // METHODs                                                         ( private )
  // ---------------------------------------------------------------------------
  // Used to parse the full content of the provided string.
  void parseFull(const char* str, const std::size_t length) {
    parseScheme(str, length);
    parseSchemeSpecificPart();
  }
  // Used to parse the schema part.
  void parseScheme(const char* str, const std::size_t length) {
    std::size_t i = 0, end = length;
    while (i < end) {
      auto const& ch = str[i++];
      if (ch == ':') {
        scheme_.assign(&str[0], i - 1);
        break;
      }
    }
    if (i != end) {
      specific_.assign(&str[i], end - i);
    } else {
      specific_.assign(&str[0], end);
    }
  }
  // Used to parse the schema-specific part.
  void parseSchemeSpecificPart() {
    std::size_t i = 0;
    auto len = specific_.length();
    if (len >= 2 && specific_[0] == '/' && specific_[1] == '/') {
      // net_path = "//" authority [abs_path]
      parseAuthority(i);
      parsePath(i);
    } else if (len >= 1 && specific_[0] == '/') {
      // ok!, it's a <abs-path> candidate!
      parsePath(i);
      parseQuery(i);
    } else {
      // ok!, it's an <opaque-part> candidate!
      parseOpaque();
    }
  }
  // Used to parse the authority part.
  void parseAuthority(std::size_t& i) {
    std::size_t off = i, start = i, end = specific_.length();
    while (off < end) {
      auto const& ch = specific_[off];
      if (ch == '/' || ch == '?') break;
      off++;
    }
    i = off;
    authority_.assign(specific_.substr(start, i - start));
  }
  // Used to parse the path part.
  void parsePath(std::size_t& i) {
    std::size_t start = i, end = specific_.length();
    while (i < end) {
      auto const& ch = specific_[i];
      if (!isUnreserved(ch) && !isEscaped(ch) && ch != ':' && ch != '@' &&
          ch != '&' && ch != '=' && ch != '+' && ch != '$' && ch != ',' &&
          ch != '/')
        break;
      i++;
    }
    path_.assign(specific_.substr(start, i - start));
  }
  // Used to parse the query part.
  void parseQuery(std::size_t& i) {
    std::size_t start = i, end = specific_.length();
    while (i < end) {
      auto const& ch = specific_[i++];
      if (!isUric(ch)) {
        // ((Error)) -> unexpected character found!
        // ((To-Do)) -> inform user back?
      }
    }
    query_.Set(specific_.substr(start, i - start));
  }
  // Used to parse the opaque part.
  void parseOpaque() {
    std::size_t start = 0, off = start, end = specific_.length();
    if (!isUricNoSlash(specific_[off++])) {
      // ((Error)) -> unexpected character found!
      // ((To-Do)) -> inform user back?
    }
    while (off < end) {
      auto const& ch = specific_[off++];
      if (!isUric(ch)) {
        // ((Error)) -> unexpected character found!
        // ((To-Do)) -> inform user back?
      }
    }
    opaque_.assign(specific_.substr(start, off - start));
  }
  // Returns true if the provided character is alphanumeric.
  inline bool isAlpha(const char& ch) const {
    return (ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122);
  }
  // Returns true if the provided character is a digit.
  inline bool isDigit(const char& ch) const { return ch >= 48 && ch <= 57; }
  // Returns true if the provided character is a reserved one.
  inline bool isReserved(const char& ch) const {
    return ch == ';' || ch == '/' || ch == '?' || ch == ':' || ch == '@' ||
           ch == '&' || ch == '=' || ch == '+' || ch == '$' || ch == ',';
  }
  // Returns true if the provided character is an unreserved one.
  inline bool isUnreserved(const char& ch) const {
    return isAlpha(ch) || isDigit(ch) || ch == '-' || ch == '_' || ch == '.' ||
           ch == '!' || ch == '~' || ch == '*' || ch == '\'' || ch == '(' ||
           ch == ')';
  }
  // Returns true if the provided character is hexadecimal.
  inline bool isHex(const char& ch) const {
    return isDigit(ch) || (ch >= 65 && ch <= 70) || (ch >= 97 && ch <= 102);
  }
  // Returns true if the provided character is an escaped one.
  inline bool isEscaped(const char& ch) const { return ch == '%' || isHex(ch); }
  // Returns true if the provided character is an uric.
  inline bool isUric(const char& ch) {
    return (isReserved(ch) || isUnreserved(ch) || isEscaped(ch));
  }
  // Returns true if the provided character is an uric-no-slash.
  inline bool isUricNoSlash(const char& ch) const {
    return (isReserved(ch) || isUnreserved(ch) || isEscaped(ch));
  }
  // ---------------------------------------------------------------------------
  // ATTRIBUTEs                                                      ( private )
  // ---------------------------------------------------------------------------
  std::string scheme_;
  std::string specific_;
  std::string authority_;
  std::string path_;
  std::string opaque_;
  UriQuery query_;
};
}  // namespace koobika::hook::base

#endif