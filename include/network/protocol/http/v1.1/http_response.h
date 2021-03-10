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

#ifndef koobika_hook_network_protocol_http_v11_httpresponse_h
#define koobika_hook_network_protocol_http_v11_httpresponse_h

#include <optional>
#include <string>
#include <unordered_map>

#include "base/serializable.h"
#include "base/stream.h"
#include "http_constants.h"
#include "http_headers.h"

namespace koobika::hook::network::protocol::http::v11 {
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
  HttpResponse() : StatusCode{k000_}, ReasonPhrase{kUnkStr_} {}
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
    setStatusCodeAndReasonPhrase(k100_, reason_phrase);
  }
  // 101 - Switching Protocols.
  void SwitchingProtocols_101(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k101_, reason_phrase);
  }
  // 200 - Ok.
  void Ok_200(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k200_, reason_phrase);
  }
  // 201 - Created.
  void Created_201(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k201_, reason_phrase);
  }
  // 202 - Accepted.
  void Accepted_202(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k202_, reason_phrase);
  }
  // 203 - Non Authoritative Information.
  void NonAuthoritativeInformation_203(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k203_, reason_phrase);
  }
  // 204 - No Content.
  void NoContent_204(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k204_, reason_phrase);
  }
  // 205 - Reset Content.
  void ResetContent_205(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k205_, reason_phrase);
  }
  // 206 - Partial Content.
  void PartialContent_206(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k206_, reason_phrase);
  }
  // 300 - Multiple Choices.
  void MultipleChoices_300(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k300_, reason_phrase);
  }
  // 301 - Moved Permanently.
  void MovedPermanently_301(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k301_, reason_phrase);
  }
  // 302 - Found.
  void Found_302(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k302_, reason_phrase);
  }
  // 303 - See Other.
  void SeeOther_303(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k303_, reason_phrase);
  }
  // 304 - Not Modified.
  void NotModified_304(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k304_, reason_phrase);
  }
  // 305 - Use Proxy.
  void UseProxy_305(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k305_, reason_phrase);
  }
  // 307 - Temporary Redirect.
  void TemporaryRedirect_307(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k307_, reason_phrase);
  }
  // 400 - Bad Request.
  void BadRequest_400(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k400_, reason_phrase);
  }
  // 401 - Unauthorized.
  void Unauthorized_401(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k401_, reason_phrase);
  }
  // 402 - Payment Required.
  void PaymentRequired_402(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k402_, reason_phrase);
  }
  // 403 - Forbidden.
  void Forbidden_403(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k403_, reason_phrase);
  }
  // 404 - Not Found.
  void NotFound_404(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k404_, reason_phrase);
  }
  // 405 - Method Not Allowed.
  void MethodNotAllowed_405(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k405_, reason_phrase);
  }
  // 406 - Not Acceptable.
  void NotAcceptable_406(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k406_, reason_phrase);
  }
  // 407 - Proxy Authentication Required.
  void ProxyAuthenticationRequired_407(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k407_, reason_phrase);
  }
  // 408 - Request Timeout.
  void RequestTimeout_408(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k408_, reason_phrase);
  }
  // 409 - Conflict.
  void Conflict_409(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k409_, reason_phrase);
  }
  // 410 - Gone.
  void Gone_410(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k410_, reason_phrase);
  }
  // 411 - Length Required.
  void LengthRequired_411(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k411_, reason_phrase);
  }
  // 412 - Precondition Failed.
  void PreconditionFailed_412(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k412_, reason_phrase);
  }
  // 413 - Request Entity Too Large.
  void RequestEntityTooLarge_413(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k413_, reason_phrase);
  }
  // 414 - Request Uri Too Large.
  void RequestUriTooLarge_414(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k414_, reason_phrase);
  }
  // 415 - Unsupported Media Type.
  void UnsupportedMediaType_415(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k415_, reason_phrase);
  }
  // 416 - Requested Range Not Satisfiable.
  void RequestedRangeNotSatisfiable_416(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k416_, reason_phrase);
  }
  // 417 - Expectation Failed.
  void ExpectationFailed_417(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k417_, reason_phrase);
  }
  // 500 - InternalServerError.
  void InternalServerError_500(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k500_, reason_phrase);
  }
  // 501 - Not Implemented.
  void NotImplemented_501(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k501_, reason_phrase);
  }
  // 502 - Bad Gateway.
  void BadGateway_502(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k502_, reason_phrase);
  }
  // 503 - Service Unavailable.
  void ServiceUnavailable_503(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k503_, reason_phrase);
  }
  // 504 - Gateway Timeout.
  void GatewayTimeout_504(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k504_, reason_phrase);
  }
  // 505 - Http Version Not Supported.
  void HttpVersionNotSupported_505(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(k505_, reason_phrase);
  }
  // Renders current content to string.
  base::Stream Serialize() const override {
    base::Stream stream;
    HttpHeaders headers{Headers};
    auto body_length = Body.Length();
    // [status-line] setup
    stream.Write(HttpConstants::Strings::kHttpVersion)
        .Write(HttpConstants::Strings::kSpace)
        .Write(std::to_string(StatusCode))
        .Write(HttpConstants::Strings::kSpace)
        .Write(ReasonPhrase)
        .Write(HttpConstants::Strings::kCrLf);
    // [headers] <empty-body-considerations>
    headers.Set(HttpConstants::Headers::kContentLength, body_length);
    // [headers] <setup>
    stream.Write(headers.Serialize()).Write(HttpConstants::Strings::kCrLf);
    // [body] setup
    if (body_length) {
      stream.Write(Body);
    }
    // We're done! Let's return our stream!
    return stream;
  }
  // ---------------------------------------------------------------------------
  // PROPERTIEs                                                       ( public )
  // ---------------------------------------------------------------------------
  unsigned short StatusCode = k000_;
  std::string ReasonPhrase = kUnkStr_;
  HttpHeaders Headers;
  base::Stream Body;

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
        known_reason_phrases = {
            {k100_, k100Str_}, {k101_, k101Str_}, {k200_, k200Str_},
            {k201_, k201Str_}, {k202_, k202Str_}, {k203_, k203Str_},
            {k204_, k204Str_}, {k205_, k205Str_}, {k206_, k206Str_},
            {k300_, k300Str_}, {k301_, k301Str_}, {k302_, k302Str_},
            {k303_, k303Str_}, {k304_, k304Str_}, {k305_, k305Str_},
            {k307_, k307Str_}, {k400_, k400Str_}, {k401_, k401Str_},
            {k402_, k402Str_}, {k403_, k403Str_}, {k404_, k404Str_},
            {k405_, k405Str_}, {k406_, k406Str_}, {k407_, k407Str_},
            {k408_, k408Str_}, {k409_, k409Str_}, {k410_, k410Str_},
            {k411_, k411Str_}, {k412_, k412Str_}, {k413_, k413Str_},
            {k414_, k414Str_}, {k415_, k415Str_}, {k416_, k416Str_},
            {k417_, k417Str_}};
    auto itr = known_reason_phrases.find(code);
    return itr != known_reason_phrases.end() ? itr->second : kUnkStr_;
  }
  // ---------------------------------------------------------------------------
  // CONSTANTs                                                       ( private )
  // ---------------------------------------------------------------------------
  // Default status-codes.
  static constexpr unsigned short k000_ = 000;
  static constexpr unsigned short k100_ = 100;
  static constexpr unsigned short k101_ = 101;
  static constexpr unsigned short k200_ = 200;
  static constexpr unsigned short k201_ = 201;
  static constexpr unsigned short k202_ = 202;
  static constexpr unsigned short k203_ = 203;
  static constexpr unsigned short k204_ = 204;
  static constexpr unsigned short k205_ = 205;
  static constexpr unsigned short k206_ = 206;
  static constexpr unsigned short k300_ = 300;
  static constexpr unsigned short k301_ = 301;
  static constexpr unsigned short k302_ = 302;
  static constexpr unsigned short k303_ = 303;
  static constexpr unsigned short k304_ = 304;
  static constexpr unsigned short k305_ = 305;
  static constexpr unsigned short k307_ = 307;
  static constexpr unsigned short k400_ = 400;
  static constexpr unsigned short k401_ = 401;
  static constexpr unsigned short k402_ = 402;
  static constexpr unsigned short k403_ = 403;
  static constexpr unsigned short k404_ = 404;
  static constexpr unsigned short k405_ = 405;
  static constexpr unsigned short k406_ = 406;
  static constexpr unsigned short k407_ = 407;
  static constexpr unsigned short k408_ = 408;
  static constexpr unsigned short k409_ = 409;
  static constexpr unsigned short k410_ = 410;
  static constexpr unsigned short k411_ = 411;
  static constexpr unsigned short k412_ = 412;
  static constexpr unsigned short k413_ = 413;
  static constexpr unsigned short k414_ = 414;
  static constexpr unsigned short k415_ = 415;
  static constexpr unsigned short k416_ = 416;
  static constexpr unsigned short k417_ = 417;
  static constexpr unsigned short k500_ = 500;
  static constexpr unsigned short k501_ = 501;
  static constexpr unsigned short k502_ = 502;
  static constexpr unsigned short k503_ = 503;
  static constexpr unsigned short k504_ = 504;
  static constexpr unsigned short k505_ = 505;
  // Default reason-phrases.
  static constexpr char k100Str_[] = "Continue";
  static constexpr char k101Str_[] = "Switching protocols";
  static constexpr char k200Str_[] = "Ok";
  static constexpr char k201Str_[] = "Created";
  static constexpr char k202Str_[] = "Accepted";
  static constexpr char k203Str_[] = "Non-authoritative information";
  static constexpr char k204Str_[] = "No content";
  static constexpr char k205Str_[] = "Reset content";
  static constexpr char k206Str_[] = "Partial content";
  static constexpr char k300Str_[] = "Multiple choices";
  static constexpr char k301Str_[] = "Move permanently";
  static constexpr char k302Str_[] = "Found";
  static constexpr char k303Str_[] = "See others";
  static constexpr char k304Str_[] = "Not modified";
  static constexpr char k305Str_[] = "Use proxy";
  static constexpr char k307Str_[] = "Temporary redirect";
  static constexpr char k400Str_[] = "Bad request";
  static constexpr char k401Str_[] = "Unauthorized";
  static constexpr char k402Str_[] = "Payment required";
  static constexpr char k403Str_[] = "Forbidden";
  static constexpr char k404Str_[] = "Not found";
  static constexpr char k405Str_[] = "Method not allowed";
  static constexpr char k406Str_[] = "Not acceptable";
  static constexpr char k407Str_[] = "Proxy Authentication required";
  static constexpr char k408Str_[] = "Request time-out";
  static constexpr char k409Str_[] = "Conflict";
  static constexpr char k410Str_[] = "Gone";
  static constexpr char k411Str_[] = "Length required";
  static constexpr char k412Str_[] = "Precondition failed";
  static constexpr char k413Str_[] = "Request entity too large";
  static constexpr char k414Str_[] = "Request Uri too large";
  static constexpr char k415Str_[] = "Unsupported media type";
  static constexpr char k416Str_[] = "Requested range not satisfiable";
  static constexpr char k417Str_[] = "Expectation failed";
  static constexpr char k500Str_[] = "Internal server error";
  static constexpr char k501Str_[] = "Not implemented";
  static constexpr char k502Str_[] = "Bad gateway";
  static constexpr char k503Str_[] = "Service unavailable";
  static constexpr char k504Str_[] = "Gateway time-out";
  static constexpr char k505Str_[] = "Http version not supported";
  static constexpr char kUnkStr_[] = "Unknown";
};
}  // namespace koobika::hook::network::protocol::http::v11

#endif