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
#include "headers_helper.h"

namespace koobika::hook::network::protocol::http {
//! @brief Http response class
class Response {
 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  Response() = default;
  Response(const Response&) = default;
  Response(Response&&) noexcept = default;
  ~Response() = default;
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  //
  Response& operator=(const Response&) = default;
  Response& operator=(Response&&) noexcept = default;
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>100 - Continue</em>. User can override the default provided
  //! reason-phrase by passing an specific string (via optional parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void Continue_100(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k100, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>101 - Switching Protocols</em>. User can override the default provided
  //! reason-phrase by passing an specific string (via optional parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void SwitchingProtocols_101(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k101, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>200 - Ok</em>. User can override the default provided
  //! reason-phrase by passing an specific string (via optional parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void Ok_200(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k200, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>201 - Created</em>. User can override the default provided
  //! reason-phrase by passing an specific string (via optional parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void Created_201(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k201, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>202 - Accepted</em>. User can override the default provided
  //! reason-phrase by passing an specific string (via optional parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void Accepted_202(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k202, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>203 - Non Authoritative Information</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void NonAuthoritativeInformation_203(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k203, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>204 - No Content</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void NoContent_204(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k204, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>205 - Reset Content</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void ResetContent_205(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k205, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>206 - Partial Content</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void PartialContent_206(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k206, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>300 - Multiple Choices</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void MultipleChoices_300(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k300, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>301 - Moved Permanently</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void MovedPermanently_301(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k301, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>302 - Found</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void Found_302(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k302, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>303 - See Other</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void SeeOther_303(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k303, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>304 - Not Modified</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void NotModified_304(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k304, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>305 - Use Proxy</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void UseProxy_305(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k305, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>307 - Temporary Redirect</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void TemporaryRedirect_307(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k307, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>400 - Bad Request</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void BadRequest_400(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k400, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>401 - Unauthorized</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void Unauthorized_401(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k401, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>402 - Payment Required</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void PaymentRequired_402(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k402, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>403 - Forbidden</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void Forbidden_403(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k403, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>404 - Not Found</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void NotFound_404(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k404, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>405 - Method Not Allowed</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void MethodNotAllowed_405(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k405, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>406 - Not Acceptable</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void NotAcceptable_406(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k406, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>407 - Proxy Authentication Required</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void ProxyAuthenticationRequired_407(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k407, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>408 - Request Timeout</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void RequestTimeout_408(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k408, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>409 - Conflict</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void Conflict_409(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k409, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>410 - Gone</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void Gone_410(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k410, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>411 - Length Required</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void LengthRequired_411(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k411, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>412 - Precondition Failed</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void PreconditionFailed_412(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k412, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>413 - Request Entity Too Large</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void RequestEntityTooLarge_413(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k413, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>414 - Request Uri Too Large</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void RequestUriTooLarge_414(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k414, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>415 - Unsupported Media Type</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void UnsupportedMediaType_415(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k415, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>416 - Requested Range Not Satisfiable</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void RequestedRangeNotSatisfiable_416(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k416, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>417 - Expectation Failed</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void ExpectationFailed_417(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k417, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>500 - InternalServerError</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void InternalServerError_500(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k500, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>501 - Not Implemented</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void NotImplemented_501(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k501, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>502 - Bad Gateway</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void BadGateway_502(const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k502, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>503 - Service Unavailable</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void ServiceUnavailable_503(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k503, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>504 - Gateway Timeout</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void GatewayTimeout_504(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k504, reason_phrase);
  }
  //! @brief Sets current response status-code and reason-phrase to
  //! <em>505 - Http Version Not Supported</em>. User can override the
  //! default provided reason-phrase by passing an specific string (via optional
  //! parameter)
  //! @param[in] reason_phrase Optional user-specific reason phrase
  void HttpVersionNotSupported_505(
      const std::optional<std::string>& reason_phrase = {}) {
    setStatusCodeAndReasonPhrase(constants::StatusCodes::k505, reason_phrase);
  }
  // ___________________________________________________________________________
  // PROPERTIEs                                                       ( public )
  //
  //! @brief HTTP status-code associated within the response
  unsigned short StatusCode = constants::StatusCodes::k501;
  //! @brief HTTP reason-phrase associated within the response
  std::string ReasonPhrase = constants::StatusCodes::k501Str;
  //! @brief HTTP headers associated within the response
  HeadersHelper Headers;
  //! @brief HTTP body associated within the response
  base::AutoBuffer Body;
  //! @brief HTTP <b>RAW</b> response data. Writing to this field will bypass
  //! <em>StatusCode</em>, <em>ReasonPhrase</em>, <em>Headers</em> and
  //! <em>Body</em> properties while sending the response data. It should only be
  //! used in case of writing customized response data (bypassing default
  //! provided data preparation mechanism). In other words, write to the
  //! <em>Raw</em> field if you want to deeply control the data being sent to
  //! the client (e.g.: custom writers). It is <b>strongly</b> recommended to
  //! use default provided mechanisms (through <em>StatusCode</em>,
  //! <em>ReasonPhrase</em>, <em>Headers</em> and <em>Body</em> fields).
  //! @section Example
  //! @snippet network/http_server_raw_response.cpp Example
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