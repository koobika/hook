// /////////////////////////////////////////////////////////////////////////////
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
#include "structured/json/json_value.h"
#include "constants/limits.h"

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
  HttpServerBase(const structured::json::JsonObject& configuration)
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
    transport_thread_ = std::make_shared<std::thread>([this]() {
      transport_->Start(
          configuration_, [this](auto const& req, auto const& sender) {
            HttpResponse res;
            try {
              if (!router_.Perform(req, res)) {
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
            void* body_buf = nullptr;
            std::size_t body_len = 0;
            // Check for a 'RAW' based stream!
            if (res.Raw.Length()) {
              if (res.Raw.GetInternalBuffer(body_buf, body_len)) {
                // <memory-based> body buffer!
                sender((const char*)body_buf, body_len);
              } else {
                // <file-based> body buffer!
              }
            } else {
              if (res.Body.GetInternalBuffer(body_buf, body_len)) {
                // <memory-based> body buffer!
                std::string content;
                res.Headers.Set(constants::Headers::kContentLength, body_len);
                content.append(constants::Strings::kHttpVersion)
                    .append(constants::Strings::kSpace)
                    .append(std::to_string(res.StatusCode))
                    .append(constants::Strings::kSpace)
                    .append(res.ReasonPhrase)
                    .append(constants::Strings::kCrLf)
                    .append(res.Headers.Dump())
                    .append(constants::Strings::kCrLf)
                    .append((const char*)body_buf, body_len);
                sender(content.c_str(), content.length());
              } else {
                // <file-based> body buffer!

                /*
                pepe
                */

                /*
                sender(res.Serialize());
                */

                /*
                pepe fin
                */
              }
            }
          });
    });

    /*
    pepe
    */

    support_thread_ = std::make_shared<std::thread>([this] {
      using namespace std::chrono_literals;
      while (transport_) {
        updateInternalDate();
        std::this_thread::sleep_for(1s);
      }
    });

    /*
    pepe fin
    */
  }
  // Stops server activity.
  void Stop() {
    if (transport_ != nullptr) {
      transport_->Stop();
    }
    if (transport_thread_ != nullptr && transport_thread_->joinable()) {
      transport_thread_->join();
    }
    transport_ = nullptr;
    if (support_thread_ != nullptr && support_thread_->joinable()) {
      support_thread_->join();
    }
  }
  // Adds a new <controller> to the internal map using an string route.
  template <typename AUty, typename... PAty>
  void Handle(const PAty&... params) {
    static std::shared_ptr<AUty> internal = std::make_shared<AUty>(params...);
    internal->AddToRouter(router_);
  }
  // Adds a new <controller> to the internal map using an string route.
  template <typename AUty>
  void Handle(AUty&& auth) {
    auth.AddToRouter(router_);
  }
  // Adds a new <generic> route to 'nominal' router structures.
  void Handle(
      const std::string& route, const HttpRoutingHandler& handler,
      const HttpMethodValue& method = constants::Methods::kAll) override {
    router_.Handle(route, handler, method);
  }
  // Adds a new <generic> route to 'nominal' router structures.
  void Handle(
      const std::string& route,
      const HttpRoutingHandlerExtended& handler_extended,
      const HttpMethodValue& method = constants::Methods::kAll) override {
    router_.Handle(route, handler_extended, method);
  }
  // Adds a new <options> route to the 'nominal' router structures
  void Options(const std::string& route,
               const HttpRoutingHandler& handler) override {
    Handle(route, handler, constants::Methods::kOptions);
  }
  // Adds a new <options> route to the 'nominal' router structures
  void Options(const std::string& route,
               const HttpRoutingHandlerExtended& handler_extended) override {
    Handle(route, handler_extended, constants::Methods::kOptions);
  }
  // Adds a new <get> route to the 'nominal' router structures
  void Get(const std::string& route,
           const HttpRoutingHandler& handler) override {
    Handle(route, handler, constants::Methods::kGet);
  }
  // Adds a new <get> route to the 'nominal' router structures
  void Get(const std::string& route,
           const HttpRoutingHandlerExtended& handler_extended) override {
    Handle(route, handler_extended, constants::Methods::kGet);
  }
  // Adds a new <head> route to the 'nominal' router structures
  void Head(const std::string& route,
            const HttpRoutingHandler& handler) override {
    Handle(route, handler, constants::Methods::kHead);
  }
  // Adds a new <head> route to the 'nominal' router structures
  void Head(const std::string& route,
            const HttpRoutingHandlerExtended& handler_extended) override {
    Handle(route, handler_extended, constants::Methods::kHead);
  }
  // Adds a new <post> route to the 'nominal' router structures
  void Post(const std::string& route,
            const HttpRoutingHandler& handler) override {
    Handle(route, handler, constants::Methods::kPost);
  }
  // Adds a new <post> route to the 'nominal' router structures
  void Post(const std::string& route,
            const HttpRoutingHandlerExtended& handler_extended) override {
    Handle(route, handler_extended, constants::Methods::kPost);
  }
  // Adds a new <put> route to the 'nominal' router structures
  void Put(const std::string& route,
           const HttpRoutingHandler& handler) override {
    Handle(route, handler, constants::Methods::kPut);
  }
  // Adds a new <put> route to the 'nominal' router structures
  void Put(const std::string& route,
           const HttpRoutingHandlerExtended& handler_extended) override {
    Handle(route, handler_extended, constants::Methods::kPut);
  }
  // Adds a new <delete> route to the 'nominal' router structures
  void Delete(const std::string& route,
              const HttpRoutingHandler& handler) override {
    Handle(route, handler, constants::Methods::kDelete);
  }
  // Adds a new <delete> route to the 'nominal' router structures
  void Delete(const std::string& route,
              const HttpRoutingHandlerExtended& handler_extended) override {
    Handle(route, handler_extended, constants::Methods::kDelete);
  }
  // Adds a new <trace> route to the 'nominal' router structures
  void Trace(const std::string& route,
             const HttpRoutingHandler& handler) override {
    Handle(route, handler, constants::Methods::kTrace);
  }
  // Adds a new <trace> route to the 'nominal' router structures
  void Trace(const std::string& route,
             const HttpRoutingHandlerExtended& handler_extended) override {
    Handle(route, handler_extended, constants::Methods::kTrace);
  }
  // Adds a new <connect> route to the 'nominal' router structures
  void Connect(const std::string& route,
               const HttpRoutingHandler& handler) override {
    Handle(route, handler, constants::Methods::kConnect);
  }
  // Adds a new <connect> route to the 'nominal' router structures
  void Connect(const std::string& route,
               const HttpRoutingHandlerExtended& handler_extended) override {
    Handle(route, handler_extended, constants::Methods::kConnect);
  }
  // Returns server currently stored date.
  const char* GetCurrentDate() const {
    std::scoped_lock<std::mutex> sync_protection(date_buffer_mutex);
    return date_buffer_;
  }

 private:
  // Updates current stored server date.
  void updateInternalDate() {
    std::scoped_lock<std::mutex> sync_protection(date_buffer_mutex);
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(date_buffer_, kMaxLength, "%a, %d %h %G %T GMT", timeinfo);
  }

  // ___________________________________________________________________________
  // CONSTANTs                                                       ( private )
  //
  static constexpr std::size_t kMaxLength = 80;
  // ___________________________________________________________________________
  // ATTRIBUTEs                                                      ( private )
  //
  ROty router_;
  std::shared_ptr<TRty> transport_;
  std::shared_ptr<std::thread> transport_thread_;
  std::shared_ptr<std::thread> support_thread_;
  structured::json::JsonObject configuration_;
  // Attributes needed for server-date management!
  char date_buffer_[kMaxLength] = {0};
  mutable std::mutex date_buffer_mutex;
  struct tm* timeinfo = nullptr;
  time_t rawtime;
};
}  // namespace koobika::hook::network::protocol::http

#endif