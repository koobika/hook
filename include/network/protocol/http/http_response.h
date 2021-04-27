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

#ifndef koobika_hook_network_protocol_http_httpresponse_h
#define koobika_hook_network_protocol_http_httpresponse_h

#include <optional>
#include <string>
#include <unordered_map>

#include "base/serializable.h"
#include "base/auto_buffer.h"
#include "http_constants.h"
#include "http_headers.h"

namespace koobika::hook::network::protocol::http {
// =============================================================================
// HttpResponse                                                        ( class )
// -----------------------------------------------------------------------------
// This class is in charge of providing the http response class.
// =============================================================================
class HttpResponse : public base::Serializable {
 public:
  // ---------------------------------------------------------------------------
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  // ---------------------------------------------------------------------------
  HttpResponse()
      : StatusCode{HttpConstants::StatusCodes::k000},
        ReasonPhrase{HttpConstants::StatusCodes::kUnkStr} {}
  HttpResponse(const HttpResponse&) = delete;
  HttpResponse(HttpResponse&&) noexcept = delete;
  ~HttpResponse() = default;
  // ---------------------------------------------------------------------------
  // OPERATORs                                                        ( public )
  // ---------------------------------------------------------------------------
  HttpResponse& operator=(const HttpResponse&) = delete;
  HttpResponse& operator=(HttpResponse&&) noexcept = delete;
  // ---------------------------------------------------------------------------
  // METHODs                                                          ( public )
  // ---------------------------------------------------------------------------
  // 100 - Continue.
  void Continue_100(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k100,
                                 reason_phrase);
  }
  // 101 - Switching Protocols.
  void SwitchingProtocols_101(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k101,
                                 reason_phrase);
  }
  // 200 - Ok.
  void Ok_200(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k200,
                                 reason_phrase);
  }
  // 201 - Created.
  void Created_201(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k201,
                                 reason_phrase);
  }
  // 202 - Accepted.
  void Accepted_202(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k202,
                                 reason_phrase);
  }
  // 203 - Non Authoritative Information.
  void NonAuthoritativeInformation_203(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k203,
                                 reason_phrase);
  }
  // 204 - No Content.
  void NoContent_204(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k204,
                                 reason_phrase);
  }
  // 205 - Reset Content.
  void ResetContent_205(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k205,
                                 reason_phrase);
  }
  // 206 - Partial Content.
  void PartialContent_206(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k206,
                                 reason_phrase);
  }
  // 300 - Multiple Choices.
  void MultipleChoices_300(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k300,
                                 reason_phrase);
  }
  // 301 - Moved Permanently.
  void MovedPermanently_301(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k301,
                                 reason_phrase);
  }
  // 302 - Found.
  void Found_302(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k302,
                                 reason_phrase);
  }
  // 303 - See Other.
  void SeeOther_303(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k303,
                                 reason_phrase);
  }
  // 304 - Not Modified.
  void NotModified_304(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k304,
                                 reason_phrase);
  }
  // 305 - Use Proxy.
  void UseProxy_305(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k305,
                                 reason_phrase);
  }
  // 307 - Temporary Redirect.
  void TemporaryRedirect_307(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k307,
                                 reason_phrase);
  }
  // 400 - Bad Request.
  void BadRequest_400(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k400,
                                 reason_phrase);
  }
  // 401 - Unauthorized.
  void Unauthorized_401(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k401,
                                 reason_phrase);
  }
  // 402 - Payment Required.
  void PaymentRequired_402(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k402,
                                 reason_phrase);
  }
  // 403 - Forbidden.
  void Forbidden_403(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k403,
                                 reason_phrase);
  }
  // 404 - Not Found.
  void NotFound_404(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k404,
                                 reason_phrase);
  }
  // 405 - Method Not Allowed.
  void MethodNotAllowed_405(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k405,
                                 reason_phrase);
  }
  // 406 - Not Acceptable.
  void NotAcceptable_406(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k406,
                                 reason_phrase);
  }
  // 407 - Proxy Authentication Required.
  void ProxyAuthenticationRequired_407(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k407,
                                 reason_phrase);
  }
  // 408 - Request Timeout.
  void RequestTimeout_408(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k408,
                                 reason_phrase);
  }
  // 409 - Conflict.
  void Conflict_409(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k409,
                                 reason_phrase);
  }
  // 410 - Gone.
  void Gone_410(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k410,
                                 reason_phrase);
  }
  // 411 - Length Required.
  void LengthRequired_411(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k411,
                                 reason_phrase);
  }
  // 412 - Precondition Failed.
  void PreconditionFailed_412(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k412,
                                 reason_phrase);
  }
  // 413 - Request Entity Too Large.
  void RequestEntityTooLarge_413(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k413,
                                 reason_phrase);
  }
  // 414 - Request Uri Too Large.
  void RequestUriTooLarge_414(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k414,
                                 reason_phrase);
  }
  // 415 - Unsupported Media Type.
  void UnsupportedMediaType_415(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k415,
                                 reason_phrase);
  }
  // 416 - Requested Range Not Satisfiable.
  void RequestedRangeNotSatisfiable_416(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k416,
                                 reason_phrase);
  }
  // 417 - Expectation Failed.
  void ExpectationFailed_417(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k417,
                                 reason_phrase);
  }
  // 500 - InternalServerError.
  void InternalServerError_500(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k500,
                                 reason_phrase);
  }
  // 501 - Not Implemented.
  void NotImplemented_501(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k501,
                                 reason_phrase);
  }
  // 502 - Bad Gateway.
  void BadGateway_502(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k502,
                                 reason_phrase);
  }
  // 503 - Service Unavailable.
  void ServiceUnavailable_503(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k503,
                                 reason_phrase);
  }
  // 504 - Gateway Timeout.
  void GatewayTimeout_504(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k504,
                                 reason_phrase);
  }
  // 505 - Http Version Not Supported.
  void HttpVersionNotSupported_505(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(HttpConstants::StatusCodes::k505,
                                 reason_phrase);
  }
  // Renders current content to string.
  base::AutoBuffer Serialize() const override {
    base::AutoBuffer buffer;
    HttpHeaders headers{Headers};
    auto body_length = Body.Length();
    // [status-line] setup
    buffer.Write(HttpConstants::Strings::kHttpVersion)
        .Write(HttpConstants::Strings::kSpace)
        .Write(std::to_string(StatusCode))
        .Write(HttpConstants::Strings::kSpace)
        .Write(ReasonPhrase)
        .Write(HttpConstants::Strings::kCrLf);
    // [headers] <empty-body-considerations>
    headers.Set(HttpConstants::Headers::kContentLength, body_length);
    // [headers] <setup>
    buffer.Write(headers.Serialize()).Write(HttpConstants::Strings::kCrLf);
    // [body] setup
    if (body_length) {
      buffer.Write(Body);
    }
    // We're done! Let's return our stream!
    return buffer;
  }
  // ---------------------------------------------------------------------------
  // PROPERTIEs                                                       ( public )
  // ---------------------------------------------------------------------------
  unsigned short StatusCode = HttpConstants::StatusCodes::k000;
  std::string ReasonPhrase = HttpConstants::StatusCodes::kUnkStr;
  HttpHeaders Headers;
  base::AutoBuffer Body;

 private:
  // ---------------------------------------------------------------------------
  // METHODs                                                         ( private )
  // ---------------------------------------------------------------------------
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
        known_reason_phrases = {{HttpConstants::StatusCodes::k100,
                                 HttpConstants::StatusCodes::k100Str},
                                {HttpConstants::StatusCodes::k101,
                                 HttpConstants::StatusCodes::k101Str},
                                {HttpConstants::StatusCodes::k200,
                                 HttpConstants::StatusCodes::k200Str},
                                {HttpConstants::StatusCodes::k201,
                                 HttpConstants::StatusCodes::k201Str},
                                {HttpConstants::StatusCodes::k202,
                                 HttpConstants::StatusCodes::k202Str},
                                {HttpConstants::StatusCodes::k203,
                                 HttpConstants::StatusCodes::k203Str},
                                {HttpConstants::StatusCodes::k204,
                                 HttpConstants::StatusCodes::k204Str},
                                {HttpConstants::StatusCodes::k205,
                                 HttpConstants::StatusCodes::k205Str},
                                {HttpConstants::StatusCodes::k206,
                                 HttpConstants::StatusCodes::k206Str},
                                {HttpConstants::StatusCodes::k300,
                                 HttpConstants::StatusCodes::k300Str},
                                {HttpConstants::StatusCodes::k301,
                                 HttpConstants::StatusCodes::k301Str},
                                {HttpConstants::StatusCodes::k302,
                                 HttpConstants::StatusCodes::k302Str},
                                {HttpConstants::StatusCodes::k303,
                                 HttpConstants::StatusCodes::k303Str},
                                {HttpConstants::StatusCodes::k304,
                                 HttpConstants::StatusCodes::k304Str},
                                {HttpConstants::StatusCodes::k305,
                                 HttpConstants::StatusCodes::k305Str},
                                {HttpConstants::StatusCodes::k307,
                                 HttpConstants::StatusCodes::k307Str},
                                {HttpConstants::StatusCodes::k400,
                                 HttpConstants::StatusCodes::k400Str},
                                {HttpConstants::StatusCodes::k401,
                                 HttpConstants::StatusCodes::k401Str},
                                {HttpConstants::StatusCodes::k402,
                                 HttpConstants::StatusCodes::k402Str},
                                {HttpConstants::StatusCodes::k403,
                                 HttpConstants::StatusCodes::k403Str},
                                {HttpConstants::StatusCodes::k404,
                                 HttpConstants::StatusCodes::k404Str},
                                {HttpConstants::StatusCodes::k405,
                                 HttpConstants::StatusCodes::k405Str},
                                {HttpConstants::StatusCodes::k406,
                                 HttpConstants::StatusCodes::k406Str},
                                {HttpConstants::StatusCodes::k407,
                                 HttpConstants::StatusCodes::k407Str},
                                {HttpConstants::StatusCodes::k408,
                                 HttpConstants::StatusCodes::k408Str},
                                {HttpConstants::StatusCodes::k409,
                                 HttpConstants::StatusCodes::k409Str},
                                {HttpConstants::StatusCodes::k410,
                                 HttpConstants::StatusCodes::k410Str},
                                {HttpConstants::StatusCodes::k411,
                                 HttpConstants::StatusCodes::k411Str},
                                {HttpConstants::StatusCodes::k412,
                                 HttpConstants::StatusCodes::k412Str},
                                {HttpConstants::StatusCodes::k413,
                                 HttpConstants::StatusCodes::k413Str},
                                {HttpConstants::StatusCodes::k414,
                                 HttpConstants::StatusCodes::k414Str},
                                {HttpConstants::StatusCodes::k415,
                                 HttpConstants::StatusCodes::k415Str},
                                {HttpConstants::StatusCodes::k416,
                                 HttpConstants::StatusCodes::k416Str},
                                {HttpConstants::StatusCodes::k417,
                                 HttpConstants::StatusCodes::k417Str},
                                {HttpConstants::StatusCodes::k500,
                                 HttpConstants::StatusCodes::k500Str},
                                {HttpConstants::StatusCodes::k501,
                                 HttpConstants::StatusCodes::k501Str},
                                {HttpConstants::StatusCodes::k502,
                                 HttpConstants::StatusCodes::k502Str},
                                {HttpConstants::StatusCodes::k503,
                                 HttpConstants::StatusCodes::k503Str},
                                {HttpConstants::StatusCodes::k504,
                                 HttpConstants::StatusCodes::k504Str},
                                {HttpConstants::StatusCodes::k505,
                                 HttpConstants::StatusCodes::k505Str}};
    auto itr = known_reason_phrases.find(code);
    return itr != known_reason_phrases.end()
               ? itr->second
               : HttpConstants::StatusCodes::kUnkStr;
  }
};
}  // namespace koobika::hook::network::protocol::http

#endif