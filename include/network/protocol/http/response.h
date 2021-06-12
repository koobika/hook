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
// network/protocol/http/response.h
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

#ifndef koobika_hook_network_protocol_http_response_h
#define koobika_hook_network_protocol_http_response_h

#include "base/auto_buffer.h"
#include "constants/headers.h"
#include "constants/status_codes.h"
#include "constants/strings.h"
#include "headers.h"

namespace koobika::hook::network::protocol::http {
// =============================================================================
// Response                                                            ( class )
// -----------------------------------------------------------------------------
// This class is in charge of providing the http response class.
// =============================================================================
class Response {
 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  Response() = default;
  Response(const Response&) = delete;
  Response(Response&&) noexcept = delete;
  ~Response() = default;
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  //
  Response& operator=(const Response&) = delete;
  Response& operator=(Response&&) noexcept = delete;
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  // 100 - Continue.
  void Continue_100(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k100, reason_phrase);
  }
  // 101 - Switching Protocols.
  void SwitchingProtocols_101(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k101, reason_phrase);
  }
  // 200 - Ok.
  void Ok_200(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k200, reason_phrase);
  }
  // 201 - Created.
  void Created_201(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k201, reason_phrase);
  }
  // 202 - Accepted.
  void Accepted_202(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k202, reason_phrase);
  }
  // 203 - Non Authoritative Information.
  void NonAuthoritativeInformation_203(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k203, reason_phrase);
  }
  // 204 - No Content.
  void NoContent_204(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k204, reason_phrase);
  }
  // 205 - Reset Content.
  void ResetContent_205(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k205, reason_phrase);
  }
  // 206 - Partial Content.
  void PartialContent_206(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k206, reason_phrase);
  }
  // 300 - Multiple Choices.
  void MultipleChoices_300(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k300, reason_phrase);
  }
  // 301 - Moved Permanently.
  void MovedPermanently_301(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k301, reason_phrase);
  }
  // 302 - Found.
  void Found_302(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k302, reason_phrase);
  }
  // 303 - See Other.
  void SeeOther_303(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k303, reason_phrase);
  }
  // 304 - Not Modified.
  void NotModified_304(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k304, reason_phrase);
  }
  // 305 - Use Proxy.
  void UseProxy_305(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k305, reason_phrase);
  }
  // 307 - Temporary Redirect.
  void TemporaryRedirect_307(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k307, reason_phrase);
  }
  // 400 - Bad Request.
  void BadRequest_400(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k400, reason_phrase);
  }
  // 401 - Unauthorized.
  void Unauthorized_401(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k401, reason_phrase);
  }
  // 402 - Payment Required.
  void PaymentRequired_402(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k402, reason_phrase);
  }
  // 403 - Forbidden.
  void Forbidden_403(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k403, reason_phrase);
  }
  // 404 - Not Found.
  void NotFound_404(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k404, reason_phrase);
  }
  // 405 - Method Not Allowed.
  void MethodNotAllowed_405(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k405, reason_phrase);
  }
  // 406 - Not Acceptable.
  void NotAcceptable_406(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k406, reason_phrase);
  }
  // 407 - Proxy Authentication Required.
  void ProxyAuthenticationRequired_407(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k407, reason_phrase);
  }
  // 408 - Request Timeout.
  void RequestTimeout_408(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k408, reason_phrase);
  }
  // 409 - Conflict.
  void Conflict_409(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k409, reason_phrase);
  }
  // 410 - Gone.
  void Gone_410(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k410, reason_phrase);
  }
  // 411 - Length Required.
  void LengthRequired_411(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k411, reason_phrase);
  }
  // 412 - Precondition Failed.
  void PreconditionFailed_412(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k412, reason_phrase);
  }
  // 413 - Request Entity Too Large.
  void RequestEntityTooLarge_413(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k413, reason_phrase);
  }
  // 414 - Request Uri Too Large.
  void RequestUriTooLarge_414(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k414, reason_phrase);
  }
  // 415 - Unsupported Media Type.
  void UnsupportedMediaType_415(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k415, reason_phrase);
  }
  // 416 - Requested Range Not Satisfiable.
  void RequestedRangeNotSatisfiable_416(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k416, reason_phrase);
  }
  // 417 - Expectation Failed.
  void ExpectationFailed_417(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k417, reason_phrase);
  }
  // 500 - InternalServerError.
  void InternalServerError_500(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k500, reason_phrase);
  }
  // 501 - Not Implemented.
  void NotImplemented_501(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k501, reason_phrase);
  }
  // 502 - Bad Gateway.
  void BadGateway_502(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k502, reason_phrase);
  }
  // 503 - Service Unavailable.
  void ServiceUnavailable_503(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k503, reason_phrase);
  }
  // 504 - Gateway Timeout.
  void GatewayTimeout_504(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k504, reason_phrase);
  }
  // 505 - Http Version Not Supported.
  void HttpVersionNotSupported_505(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k505, reason_phrase);
  }
  // ___________________________________________________________________________
  // PROPERTIEs                                                       ( public )
  //
  unsigned short StatusCode = constants::StatusCodes::k501;
  std::string ReasonPhrase = constants::StatusCodes::k501Str;
  Headers Headers;
  base::AutoBuffer Body;
  base::AutoBuffer Raw;

 private:
  // ___________________________________________________________________________
  // METHODs                                                         ( private )
  //
  // Sets the status code and reason phrase.
  void setStatusCodeAndReasonPhrase(
      const unsigned short& status_code,
      const std::optional<std::string>& reason_phrase) {
    StatusCode = status_code;
    ReasonPhrase = reason_phrase.has_value()
                       ? reason_phrase.value()
                       : getDefaultReasonPhraseFor(status_code);
  }
  // Gets the default reason phrase for the specified code.
  std::string getDefaultReasonPhraseFor(const unsigned short& code) {
    static const std::unordered_map<unsigned short, std::string>
        known_reason_phrases = {
            {constants::StatusCodes::k100, constants::StatusCodes::k100Str},
            {constants::StatusCodes::k101, constants::StatusCodes::k101Str},
            {constants::StatusCodes::k200, constants::StatusCodes::k200Str},
            {constants::StatusCodes::k201, constants::StatusCodes::k201Str},
            {constants::StatusCodes::k202, constants::StatusCodes::k202Str},
            {constants::StatusCodes::k203, constants::StatusCodes::k203Str},
            {constants::StatusCodes::k204, constants::StatusCodes::k204Str},
            {constants::StatusCodes::k205, constants::StatusCodes::k205Str},
            {constants::StatusCodes::k206, constants::StatusCodes::k206Str},
            {constants::StatusCodes::k300, constants::StatusCodes::k300Str},
            {constants::StatusCodes::k301, constants::StatusCodes::k301Str},
            {constants::StatusCodes::k302, constants::StatusCodes::k302Str},
            {constants::StatusCodes::k303, constants::StatusCodes::k303Str},
            {constants::StatusCodes::k304, constants::StatusCodes::k304Str},
            {constants::StatusCodes::k305, constants::StatusCodes::k305Str},
            {constants::StatusCodes::k307, constants::StatusCodes::k307Str},
            {constants::StatusCodes::k400, constants::StatusCodes::k400Str},
            {constants::StatusCodes::k401, constants::StatusCodes::k401Str},
            {constants::StatusCodes::k402, constants::StatusCodes::k402Str},
            {constants::StatusCodes::k403, constants::StatusCodes::k403Str},
            {constants::StatusCodes::k404, constants::StatusCodes::k404Str},
            {constants::StatusCodes::k405, constants::StatusCodes::k405Str},
            {constants::StatusCodes::k406, constants::StatusCodes::k406Str},
            {constants::StatusCodes::k407, constants::StatusCodes::k407Str},
            {constants::StatusCodes::k408, constants::StatusCodes::k408Str},
            {constants::StatusCodes::k409, constants::StatusCodes::k409Str},
            {constants::StatusCodes::k410, constants::StatusCodes::k410Str},
            {constants::StatusCodes::k411, constants::StatusCodes::k411Str},
            {constants::StatusCodes::k412, constants::StatusCodes::k412Str},
            {constants::StatusCodes::k413, constants::StatusCodes::k413Str},
            {constants::StatusCodes::k414, constants::StatusCodes::k414Str},
            {constants::StatusCodes::k415, constants::StatusCodes::k415Str},
            {constants::StatusCodes::k416, constants::StatusCodes::k416Str},
            {constants::StatusCodes::k417, constants::StatusCodes::k417Str},
            {constants::StatusCodes::k500, constants::StatusCodes::k500Str},
            {constants::StatusCodes::k501, constants::StatusCodes::k501Str},
            {constants::StatusCodes::k502, constants::StatusCodes::k502Str},
            {constants::StatusCodes::k503, constants::StatusCodes::k503Str},
            {constants::StatusCodes::k504, constants::StatusCodes::k504Str},
            {constants::StatusCodes::k505, constants::StatusCodes::k505Str}};
    auto itr = known_reason_phrases.find(code);
    return itr != known_reason_phrases.end() ? itr->second : "";
  }
};
}  // namespace koobika::hook::network::protocol::http

#endif