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

#ifndef koobika_hook_network_protocol_http_v11_httpserverbase_h
#define koobika_hook_network_protocol_http_v11_httpserverbase_h

#include <chrono>
#include <functional>
#include <iostream>
#include <stdexcept>

#include "auth/basic.h"
#include "http_router.h"
#include "network/transport/server_transport_constants.h"
#include "structured/json/json_value.h"

namespace koobika::hook::network::protocol::http::v11 {
// =============================================================================
// HttpServerBase                                                      [ class ]
// -----------------------------------------------------------------------------
// This class is in charge of providing the default http server base class
// -----------------------------------------------------------------------------
// Template parameters:
//    TRty - transport type being used
//      [requires: ServerTransport]
//    ROty - router type being used
//      [requires: HttpRoutesManager, HttpRoutesPerformer]
//    RQty - request type being used
//    RSty - response type being used
// =============================================================================
template <typename TRty, template <typename, typename> class ROty,
          typename RQty, typename RSty>
class HttpServerBase : public HttpRoutesManager<RQty, RSty> {
 public:
  // ---------------------------------------------------------------------------
  // Constructors/Destructors                                         [ public ]
  // ---------------------------------------------------------------------------
  HttpServerBase(
      const int& workers_number =
          transport::ServerTransportConstants::kNumberOfWorkersValue,
      const int& maximum_number_of_connections =
          transport::ServerTransportConstants::kMaxConnectionsValue) {
    configuration_[transport::ServerTransportConstants::kNumberOfWorkersKey] =
        workers_number;
    configuration_[transport::ServerTransportConstants::kMaxConnectionsKey] =
        maximum_number_of_connections;
  }
  HttpServerBase(const structured::json::JsonObject& configuration)
      : configuration_{configuration} {}
  HttpServerBase(const HttpServerBase&) = delete;
  HttpServerBase(HttpServerBase&&) noexcept = delete;
  ~HttpServerBase() { Stop(); }
  // ---------------------------------------------------------------------------
  // Operators                                                        [ public ]
  // ---------------------------------------------------------------------------
  HttpServerBase& operator=(const HttpServerBase&) = delete;
  HttpServerBase& operator=(HttpServerBase&&) noexcept = delete;
  // ---------------------------------------------------------------------------
  // Methods                                                          [ public ]
  // ---------------------------------------------------------------------------
  // starts server activity uwing the provided port!
  void Start(const std::string& port) {
    if (transport_ != nullptr) {
      // [error] -> server is already running!
      throw std::logic_error("server is already running!");
    }
    configuration_[transport::ServerTransportConstants::kPortKey] = port;
    // let's setup the required transport!
    transport_ = std::make_unique<TRty>();
    // let' start transport activity on a separate thread!
    transport_thread_ = std::make_shared<std::thread>(
        [&router = router_, &auth = auth_, &transport = transport_,
         &configuration = configuration_]() {
          transport->Start(configuration, [&router, &auth](auto const& req,
                                                           auto const& sender) {
            RSty res;
            try {
              if (!router.Perform(req.Uri.GetPath(), req, res, auth)) {
                // [error] -> route is not registered!
                // [to-do] -> inform user back?
                res.NotFound_404();
              }
            } catch (std::exception& e) {
              // [error] -> an exception was thrown!
              // [to-do] -> inform user back!
              res.InternalServerError_500(e.what());
            } catch (...) {
              // [error] -> an exception was thrown!
              // [to-do] -> inform user back!
              res.InternalServerError_500("Unknown server exception!");
            }
            sender(res.Serialize());
          });
        });
  }
  // stops server activity!
  void Stop() {
    if (transport_ != nullptr) {
      transport_->Stop();
    }
    if (transport_thread_->joinable()) {
      transport_thread_->join();
    }
  }

  /*
  pepe
  */

  /*
  // Sets the authorization module for this server
  template <template <typename RQ, typename RS,
                      template <typename KE> typename RP>
            typename AUty,
            template <typename KEty>
            typename RPty = base::RepositoryMemory,
            typename... PAty>
  void Authorization(PAty&&... args) {
    auth_ = std::make_shared<AUty<RQty, RSty, RPty>>(args...);
    router_.handle(*auth_);
  }
  */

