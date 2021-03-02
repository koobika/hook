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

#include <memory>
#include <functional>

namespace koobika::hook::network::transport {
  // =============================================================================
  // ServerTransportDecoder                                          [ interface ]
  // -----------------------------------------------------------------------------
  // This specification holds for server transport decoder interface
  // -----------------------------------------------------------------------------
  // Template parameters:
  //    RQty - server transport request type being generated
  // =============================================================================
  template <typename RQty>
  class ServerTransportDecoder {
  public:
    // ---------------------------------------------------------------------------
    // Usings                                                           [ public ]
    // ---------------------------------------------------------------------------
    using OnErrorHandler = std::function<void()>;
    // ---------------------------------------------------------------------------
    // Methods                                                          [ public ]
    // ---------------------------------------------------------------------------
    // Adds (opaque) content to the decoder.
    virtual bool Add(void*, const std::size_t&) = 0;
    // Tries to decode internal content.
    virtual bool Decode(const OnErrorHandler&) = 0;
  };
}  // namespace koobika::hook::network::transport

#endif