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

#ifndef koobika_hook_network_transport_servertransport_h
#define koobika_hook_network_transport_servertransport_h

#include <functional>

#include "base/stream.h"

namespace koobika::hook::network::transport {
// =============================================================================
// ServerTransport                                                 [ interface ]
// -----------------------------------------------------------------------------
// This specification holds for server transport interface
// -----------------------------------------------------------------------------
// Template parameters:
//    HDty - server transport handler (native) type being used
//    DEty - server transport request decoder type being used
// =============================================================================
template <typename HDty, typename DEty>
class ServerTransport {
 public:
  // ---------------------------------------------------------------------------
  // Usings                                                           [ public ]
  // ---------------------------------------------------------------------------
  using Sender = std::function<void(const base::Stream&)>;
  using ErrorHandler = std::function<void()>;
  // ---------------------------------------------------------------------------
  // Methods                                                          [ public ]
  // ---------------------------------------------------------------------------
  // |starts current transport activity using the provided (json) configuration.
  virtual void Start(const structured::json::JsonObject&) = 0;
  // |stops transport activity.
  virtual void Stop() = 0;
  // Tries to sends the specified buffer through the transport connection.
  virtual void Send(const HDty&, const base::Stream&) = 0;
};
}  // namespace koobika::hook::network::transport

#endif