  /*
  pepe fin
  */

  // Adds a new <generic> handler to 'nominal' router structures
  void Handle(
      const std::string& route,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
      const HttpMethodValue& methods = HttpConstants::Methods::kAll,
      const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    router_.Handle(route, handler, methods, auth);
  }
  // Adds a new <generic> handler to 'regular-expressions' router structures
  void Handle(
      const std::regex& regex,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
      const HttpMethodValue& methods = HttpConstants::Methods::kAll,
      const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    router_.Handle(regex, handler, methods, auth);
  }
  // Adds a new <options> handler to the 'nominal' router structures
  void Options(
      const std::string& route,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
      const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(route, handler, HttpConstants::Methods::kOptions, auth);
  }
  // Adds a new <options> handler to the 'regex' router structures
  void Options(
      const std::regex& regex,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
      const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(regex, handler, HttpConstants::Methods::kOptions, auth);
  }
  // Adds a new <get> handler to the 'nominal' router structures
  void Get(const std::string& route,
           const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
           const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(route, handler, HttpConstants::Methods::kGet, auth);
  }
  // Adds a new <get> handler to the 'regex' router structures
  void Get(const std::regex& regex,
           const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
           const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(regex, handler, HttpConstants::Methods::kGet, auth);
  }
  // Adds a new <head> handler to the 'nominal' router structures
  void Head(const std::string& route,
            const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
            const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(route, handler, HttpConstants::Methods::kHead, auth);
  }
  // Adds a new <head> handler to the 'regex' router structures
  void Head(const std::regex& regex,
            const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
            const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(regex, handler, HttpConstants::Methods::kHead, auth);
  }
  // Adds a new <post> handler to the 'nominal' router structures
  void Post(const std::string& route,
            const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
            const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(route, handler, HttpConstants::Methods::kPost, auth);
  }
  // Adds a new <post> handler to the 'regex' router structures
  void Post(const std::regex& regex,
            const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
            const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(regex, handler, HttpConstants::Methods::kPost, auth);
  }
  // Adds a new <put> handler to the 'nominal' router structures
  void Put(const std::string& route,
           const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
           const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(route, handler, HttpConstants::Methods::kPut, auth);
  }
  // Adds a new <put> handler to the 'regex' router structures
  void Put(const std::regex& regex,
           const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
           const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(regex, handler, HttpConstants::Methods::kPut, auth);
  }
  // Adds a new <delete> handler to the 'nominal' router structures
  void Delete(
      const std::string& route,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
      const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(route, handler, HttpConstants::Methods::kDelete, auth);
  }
  // Adds a new <delete> handler to the 'regex' router structures
  void Delete(
      const std::regex& regex,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
      const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(regex, handler, HttpConstants::Methods::kDelete, auth);
  }
  // Adds a new <trace> handler to the 'nominal' router structures
  void Trace(
      const std::string& route,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
      const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(route, handler, HttpConstants::Methods::kTrace, auth);
  }
  // Adds a new <trace> handler to the 'regex' router structures
  void Trace(
      const std::regex& regex,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
      const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(regex, handler, HttpConstants::Methods::kTrace, auth);
  }
  // Adds a new <connect> handler to the 'nominal' router structures
  void Connect(
      const std::string& route,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
      const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(route, handler, HttpConstants::Methods::kConnect, auth);
  }
  // Adds a new <connect> handler to the 'regex' router structures
  void Connect(const std::regex& regex,
               const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
               const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) {
    Handle(regex, handler, HttpConstants::Methods::kConnect, auth);
  }

 private:
  // ---------------------------------------------------------------------------
  // Attributes                                                      [ private ]
  // ---------------------------------------------------------------------------
  ROty<RQty, RSty> router_;
  std::shared_ptr<TRty> transport_;
  std::shared_ptr<HttpAuthModule<RQty, RSty>> auth_;
  std::shared_ptr<std::thread> transport_thread_;
  structured::json::JsonObject configuration_;
};
}  // namespace koobika::hook::network::protocol::http::v11

#endif