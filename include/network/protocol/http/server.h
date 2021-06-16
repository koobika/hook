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
// network/protocol/http/server.h
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

#ifndef koobika_hook_network_protocol_http_server_h
#define koobika_hook_network_protocol_http_server_h

#include "request.h"
#include "response.h"
#include "router.h"
#include "server_base.h"
#include "v1.1/request_decoder.h"
#include "network/transport/tcpip/server.h"

namespace koobika::hook::network::protocol::http {
//! @brief This is the default HTTPServer implementation module using
//! <b>socket-based TCP/IP</b> transport, <b>HTTP1.1</b> request decoder and the
//! <b>default provided</b> router. Offering for the following functionalities:
//! - Leading in performance, even in highly demanding scenarios (e.g.: thousand
//! of simultaneous clients)
//! - Full routing capabilities (both string and regular-expressions based
//! routes)
//! - Controllers, allowing to group any set of related end-points within a
//! dedicated class context
//! - Authentication (NoAuth, Basic, Api-Token and Bearer-Token currently
//! supported by default). Also, making use of an open design concept that will
//! allow any developer to add new authentication methods in an easy and
//! standardized way
//! - Body encoders/decoders, allowing an easy way to read/write from/to body
//! parts using serialized/raw content
//! - Static file server provided by default
//! - Transfer-encoding support (e.g.: gzip compression, chunked content,
//! etc...)
//! - Multipart/form (content-type) support
//! - Stream based (memory/file) data ingestion, avoiding out-of-memory
//! scenarios due to big buffers management scenarios
//! - Hyper fast message decoding routines based on asynchronous dispatching
//! - Fully configurable via builder (e.g.: concurrency levels, maximum number
//! of connections, etc...)
//! - Very low memory footprint (currently, lower than in any other available
//! library)
//! - Exception safe design
//! .
//!
//! For full API specification see ServerBase class
class Server
    : public ServerBase<transport::tcpip::Server<v11::RequestDecoder>, Router> {
 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  //! @brief Creates a Server instance using explicit parameters
  //! @param[in] workers_number The number of workers (threads) that the server
  //! will create to handle I/O communications. It is strongly recommended to
  //! use (as default value) /a std::thread::hardware_concurrency()
  Server(const unsigned int& workers_number) : ServerBase(workers_number) {}
  //! @brief Creates a Server instance using a structured::json::Object object
  //! @param[in] configuration A structured::json::Object object containing the
  //! needed configuration parameters. Structured as follows:
  //! @code{.json}
  //! {
  //!   "number_of_workers": <number>
  //! }
  //! @endcode
  Server(const structured::json::Object& configuration)
      : ServerBase(configuration) {}
  Server(const Server&) = delete;
  Server(Server&&) noexcept = delete;
  ~Server() = default;
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  //
  Server& operator=(const Server&) = delete;
  Server& operator=(Server&&) noexcept = delete;
};
}  // namespace koobika::hook::network::protocol::http

#endif