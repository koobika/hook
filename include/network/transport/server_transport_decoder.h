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

#ifndef koobika_hook_network_transport_servertransportdecoder_h
#define koobika_hook_network_transport_servertransportdecoder_h

#include <functional>
#include <memory>

#include "base/stream.h"

namespace koobika::hook::network::transport {
// =============================================================================
// ServerTransportDecoder                                          ( interface )
// -----------------------------------------------------------------------------
// This specification holds for server transport decoder interface.
// -----------------------------------------------------------------------------
// Template parameters:
//    RQty - server transport request type being generated
// =============================================================================
template <typename RQty>
class ServerTransportDecoder {
 public:
  // ---------------------------------------------------------------------------
  // USINGs                                                           ( public )
  // ---------------------------------------------------------------------------
  using Sender = std::function<void(const base::Stream&)>;
  using RequestHandler = std::function<void(const RQty&, const Sender&)>;
  using ErrorHandler = std::function<void()>;
  // ---------------------------------------------------------------------------
  // METHODs                                                          ( public )
  // ---------------------------------------------------------------------------
  // Adds (opaque) content to the decoder.
  virtual bool Add(void* buffer, const std::size_t& length) = 0;
  // Tries to decode internal content.
  virtual void Decode(
      const transport::ServerTransportDecoder<RQty>::RequestHandler&
          request_handler,
      const transport::ServerTransportDecoder<RQty>::ErrorHandler&
          error_handler,
      const transport::ServerTransportDecoder<RQty>::Sender& sender) = 0;
};
}  // namespace koobika::hook::network::transport

#endif