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

#include <iostream>
#include <chrono>
#include <functional>
#include <stdexcept>

#include "auth/basic.h"
#include "base/key_value_store.h"
#include "base/repository_memory.h"
#include "http_router.h"
#include "network/transport/server_transport_constants.h"

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
  HttpServerBase() {
    configuration_[transport::ServerTransportConstants::kKeyNumberOfWorkers] =
        transport::ServerTransportConstants::kDefNumberOfWorkers;
    configuration_[transport::ServerTransportConstants::kKeyMaxConnections] =
        transport::ServerTransportConstants::kDefMaxConnections;
  }
  HttpServerBase(const int& workers_number,
                 const int& maximum_number_of_connections) {
    configuration_[transport::ServerTransportConstants::kKeyNumberOfWorkers] =
        workers_number;
    configuration_[transport::ServerTransportConstants::kKeyMaxConnections] =
        maximum_number_of_connections;
  }
  HttpServerBase(const base::KeyValueStore<std::string>& configuration)
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
  // Starts server activity
  void Start(const std::string& port) {
    if (transport_ != nullptr) {
      // [error] -> server is already running!
      throw std::logic_error("server is already running!");
    }
    // let's fill the configuration repository with the incoming port!
    configuration_[transport::ServerTransportConstants::kKeyPort] = port;
    on_accept_connection_ = [](const typename TRty::Handle& id) -> bool {
      return true;
    };
    on_connection_accepted_ = [](const typename TRty::Handle& id) -> void {};
    on_connection_rejected_ = [](const typename TRty::Handle& id) -> void {};
    on_data_ = [this](typename TRty::Decoder& decoder,
                      const typename TRty::Handle& id,
                      const typename TRty::Sender& sender,
                      const typename TRty::OnError onError) {
      decoder.Decode(
          // OnSuccess handler! Here goes our code!
          [this, sender](const RQty& req) {
            RSty res;
            try {
              if (!router_.Perform(req.Uri.GetPath(), req, res, auth_)) {
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

            /*
            pepe
            */

            auto start = std::chrono::steady_clock::now();
            sender(res.Serialize());
            auto end = std::chrono::steady_clock::now();

            /*
            pepe fin
            */
          },
          // OnError handler! Here goes our error handling routine
          [id, onError]() { onError(); });
    };
    // let's setup the required transport!
    transport_ = std::make_unique<TRty>();
    transport_->SetOnAcceptConnectionCallback(on_accept_connection_);
    transport_->SetOnConnectionAcceptedCallback(on_connection_accepted_);
    transport_->SetOnConnectionRejectedCallback(on_connection_rejected_);
    transport_->SetOnDataCallback(on_data_);
    transport_->Setup(configuration_);
  }
  // Stops server activity
  void Stop() {
    if (transport_ != nullptr) {
      transport_->Cleanup();
    }
  }
  // Sets the authorization module for this server
  template <template <typename RQ, typename RS,
                      template <typename KE, typename VA> typename RP>
            typename AUty,
            template <typename KEty, typename VAty>
            typename RPty = base::RepositoryMemory,
            typename... PAty>
  void Authorization(PAty&&... args) {
    auth_ = std::make_shared<AUty<RQty, RSty, RPty>>(args...);
    router_.handle(*auth_);
  }
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
  std::unique_ptr<TRty> transport_;
  typename TRty::OnAcceptConnection on_accept_connection_;
  typename TRty::OnConnectionAccepted on_connection_accepted_;
  typename TRty::OnConnectionRejected on_connection_rejected_;
  typename TRty::OnData on_data_;
  base::KeyValueStore<std::string> configuration_;
  std::shared_ptr<HttpAuthModule<RQty, RSty>> auth_;
};
}  // namespace koobika::hook::network::protocol::http::v11

#endif