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
#include "http_controller.h"
#include "network/transport/server_transport_constants.h"
#include "structured/json/json_value.h"

namespace koobika::hook::network::protocol::http::v11 {
// =============================================================================
// HttpServerBase                                                      ( class )
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
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
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
  // OPERATORs                                                        ( public )
  // ---------------------------------------------------------------------------
  HttpServerBase& operator=(const HttpServerBase&) = delete;
  HttpServerBase& operator=(HttpServerBase&&) noexcept = delete;
  // ---------------------------------------------------------------------------
  // METHODs                                                          ( public )
  // ---------------------------------------------------------------------------
  // Starts server activity uwing the provided port.
  void Start(const std::string& port) {
    if (transport_ != nullptr) {
      // ((Error)) -> server is already running!
      throw std::logic_error("server is already running!");
    }
    configuration_[transport::ServerTransportConstants::kPortKey] = port;
    // let's setup the required transport!
    transport_ = std::make_unique<TRty>();
    // let' start transport activity on a separate thread!
    transport_thread_ = std::make_shared<std::thread>(
        [&router = router_, &auth_support = auth_, &transport = transport_,
         &configuration = configuration_]() {
          transport->Start(configuration, [&router, &auth_support](
                                              auto const& req,
                                              auto const& sender) {
            RSty res;
            try {
              if (!router.Perform(req.Uri.GetPath(), req, res, auth_support)) {
                // ((Error)) -> route is not registered!
                // ((To-Do)) -> inform user back?
                res.NotFound_404();
              }
            } catch (std::exception& e) {
              // ((Error)) -> an exception was thrown!
              // ((To-Do)) -> inform user back!
              res.InternalServerError_500(e.what());
            } catch (...) {
              // ((Error)) -> an exception was thrown!
              // ((To-Do)) -> inform user back!
              res.InternalServerError_500("Unknown server exception!");
            }
            sender(res.Serialize());
          });
        });
  }
  // Stops server activity.
  void Stop() {
    if (transport_ != nullptr) {
      transport_->Stop();
    }
    if (transport_thread_->joinable()) {
      transport_thread_->join();
    }
  }
  // Adds a new <templated-controller> to the internal map.
  template <template <typename, typename> typename CNty, typename... ARty>
  void Handle() {
    auto controller = std::make_shared<CNty<RQty, RSty>>(ARty...);
    controller->AddToRouter(router_);
  }
  // Adds a new <templated-controller> to the internal map.
  template <typename CNty, typename... ARty>
  void Handle() {
    auto controller = std::make_shared<CNty>(ARty...);
    controller->AddToRouter(router_);
  }
  // Adds a new <generic> route to 'nominal' router structures.
  void Handle(
      const std::string& route,
      const typename HttpRoutesTypes<RQty, RSty>::RouteHandler& route_handler,
      const HttpMethodValue& method = HttpConstants::Methods::kAll,
      const HttpAuthSupport& auth_support = HttpAuthSupport::kDisabled,
      const std::shared_ptr<HttpController<RQty, RSty>>& controller =
          nullptr) override {
    router_.Handle(route, route_handler, method, auth_support, controller);
  }
  // Adds a new <generic> route to 'regular-expressions' router structures.
  void Handle(
      const std::regex& regex,
      const typename HttpRoutesTypes<RQty, RSty>::RouteHandler& route_handler,
      const HttpMethodValue& method = HttpConstants::Methods::kAll,
      const HttpAuthSupport& auth_support = HttpAuthSupport::kDisabled,
      const std::shared_ptr<HttpController<RQty, RSty>>& controller =
          nullptr) override {
    router_.Handle(regex, route_handler, method, auth_support, controller);
  }
  // Adds a new <options> route to the 'nominal' router structures
  void Options(
      const std::string& route,
      const typename HttpRoutesTypes<RQty, RSty>::RouteHandler& route_handler,
      const HttpAuthSupport& auth_support =
          HttpAuthSupport::kDisabled) override {
    Handle(route, route_handler, HttpConstants::Methods::kOptions,
           auth_support);
  }
  // Adds a new <options> route to the 'regex' router structures
  void Options(
      const std::regex& regex,
      const typename HttpRoutesTypes<RQty, RSty>::RouteHandler& route_handler,
      const HttpAuthSupport& auth_support =
          HttpAuthSupport::kDisabled) override {
    Handle(regex, route_handler, HttpConstants::Methods::kOptions,
           auth_support);
  }
  // Adds a new <get> route to the 'nominal' router structures
  void Get(const std::string& route,
           const typename HttpRoutesTypes<RQty, RSty>::RouteHandler& route_handler,
           const HttpAuthSupport& auth_support =
               HttpAuthSupport::kDisabled) override {
    Handle(route, route_handler, HttpConstants::Methods::kGet, auth_support);
  }
  // Adds a new <get> route to the 'regex' router structures
  void Get(const std::regex& regex,
           const typename HttpRoutesTypes<RQty, RSty>::RouteHandler& route_handler,
           const HttpAuthSupport& auth_support =
               HttpAuthSupport::kDisabled) override {
    Handle(regex, route_handler, HttpConstants::Methods::kGet, auth_support);
  }
  // Adds a new <head> route to the 'nominal' router structures
  void Head(const std::string& route,
            const typename HttpRoutesTypes<RQty, RSty>::RouteHandler& route_handler,
            const HttpAuthSupport& auth_support =
                HttpAuthSupport::kDisabled) override {
    Handle(route, route_handler, HttpConstants::Methods::kHead, auth_support);
  }
  // Adds a new <head> route to the 'regex' router structures
  void Head(const std::regex& regex,
            const typename HttpRoutesTypes<RQty, RSty>::RouteHandler& route_handler,
            const HttpAuthSupport& auth_support =
                HttpAuthSupport::kDisabled) override {
    Handle(regex, route_handler, HttpConstants::Methods::kHead, auth_support);
  }
  // Adds a new <post> route to the 'nominal' router structures
  void Post(const std::string& route,
            const typename HttpRoutesTypes<RQty, RSty>::RouteHandler& route_handler,
            const HttpAuthSupport& auth_support =
                HttpAuthSupport::kDisabled) override {
    Handle(route, route_handler, HttpConstants::Methods::kPost, auth_support);
  }
  // Adds a new <post> route to the 'regex' router structures
  void Post(const std::regex& regex,
            const typename HttpRoutesTypes<RQty, RSty>::RouteHandler& route_handler,
            const HttpAuthSupport& auth_support =
                HttpAuthSupport::kDisabled) override {
    Handle(regex, route_handler, HttpConstants::Methods::kPost, auth_support);
  }
  // Adds a new <put> route to the 'nominal' router structures
  void Put(const std::string& route,
           const typename HttpRoutesTypes<RQty, RSty>::RouteHandler& route_handler,
           const HttpAuthSupport& auth_support =
               HttpAuthSupport::kDisabled) override {
    Handle(route, route_handler, HttpConstants::Methods::kPut, auth_support);
  }
  // Adds a new <put> route to the 'regex' router structures
  void Put(const std::regex& regex,
           const typename HttpRoutesTypes<RQty, RSty>::RouteHandler& route_handler,
           const HttpAuthSupport& auth_support =
               HttpAuthSupport::kDisabled) override {
    Handle(regex, route_handler, HttpConstants::Methods::kPut, auth_support);
  }
  // Adds a new <delete> route to the 'nominal' router structures
  void Delete(
      const std::string& route,
      const typename HttpRoutesTypes<RQty, RSty>::RouteHandler& route_handler,
      const HttpAuthSupport& auth_support =
          HttpAuthSupport::kDisabled) override {
    Handle(route, route_handler, HttpConstants::Methods::kDelete, auth_support);
  }
  // Adds a new <delete> route to the 'regex' router structures
  void Delete(
      const std::regex& regex,
      const typename HttpRoutesTypes<RQty, RSty>::RouteHandler& route_handler,
      const HttpAuthSupport& auth_support =
          HttpAuthSupport::kDisabled) override {
    Handle(regex, route_handler, HttpConstants::Methods::kDelete, auth_support);
  }
  // Adds a new <trace> route to the 'nominal' router structures
  void Trace(const std::string& route,
             const typename HttpRoutesTypes<RQty, RSty>::RouteHandler& route_handler,
             const HttpAuthSupport& auth_support =
                 HttpAuthSupport::kDisabled) override {
    Handle(route, route_handler, HttpConstants::Methods::kTrace, auth_support);
  }
  // Adds a new <trace> route to the 'regex' router structures
  void Trace(const std::regex& regex,
             const typename HttpRoutesTypes<RQty, RSty>::RouteHandler& route_handler,
             const HttpAuthSupport& auth_support =
                 HttpAuthSupport::kDisabled) override {
    Handle(regex, route_handler, HttpConstants::Methods::kTrace, auth_support);
  }
  // Adds a new <connect> route to the 'nominal' router structures
  void Connect(
      const std::string& route,
      const typename HttpRoutesTypes<RQty, RSty>::RouteHandler& route_handler,
      const HttpAuthSupport& auth_support =
          HttpAuthSupport::kDisabled) override {
    Handle(route, route_handler, HttpConstants::Methods::kConnect,
           auth_support);
  }
  // Adds a new <connect> route to the 'regex' router structures
  void Connect(
      const std::regex& regex,
      const typename HttpRoutesTypes<RQty, RSty>::RouteHandler& route_handler,
      const HttpAuthSupport& auth_support = HttpAuthSupport::kDisabled) {
    Handle(regex, route_handler, HttpConstants::Methods::kConnect,
           auth_support);
  }

 private:
  // ---------------------------------------------------------------------------
  // ATTRIBUTEs                                                      ( private )
  // ---------------------------------------------------------------------------
  ROty<RQty, RSty> router_;
  std::shared_ptr<TRty> transport_;
  std::shared_ptr<HttpAuthModule<typename HttpRoutesTypes<RQty, RSty>::Request,
                                 typename HttpRoutesTypes<RQty, RSty>::Response>>
      auth_;
  std::shared_ptr<std::thread> transport_thread_;
  structured::json::JsonObject configuration_;
};
}  // namespace koobika::hook::network::protocol::http::v11

#endif