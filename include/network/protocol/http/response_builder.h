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
// network/protocol/http/response_builder.h
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

#ifndef koobika_hook_network_protocol_http_responsebuilder_h
#define koobika_hook_network_protocol_http_responsebuilder_h

#include <optional>

#include "base/auto_buffer.h"
#include "constants/headers.h"
#include "constants/mime.h"
#include "structured/json/value.h"
#include "request.h"
#include "response_writers/default.h"
#include "response_writers/transfer_encoding.h"

namespace koobika::hook::network::protocol::http {
//! @brief Http response builder (helper) class
class ResponseBuilder {
 public:
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  //! @brief Builds the specified response object with the content provided by
  //! either the <em>buffer</em> and the <em>content_type</em>. It uses the
  //! response_writers::Default writer to perform the actions
  //! @tparam PAty The (variadic) list of arguments needed by the response
  //! writer (the default one, in this case) constructor
  //! @param[out] res The response object to fill-up
  //! @param[in] buffer The Autobuffer containing the body data to write
  //! @param[in] content_type The corresponding content type
  //! @param[in] response_writer_parameters The list of parameters needed by the
  //! response writer (the default one, in this case) constructor
  //! @returns Response& A reference to the response object being modified
  //! @see constants::Mime response_writers::Default
  //! @section Example
  //! @snippet network/http_server_response_builder.cpp Example
  template <typename... PAty>
  static Response& Build(Response& res, const base::AutoBuffer& buffer,
                         const std::string& content_type,
                         const PAty&... response_writer_parameters) {
    response_writers::Default(response_writer_parameters...).Write(res, buffer);
    res.Headers.Set(constants::Headers::kContentType, content_type);
    return res;
  }
  //! @brief Builds the specified response object with the content provided by
  //! either the <em>buffer</em> and the <em>content_type</em>. It constructs
  //! the response_writer object using the <em>WRty</em> provided type to
  //! perform the actions
  //! @tparam WRty The response writer type to use
  //! @tparam PAty The (variadic) list of arguments needed by the response
  //! writer (WRty, in this case) constructor
  //! @param[out] res The response object to fill-up
  //! @param[in] buffer The Autobuffer containing the body data to write
  //! @param[in] content_type The corresponding content type
  //! @param[in] response_writer_parameters The list of parameters needed by the
  //! response writer (WRty, in this case) constructor
  //! @returns Response& A reference to the response object being modified
  //! @see constants::Mime response_writers::Default
  //! response_writers::TransferEncoding
  //! @section Example
  //! @snippet network/http_server_response_builder_writer_class.cpp Example
  template <typename WRty, typename... PAty>
  static Response& Build(Response& res, const base::AutoBuffer& buffer,
                         const std::string& content_type,
                         const PAty&... response_writer_parameters) {
    WRty(response_writer_parameters...).Write(res, buffer);
    res.Headers.Set(constants::Headers::kContentType, content_type);
    return res;
  }
  //! @brief Builds the specified response object with the content provided by
  //! either the <em>buffer</em> and the <em>content_type</em>. It uses
  //! the provided response_writer object to perform the actions
  //! @tparam WRty The response writer type to use
  //! @param[out] res The response object to fill-up
  //! @param[in] buffer The Autobuffer containing the body data to write
  //! @param[in] writer The response writer object
  //! @param[in] content_type The corresponding content type
  //! @returns Response& A reference to the response object being modified
  //! @see constants::Mime response_writers::Default
  //! response_writers::TransferEncoding
  //! @section Example
  //! @snippet network/http_server_response_builder_writer_object.cpp Example
  template <typename WRty>
  static Response& Build(Response& res, const base::AutoBuffer& buffer,
                         const WRty& writer, const std::string& content_type) {
    writer.Write(res, buffer);
    res.Headers.Set(constants::Headers::kContentType, content_type);
    return res;
  }
  //! @brief Builds the specified response object with the content provided by
  //! either the <em>serializable</em> and the <em>content_type</em>. It uses
  //! the response_writers::Default writer to perform the actions
  //! @tparam PAty The (variadic) list of arguments needed by the response
  //! writer (the default one, in this case) constructor
  //! @param[out] res The response object to fill-up
  //! @param[in] serializable An object implementing the Serializable interface
  //! and containing the body data to write
  //! @param[in] content_type The corresponding content type
  //! @param[in] response_writer_parameters The list of parameters needed by the
  //! response writer (the default one, in this case) constructor
  //! @returns Response& A reference to the response object being modified
  //! @see constants::Mime response_writers::Default
  //! @section Example
  //! @snippet network/http_server_response_builder_serializable.cpp Example
  template <typename... PAty>
  static Response& Build(Response& res, const base::Serializable& serializable,
                         const std::string& content_type,
                         const PAty&... response_writer_parameters) {
    return Build(res, serializable.Serialize(), content_type,
                 response_writer_parameters...);
  }
  //! @brief Builds the specified response object with the content provided by
  //! either the <em>serializable</em> and the <em>content_type</em>. It
  //! constructs the response_writer object using the <em>WRty</em> provided
  //! type to perform the actions
  //! @tparam WRty The response writer type to use
  //! @tparam PAty The (variadic) list of arguments needed by the response
  //! writer constructor
  //! @param[out] res The response object to fill-up
  //! @param[in] serializable An object implementing the Serializable interface
  //! and containing the body data to write
  //! @param[in] content_type The corresponding content type
  //! @param[in] response_writer_parameters The list of parameters needed by the
  //! response writer constructor
  //! @returns Response& A reference to the response object being modified
  //! @see constants::Mime response_writers::Default
  //! response_writers::TransferEncoding
  //! @section Example
  //! @snippet network/http_server_response_builder_serializable_class.cpp Ex
  template <typename WRty, typename... PAty>
  static Response& Build(Response& res, const base::Serializable& serializable,
                         const std::string& content_type,
                         const PAty&... response_writer_parameters) {
    return Build<WRty>(res, serializable.Serialize(), content_type,
                       response_writer_parameters...);
  }
  //! @brief Builds the specified response object with the content provided by
  //! either the <em>serializable</em> and the <em>content_type</em>. It uses
  //! the provided response_writer object to perform the actions
  //! @tparam WRty The response writer type to use
  //! @param[out] res The response object to fill-up
  //! @param[in] serializable An object implementing the Serializable interface
  //! and containing the body data to write
  //! @param[in] writer The response writer object
  //! @param[in] content_type The corresponding content type
  //! @returns Response& A reference to the response object being modified
  //! @see constants::Mime response_writers::Default
  //! response_writers::TransferEncoding
  //! @section Example
  //! @snippet network/http_server_response_builder_serializable_object.cpp Ex
  template <typename WRty>
  static Response& Build(Response& res, const base::Serializable& serializable,
                         const WRty& writer, const std::string& content_type) {
    return Build(res, serializable.Serialize(), writer, content_type);
  }
};
}  // namespace koobika::hook::network::protocol::http

#endif