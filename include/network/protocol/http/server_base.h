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
// network/protocol/http/server_base.h
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

#ifndef koobika_hook_network_protocol_http_serverbase_h
#define koobika_hook_network_protocol_http_serverbase_h

#include <chrono>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <thread>

#include "auth/modules/api_key.h"
#include "auth/modules/basic.h"
#include "auth/modules/no_auth.h"
#include "constants/methods.h"
#include "routes_controller.h"
#include "response_builder.h"
#include "router.h"
#include "network/transport/server_constants.h"
#include "structured/json/value.h"
#include "constants/limits.h"

namespace koobika::hook::network::protocol::http {
//! @brief Http server (templated) base class
//! 
//! Needing for the following parameters:
//! @tparam TRty Transport type being used [requires: ServerTransport
//! interface implementation]
//! @tparam ROty Router type being used [requires: RoutesManager,
//! RoutesPerformer interfaces implementation]
template <typename TRty, typename ROty>
class ServerBase : public RoutesManager {
 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  //! @brief Creates a ServerBase instance using explicit parameters
  //! @param[in] workers_number The number of workers (threads) that the server
  //! will create to handle I/O communications. It is strongly recommended to
  //! use (as default value) std::thread::hardware_concurrency()
  ServerBase(const unsigned int& workers_number) {
    configuration_[transport::ServerConstants::kNumberOfWorkersKey] =
        workers_number;
  }
  //! @brief Creates a Server instance using a structured::json::Object object
  //! @param[in] configuration A structured::json::Object object containing the
  //! needed configuration parameters. Structured as follows:
  //! @code{.json}
  //! {
  //!   "number_of_workers": <number>
  //! }
  //! @endcode
  ServerBase(const structured::json::Object& configuration)
      : configuration_(configuration) {}
  ServerBase(const ServerBase&) = delete;
  ServerBase(ServerBase&&) noexcept = delete;
  ~ServerBase() { Stop(); }
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  //
  ServerBase& operator=(const ServerBase&) = delete;
  ServerBase& operator=(ServerBase&&) noexcept = delete;
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  //! @brief Starts server activity
  //! @param[in] port The port being used by the server
  //! @throws std::logic_error If server is already running
  void Start(const std::string& port) {
    if (transport_ != nullptr) {
      // ((Error)) -> server is already running!
      throw std::logic_error("Server is already running!");
    }
    configuration_[transport::ServerConstants::kPortKey] = port;
    // let's setup the required transport!
    transport_ = std::make_unique<TRty>();
    // let' start transport activity on a separate thread!
    transport_thread_ = std::make_shared<std::thread>([this]() {
      transport_->Start(configuration_, [this](auto const& req, auto& res,
                                               auto const& sender) {
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
        // Check for a 'RAW' based stream!
        if (!res.Raw.Length()) {
          if (!res.Headers.Exist(constants::Headers::kTransferEncoding)) {
            res.Headers.Set(constants::Headers::kContentLength,
                            res.Body.Length());
          }
          res.Raw.Write(constants::Strings::kHttpVersion)
              .Write(constants::Strings::kSpace)
              .Write(std::to_string(res.StatusCode))
              .Write(constants::Strings::kSpace)
              .Write(res.ReasonPhrase)
              .Write(constants::Strings::kCrLf);
          res.Headers.DumpTo(res.Raw);
          // append <response-body>!
          res.Raw.Write(res.Body);
        }
        sender(res.Raw);
      });
    });
    // let's start the support thread!
    support_thread_ = std::make_shared<std::thread>([this] {
      using namespace std::chrono_literals;
      while (transport_) {
        updateInternalDate();
        std::this_thread::sleep_for(1s);
      }
    });
  }
  //! @brief Stops server activity
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
  //! @brief Adds the specified user __controller__ to the server
  //! @tparam CNty The controller class to add
  //! @tparam PAty The (variadic) list of arguments needed by the controller
  //! class for construction
  //! @param[in] params The list of parameters needed by the controller class
  //! constructor/s
  //! @throws std::logic_error If user tries to add controllers while server is up
  //! and running
  //! @section Example
  //! @snippet network/http_server_controller.cpp Example
  template <typename CNty, typename... PAty>
  void Handle(const PAty&... params) {
    if (transport_ != nullptr) {
      // ((Error)) -> can't add more handlers while server is running!
      throw std::logic_error("Can't add controllers while server is running!");
    }
    static std::shared_ptr<CNty> internal = std::make_shared<CNty>(params...);
    internal->AddToRouter(router_);
  }
  //! @brief Adds an string-based route handler to the server
  //! @param[in] route The route path to add
  //! @param[in] handler The function performing the logic
  //! @param[in] method The method type being handled
  //! @throws std::logic_error If user tries to add handlers while server is up and running
  //! @see constants::Methods
  //! @remarks <em>RoutingHandler</em> is just an alias for
  //! std::function<void(const Request&, Response&)><br>
  //! @section Example
  //! @snippet network/http_server_hello_world.cpp Example
  void Handle(const std::string& route, const RoutingHandler& handler,
              const MethodValue& method = constants::Methods::kAll) override {
    if (transport_ != nullptr) {
      // ((Error)) -> can't add more handlers while server is running!
      throw std::logic_error("Can't add handlers while server is running!");
    }
    router_.Handle(route, handler, method);
  }
  //! @brief Adds an string-based route extended handler to the server
  //! @param[in] route The route path to add
  //! @param[in] handler The extended function performing the logic
  //! @param[in] method The method type being handled
  //! @throws std::logic_error If user tries to add handlers while server is up
  //! and running
  //! @see constants::Methods
  //! @remarks <em>RoutingHandlerExtended</em> is just an alias for
  //! std::function<void(const Request&, Response&, const Parameters&)><br>
  //! <em>Parameters</em> is just an alias for std::unordered_map<std::string,
  //! std::string>
  //! @section Example
  //! @snippet network/http_server_extended_routing_parameters.cpp Example
  void Handle(const std::string& route,
              const RoutingHandlerExtended& handler_extended,
              const MethodValue& method = constants::Methods::kAll) override {
    if (transport_ != nullptr) {
      // ((Error)) -> can't add more handlers while server is running!
      throw std::logic_error("Can't add handlers while server is running!");
    }
    router_.Handle(route, handler_extended, method);
  }
  //! @brief Adds an string-based OPTIONS route handler to the server
  //! @param[in] route The route path to add
  //! @param[in] handler The function performing the logic
  //! @throws std::logic_error If user tries to add handlers while server is up
  //! and running
  //! @remarks <em>RoutingHandler</em> is just an alias for
  //! std::function<void(const Request&, Response&)><br>
  //! @section Example
  //! @snippet network/http_server_handler_shortcut.cpp OPTIONS
  void Options(const std::string& route,
               const RoutingHandler& handler) override {
    Handle(route, handler, constants::Methods::kOptions);
  }
  //! @brief Adds an string-based OPTIONS route extended handler to the server
  //! @param[in] route The route path to add
  //! @param[in] handler The extended function performing the logic
  //! @throws std::logic_error If user tries to add handlers while server is up
  //! and running
  //! @remarks <em>RoutingHandlerExtended</em> is just an alias for
  //! std::function<void(const Request&, Response&, const Parameters&)><br>
  //! <em>Parameters</em> is just an alias for std::unordered_map<std::string,
  //! std::string>
  //! @section Example
  //! @snippet network/http_server_handler_shortcut.cpp OPTIONS-EXTENDED
  void Options(const std::string& route,
               const RoutingHandlerExtended& handler_extended) override {
    Handle(route, handler_extended, constants::Methods::kOptions);
  }
  //! @brief Adds an string-based GET route handler to the server
  //! @param[in] route The route path to add
  //! @param[in] handler The function performing the logic
  //! @throws std::logic_error If user tries to add handlers while server is up
  //! and running
  //! @remarks <em>RoutingHandler</em> is just an alias for
  //! std::function<void(const Request&, Response&)><br>
  //! @section Example
  //! @snippet network/http_server_handler_shortcut.cpp GET
  void Get(const std::string& route, const RoutingHandler& handler) override {
    Handle(route, handler, constants::Methods::kGet);
  }
  //! @brief Adds an string-based GET route extended handler to the server
  //! @param[in] route The route path to add
  //! @param[in] handler The extended function performing the logic
  //! @throws std::logic_error If user tries to add handlers while server is up
  //! and running
  //! @remarks <em>RoutingHandlerExtended</em> is just an alias for
  //! std::function<void(const Request&, Response&, const Parameters&)><br>
  //! <em>Parameters</em> is just an alias for std::unordered_map<std::string,
  //! std::string>
  //! @section Example
  //! @snippet network/http_server_handler_shortcut.cpp GET-EXTENDED
  void Get(const std::string& route,
           const RoutingHandlerExtended& handler_extended) override {
    Handle(route, handler_extended, constants::Methods::kGet);
  }
  //! @brief Adds an string-based HEAD route handler to the server
  //! @param[in] route The route path to add
  //! @param[in] handler The function performing the logic
  //! @throws std::logic_error If user tries to add handlers while server is up
  //! and running
  //! @remarks <em>RoutingHandler</em> is just an alias for
  //! std::function<void(const Request&, Response&)><br>
  //! @section Example
  //! @snippet network/http_server_handler_shortcut.cpp HEAD
  void Head(const std::string& route, const RoutingHandler& handler) override {
    Handle(route, handler, constants::Methods::kHead);
  }
  //! @brief Adds an string-based HEAD route extended handler to the server
  //! @param[in] route The route path to add
  //! @param[in] handler The extended function performing the logic
  //! @throws std::logic_error If user tries to add handlers while server is up
  //! and running
  //! @remarks <em>RoutingHandlerExtended</em> is just an alias for
  //! std::function<void(const Request&, Response&, const Parameters&)><br>
  //! <em>Parameters</em> is just an alias for std::unordered_map<std::string,
  //! std::string>
  //! @section Example
  //! @snippet network/http_server_handler_shortcut.cpp HEAD-EXTENDED
  void Head(const std::string& route,
            const RoutingHandlerExtended& handler_extended) override {
    Handle(route, handler_extended, constants::Methods::kHead);
  }
  //! @brief Adds an string-based POST route handler to the server
  //! @param[in] route The route path to add
  //! @param[in] handler The function performing the logic
  //! @throws std::logic_error If user tries to add handlers while server is up
  //! and running
  //! @remarks <em>RoutingHandler</em> is just an alias for
  //! std::function<void(const Request&, Response&)><br>
  //! @section Example
  //! @snippet network/http_server_handler_shortcut.cpp POST
  void Post(const std::string& route, const RoutingHandler& handler) override {
    Handle(route, handler, constants::Methods::kPost);
  }
  //! @brief Adds an string-based POST route extended handler to the server
  //! @param[in] route The route path to add
  //! @param[in] handler The extended function performing the logic
  //! @throws std::logic_error If user tries to add handlers while server is up
  //! and running
  //! @remarks <em>RoutingHandlerExtended</em> is just an alias for
  //! std::function<void(const Request&, Response&, const Parameters&)><br>
  //! <em>Parameters</em> is just an alias for std::unordered_map<std::string,
  //! std::string>
  //! @section Example
  //! @snippet network/http_server_handler_shortcut.cpp POST-EXTENDED
  void Post(const std::string& route,
            const RoutingHandlerExtended& handler_extended) override {
    Handle(route, handler_extended, constants::Methods::kPost);
  }
  //! @brief Adds an string-based PUT route handler to the server
  //! @param[in] route The route path to add
  //! @param[in] handler The function performing the logic
  //! @throws std::logic_error If user tries to add handlers while server is up
  //! and running
  //! @remarks <em>RoutingHandler</em> is just an alias for
  //! std::function<void(const Request&, Response&)><br>
  //! @section Example
  //! @snippet network/http_server_handler_shortcut.cpp PUT
  void Put(const std::string& route, const RoutingHandler& handler) override {
    Handle(route, handler, constants::Methods::kPut);
  }
  //! @brief Adds an string-based PUT route extended handler to the server
  //! @param[in] route The route path to add
  //! @param[in] handler The extended function performing the logic
  //! @throws std::logic_error If user tries to add handlers while server is up
  //! and running
  //! @remarks <em>RoutingHandlerExtended</em> is just an alias for
  //! std::function<void(const Request&, Response&, const Parameters&)><br>
  //! <em>Parameters</em> is just an alias for std::unordered_map<std::string,
  //! std::string>
  //! @section Example
  //! @snippet network/http_server_handler_shortcut.cpp PUT-EXTENDED
  void Put(const std::string& route,
           const RoutingHandlerExtended& handler_extended) override {
    Handle(route, handler_extended, constants::Methods::kPut);
  }
  //! @brief Adds an string-based DELETE route handler to the server
  //! @param[in] route The route path to add
  //! @param[in] handler The function performing the logic
  //! @throws std::logic_error If user tries to add handlers while server is up
  //! and running
  //! @remarks <em>RoutingHandler</em> is just an alias for
  //! std::function<void(const Request&, Response&)><br>
  //! @section Example
  //! @snippet network/http_server_handler_shortcut.cpp DELETE
  void Delete(const std::string& route,
              const RoutingHandler& handler) override {
    Handle(route, handler, constants::Methods::kDelete);
  }
  //! @brief Adds an string-based DELETE route extended handler to the server
  //! @param[in] route The route path to add
  //! @param[in] handler The extended function performing the logic
  //! @throws std::logic_error If user tries to add handlers while server is up
  //! and running
  //! @remarks <em>RoutingHandlerExtended</em> is just an alias for
  //! std::function<void(const Request&, Response&, const Parameters&)><br>
  //! <em>Parameters</em> is just an alias for std::unordered_map<std::string,
  //! std::string>
  //! @section Example
  //! @snippet network/http_server_handler_shortcut.cpp DELETE-EXTENDED
  void Delete(const std::string& route,
              const RoutingHandlerExtended& handler_extended) override {
    Handle(route, handler_extended, constants::Methods::kDelete);
  }
  //! @brief Adds an string-based TRACE route handler to the server
  //! @param[in] route The route path to add
  //! @param[in] handler The function performing the logic
  //! @throws std::logic_error If user tries to add handlers while server is up
  //! and running
  //! @remarks <em>RoutingHandler</em> is just an alias for
  //! std::function<void(const Request&, Response&)><br>
  //! @section Example
  //! @snippet network/http_server_handler_shortcut.cpp TRACE
  void Trace(const std::string& route, const RoutingHandler& handler) override {
    Handle(route, handler, constants::Methods::kTrace);
  }
  //! @brief Adds an string-based TRACE route extended handler to the server
  //! @param[in] route The route path to add
  //! @param[in] handler The extended function performing the logic
  //! @throws std::logic_error If user tries to add handlers while server is up
  //! and running
  //! @remarks <em>RoutingHandlerExtended</em> is just an alias for
  //! std::function<void(const Request&, Response&, const Parameters&)><br>
  //! <em>Parameters</em> is just an alias for std::unordered_map<std::string,
  //! std::string>
  //! @section Example
  //! @snippet network/http_server_handler_shortcut.cpp TRACE-EXTENDED
  void Trace(const std::string& route,
             const RoutingHandlerExtended& handler_extended) override {
    Handle(route, handler_extended, constants::Methods::kTrace);
  }
  //! @brief Adds an string-based CONNECT route handler to the server
  //! @param[in] route The route path to add
  //! @param[in] handler The function performing the logic
  //! @throws std::logic_error If user tries to add handlers while server is up
  //! and running
  //! @remarks <em>RoutingHandler</em> is just an alias for
  //! std::function<void(const Request&, Response&)><br>
  //! @section Example
  //! @snippet network/http_server_handler_shortcut.cpp CONNECT
  void Connect(const std::string& route,
               const RoutingHandler& handler) override {
    Handle(route, handler, constants::Methods::kConnect);
  }
  //! @brief Adds an string-based CONNECT route extended handler to the server
  //! @param[in] route The route path to add
  //! @param[in] handler The extended function performing the logic
  //! @throws std::logic_error If user tries to add handlers while server is up
  //! and running
  //! @remarks <em>RoutingHandlerExtended</em> is just an alias for
  //! std::function<void(const Request&, Response&, const Parameters&)><br>
  //! <em>Parameters</em> is just an alias for std::unordered_map<std::string,
  //! std::string>
  //! @section Example
  //! @snippet network/http_server_handler_shortcut.cpp CONNECT-EXTENDED
  void Connect(const std::string& route,
               const RoutingHandlerExtended& handler_extended) override {
    Handle(route, handler_extended, constants::Methods::kConnect);
  }
  //! @brief Returns currently stored server local datetime
  //! @return The currently stored local datetime 
  //! @section Example
  //! @snippet network/http_server_hello_world.cpp GetCurrentDate
  std::string GetCurrentDate() const {
    std::unique_lock<std::mutex> sync_protection(date_buffer_mutex);
    return std::string(date_buffer_);
  }

 private:
  // Updates current stored server date.
  void updateInternalDate() {
    std::unique_lock<std::mutex> sync_protection(date_buffer_mutex);
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
  structured::json::Object configuration_;
  // Attributes needed for server-date management!
  char date_buffer_[kMaxLength] = {0};
  mutable std::mutex date_buffer_mutex;
  struct tm* timeinfo = nullptr;
  time_t rawtime;
};
}  // namespace koobika::hook::network::protocol::http

#endif