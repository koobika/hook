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

#ifndef koobika_hook_network_transport_servertransportconstants_h
#define koobika_hook_network_transport_servertransportconstants_h

#include <cstddef>

namespace koobika::hook::network::transport {
// =============================================================================
// ServerTransportConstants                                            [ class ]
// -----------------------------------------------------------------------------
// This specification holds for server transport constants
// =============================================================================
struct ServerTransportConstants {
  // ---------------------------------------------------------------------------
  // Constants                                                        [ public ]
  // ---------------------------------------------------------------------------
  // Buffering capacity.
  static constexpr std::size_t kDefaultReadBufferSize = 8192;
  static constexpr std::size_t kDefaultWriteBufferSize = 8192;
  // Port.
  static constexpr char kPortKey[] = "port";
  static constexpr char kPortValue[] = "8542";
  // Number of workers.
  static constexpr char kNumberOfWorkersKey[] = "number_of_workers";
  static constexpr int kNumberOfWorkersValue = 3;
  // Maximum number of simultaneous connections.
  static constexpr char kMaxConnectionsKey[] = "max_connections";
  static constexpr int kMaxConnectionsValue = 256;
};
}  // namespace koobika::hook::network::transport

#endif