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

#include "base/key_value_store.h"
#include "base/stream.h"

namespace koobika::hook::network::transport {
// =============================================================================
// ServerTransport                                                 [ interface ]
// -----------------------------------------------------------------------------
// This specification holds for server transport interface
// -----------------------------------------------------------------------------
// Template parameters:
//    HDty - server transport handler (native) type being used
//    DEty - server transport decoder type being used
// =============================================================================
template <typename HDty, typename DEty>
class ServerTransport {
 public:
  // ---------------------------------------------------------------------------
  // Usings                                                           [ public ]
  // ---------------------------------------------------------------------------
  using Handle = HDty;
  using Decoder = DEty;
  using Sender = std::function<void(const base::Stream&)>;
  using OnError = std::function<void()>;
  using OnAcceptConnection = std::function<bool(const Handle&)>;
  using OnConnectionAccepted = std::function<void(const Handle&)>;
  using OnConnectionRejected = std::function<void(const Handle&)>;
  using OnData =
      std::function<void(DEty&, const Handle&, const Sender&, const OnError&)>;
  // ---------------------------------------------------------------------------
  // Methods                                                          [ public ]
  // ---------------------------------------------------------------------------
  // Sets up current transport object
  virtual void Setup(const base::KeyValueStore<std::string>&) = 0;
  // Cleans up current transport object
  virtual void Cleanup(void) = 0;
  // Tries to sends the specified buffer through the transport connection
  virtual void Send(const Handle&, const base::Stream&) = 0;
  // This method will be used to set the function in charge of accepting (or
  //  not an incoming connection attempt within the transport (callback)
  virtual void SetOnAcceptConnectionCallback(const OnAcceptConnection&) = 0;
  // This method will be used to set the function in charge of notifying for
  //  every accepted connection within the transport (callback)
  virtual void SetOnConnectionAcceptedCallback(const OnConnectionAccepted&) = 0;
  // This method will be used to set the function in charge of notifying for
  //  every rejected connection within the transport (callback)
  virtual void SetOnConnectionRejectedCallback(const OnConnectionRejected&) = 0;
  // This method will be used to set the function in charge of notifying for
  //  incoming data within the provided connection (callback)
  virtual void SetOnDataCallback(const OnData&) = 0;
};
}  // namespace koobika::hook::network::transport

#endif