﻿// /////////////////////////////////////////////////////////////////////////////
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

#ifndef koobika_hook_network_protocol_http_v11_httpserverbuilder_h
#define koobika_hook_network_protocol_http_v11_httpserverbuilder_h

#include "base/builder.h"
#include "base/builder_property.h"
#include "http_server.h"
#include "network/transport/server_transport_constants.h"

namespace koobika::hook::network::protocol::http::v11 {
// =============================================================================
// HttpServerBuilder                                                   ( class )
// -----------------------------------------------------------------------------
// This class is in charge of providing the http server builder
// =============================================================================
class HttpServerBuilder : public base::Builder<HttpServer> {
 public:
  // ---------------------------------------------------------------------------
  // METHODs                                                          ( public )
  // ---------------------------------------------------------------------------
  // Builds the target httpserver object.
  std::shared_ptr<HttpServer> Build() const override {
    return std::make_shared<HttpServer>(NumberOfWorkers,
                                        MaximumNumberOfConnections);
  }
  // ---------------------------------------------------------------------------
  // PROPERTIEs                                                       ( public )
  // ---------------------------------------------------------------------------
  base::BuilderProperty<int, HttpServerBuilder> NumberOfWorkers{
      this, transport::ServerTransportConstants::kNumberOfWorkersValue};
  base::BuilderProperty<int, HttpServerBuilder> MaximumNumberOfConnections{
      this, transport::ServerTransportConstants::kMaxConnectionsValue};
};
}  // namespace koobika::hook::network::protocol::http::v11

#endif