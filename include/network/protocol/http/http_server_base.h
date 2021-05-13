﻿// /////////////////////////////////////////////////////////////////////////////
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
// network/protocol/http/http_server_base.h
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

#ifndef koobika_hook_network_protocol_http_httpserverbase_h
#define koobika_hook_network_protocol_http_httpserverbase_h

#include <chrono>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <thread>

#include "auth/modules/api_key.h"
#include "auth/modules/basic.h"
#include "auth/modules/no_auth.h"
#include "constants/methods.h"
#include "http_controller.h"
#include "http_response_writer.h"
#include "http_router.h"
#include "network/transport/server_transport_constants.h"
#include "structured/json/value.h"

namespace koobika::hook::network::protocol::http {
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
// =============================================================================
template <typename TRty, typename ROty>
class HttpServerBase : public HttpRoutesManager {
 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  HttpServerBase(const unsigned int& workers_number) {
    configuration_[transport::ServerTransportConstants::kNumberOfWorkersKey] =
        workers_number;
  }
  HttpServerBase(const structured::json::Object& configuration)
      : configuration_(configuration) {}
  HttpServerBase(const HttpServerBase&) = delete;
  HttpServerBase(HttpServerBase&&) noexcept = delete;
  ~HttpServerBase() { Stop(); }
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  //
  HttpServerBase& operator=(const HttpServerBase&) = delete;
  HttpServerBase& operator=(HttpServerBase&&) noexcept = delete;
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
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
        [&router = router_, &transport = transport_,
         &configuration = configuration_]() {
          transport->Start(
              configuration, [&router](auto const& req, auto const& sender) {
                HttpResponse res;
                try {
                  if (!router.Perform(req.Uri.GetPath(), req, res)) {
                    // ((Error)) -> route is not registered!
                    // ((To-Do)) -> inform user back?
                    res.NotFound_404();
                  }
                } catch (std::exception& e) {
                  // ((Error)) -> a known exception was thrown!
                  // ((To-Do)) -> inform user back!
                  res.InternalServerError_500(e.what());
                  res.Body.Close();
                } catch (...) {
                  // ((Error)) -> a unknown exception was thrown!
                  // ((To-Do)) -> inform user back!
                  res.InternalServerError_500("Unknown server exception!");
                  res.Body.Close();
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
    if (transport_thread_ != nullptr && transport_thread_->joinable()) {
      transport_thread_->join();
    }
  }
  // Adds a new <controller> to the internal map using an string route.
  template <typename AUty, typename... PAty>
  void Handle(const PAty&... params) {
    static std::shared_ptr<AUty> internal = std::make_shared<AUty>(params...);
    internal->AddToRouter(router_);
  }
  // Adds a new <generic> route to 'nominal' router structures.
  void Handle(
      const std::string& route,
      const typename HttpRoutesTypes::Handler& handler,
      const HttpMethodValue& method = constants::Methods::kAll) override {
    router_.Handle(route, handler, method);
  }
  // Adds a new <generic> route to 'regular-expressions' router structures.
  void Handle(
      const std::regex& regex, const typename HttpRoutesTypes::Handler& handler,
      const HttpMethodValue& method = constants::Methods::kAll) override {
    router_.Handle(regex, handler, method);
  }
  // Adds a new <options> route to the 'nominal' router structures
  void Options(const std::string& route,
               const typename HttpRoutesTypes::Handler& handler) override {
    Handle(route, handler, constants::Methods::kOptions);
  }
  // Adds a new <options> route to the 'regex' router structures
  void Options(const std::regex& regex,
               const typename HttpRoutesTypes::Handler& handler) override {
    Handle(regex, handler, constants::Methods::kOptions);
  }
  // Adds a new <get> route to the 'nominal' router structures
  void Get(const std::string& route,
           const typename HttpRoutesTypes::Handler& handler) override {
    Handle(route, handler, constants::Methods::kGet);
  }
  // Adds a new <get> route to the 'regex' router structures
  void Get(const std::regex& regex,
           const typename HttpRoutesTypes::Handler& handler) override {
    Handle(regex, handler, constants::Methods::kGet);
  }
  // Adds a new <head> route to the 'nominal' router structures
  void Head(const std::string& route,
            const typename HttpRoutesTypes::Handler& handler) override {
    Handle(route, handler, constants::Methods::kHead);
  }
  // Adds a new <head> route to the 'regex' router structures
  void Head(const std::regex& regex,
            const typename HttpRoutesTypes::Handler& handler) override {
    Handle(regex, handler, constants::Methods::kHead);
  }
  // Adds a new <post> route to the 'nominal' router structures
  void Post(const std::string& route,
            const typename HttpRoutesTypes::Handler& handler) override {
    Handle(route, handler, constants::Methods::kPost);
  }
  // Adds a new <post> route to the 'regex' router structures
  void Post(const std::regex& regex,
            const typename HttpRoutesTypes::Handler& handler) override {
    Handle(regex, handler, constants::Methods::kPost);
  }
  // Adds a new <put> route to the 'nominal' router structures
  void Put(const std::string& route,
           const typename HttpRoutesTypes::Handler& handler) override {
    Handle(route, handler, constants::Methods::kPut);
  }
  // Adds a new <put> route to the 'regex' router structures
  void Put(const std::regex& regex,
           const typename HttpRoutesTypes::Handler& handler) override {
    Handle(regex, handler, constants::Methods::kPut);
  }
  // Adds a new <delete> route to the 'nominal' router structures
  void Delete(const std::string& route,
              const typename HttpRoutesTypes::Handler& handler) override {
    Handle(route, handler, constants::Methods::kDelete);
  }
  // Adds a new <delete> route to the 'regex' router structures
  void Delete(const std::regex& regex,
              const typename HttpRoutesTypes::Handler& handler) override {
    Handle(regex, handler, constants::Methods::kDelete);
  }
  // Adds a new <trace> route to the 'nominal' router structures
  void Trace(const std::string& route,
             const typename HttpRoutesTypes::Handler& handler) override {
    Handle(route, handler, constants::Methods::kTrace);
  }
  // Adds a new <trace> route to the 'regex' router structures
  void Trace(const std::regex& regex,
             const typename HttpRoutesTypes::Handler& handler) override {
    Handle(regex, handler, constants::Methods::kTrace);
  }
  // Adds a new <connect> route to the 'nominal' router structures
  void Connect(const std::string& route,
               const typename HttpRoutesTypes::Handler& handler) override {
    Handle(route, handler, constants::Methods::kConnect);
  }
  // Adds a new <connect> route to the 'regex' router structures
  void Connect(const std::regex& regex,
               const typename HttpRoutesTypes::Handler& handler) {
    Handle(regex, handler, constants::Methods::kConnect);
  }

 private:
  // ___________________________________________________________________________
  // ATTRIBUTEs                                                      ( private )
  //
  ROty router_;
  std::shared_ptr<TRty> transport_;
  std::shared_ptr<std::thread> transport_thread_;
  structured::json::Object configuration_;
};
}  // namespace koobika::hook::network::protocol::http

#endif