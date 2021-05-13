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
// network/protocol/http/constants/status_codes.h
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

#ifndef koobika_hook_network_protocol_http_constants_statuscodes_h
#define koobika_hook_network_protocol_http_constants_statuscodes_h

namespace koobika::hook::network::protocol::http::constants {
// =============================================================================
// StatusCodes                                                         ( class )
// -----------------------------------------------------------------------------
// This specification holds for all <status-codes> constants.
// =============================================================================
class StatusCodes {
 public:
  // ___________________________________________________________________________
  // CONSTANTs                                                        ( public )
  //
  // Numeric values.
  static constexpr unsigned short k100 = 100;
  static constexpr unsigned short k101 = 101;
  static constexpr unsigned short k200 = 200;
  static constexpr unsigned short k201 = 201;
  static constexpr unsigned short k202 = 202;
  static constexpr unsigned short k203 = 203;
  static constexpr unsigned short k204 = 204;
  static constexpr unsigned short k205 = 205;
  static constexpr unsigned short k206 = 206;
  static constexpr unsigned short k300 = 300;
  static constexpr unsigned short k301 = 301;
  static constexpr unsigned short k302 = 302;
  static constexpr unsigned short k303 = 303;
  static constexpr unsigned short k304 = 304;
  static constexpr unsigned short k305 = 305;
  static constexpr unsigned short k307 = 307;
  static constexpr unsigned short k400 = 400;
  static constexpr unsigned short k401 = 401;
  static constexpr unsigned short k402 = 402;
  static constexpr unsigned short k403 = 403;
  static constexpr unsigned short k404 = 404;
  static constexpr unsigned short k405 = 405;
  static constexpr unsigned short k406 = 406;
  static constexpr unsigned short k407 = 407;
  static constexpr unsigned short k408 = 408;
  static constexpr unsigned short k409 = 409;
  static constexpr unsigned short k410 = 410;
  static constexpr unsigned short k411 = 411;
  static constexpr unsigned short k412 = 412;
  static constexpr unsigned short k413 = 413;
  static constexpr unsigned short k414 = 414;
  static constexpr unsigned short k415 = 415;
  static constexpr unsigned short k416 = 416;
  static constexpr unsigned short k417 = 417;
  static constexpr unsigned short k500 = 500;
  static constexpr unsigned short k501 = 501;
  static constexpr unsigned short k502 = 502;
  static constexpr unsigned short k503 = 503;
  static constexpr unsigned short k504 = 504;
  static constexpr unsigned short k505 = 505;
  // String values.
  static constexpr char k100Str[] = "Continue";
  static constexpr char k101Str[] = "Switching protocols";
  static constexpr char k200Str[] = "Ok";
  static constexpr char k201Str[] = "Created";
  static constexpr char k202Str[] = "Accepted";
  static constexpr char k203Str[] = "Non-authoritative information";
  static constexpr char k204Str[] = "No content";
  static constexpr char k205Str[] = "Reset content";
  static constexpr char k206Str[] = "Partial content";
  static constexpr char k300Str[] = "Multiple choices";
  static constexpr char k301Str[] = "Move permanently";
  static constexpr char k302Str[] = "Found";
  static constexpr char k303Str[] = "See others";
  static constexpr char k304Str[] = "Not modified";
  static constexpr char k305Str[] = "Use proxy";
  static constexpr char k307Str[] = "Temporary redirect";
  static constexpr char k400Str[] = "Bad request";
  static constexpr char k401Str[] = "Unauthorized";
  static constexpr char k402Str[] = "Payment required";
  static constexpr char k403Str[] = "Forbidden";
  static constexpr char k404Str[] = "Not found";
  static constexpr char k405Str[] = "Method not allowed";
  static constexpr char k406Str[] = "Not acceptable";
  static constexpr char k407Str[] = "Proxy Authentication required";
  static constexpr char k408Str[] = "Request time-out";
  static constexpr char k409Str[] = "Conflict";
  static constexpr char k410Str[] = "Gone";
  static constexpr char k411Str[] = "Length required";
  static constexpr char k412Str[] = "Precondition failed";
  static constexpr char k413Str[] = "Request entity too large";
  static constexpr char k414Str[] = "Request Uri too large";
  static constexpr char k415Str[] = "Unsupported media type";
  static constexpr char k416Str[] = "Requested range not satisfiable";
  static constexpr char k417Str[] = "Expectation failed";
  static constexpr char k500Str[] = "Internal server error";
  static constexpr char k501Str[] = "Not implemented";
  static constexpr char k502Str[] = "Bad gateway";
  static constexpr char k503Str[] = "Service unavailable";
  static constexpr char k504Str[] = "Gateway time-out";
  static constexpr char k505Str[] = "Http version not supported";
};
}  // namespace koobika::hook::network::protocol::http::constants

#endif