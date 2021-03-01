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

#ifndef koobika_hook_network_protocol_http_v11_httprouter_h
#define koobika_hook_network_protocol_http_v11_httprouter_h

#include <functional>
#include <iostream>
#include <mutex>
#include <optional>
#include <regex>
#include <string>
#include <unordered_map>

#include "http_auth_module.h"
#include "http_auth_support.h"
#include "http_method.h"
#include "http_routes_manager.h"
#include "http_routes_performer.h"

namespace koobika::hook::network::protocol::http::v11 {
// =============================================================================
// HttpRouter                                                          [ class ]
// -----------------------------------------------------------------------------
// This class is in charge of providing the default http router class
// -----------------------------------------------------------------------------
// Template parameters:
//    RQty - request type being used
//    RSty - response type being used
// =============================================================================
template <typename RQty, typename RSty>
class HttpRouter : public HttpRoutesManager<RQty, RSty>,
                   public HttpRoutesPerformer<RQty, RSty> {
  // ---------------------------------------------------------------------------
  // Types                                                           [ private ]
  // ---------------------------------------------------------------------------
  struct Value {
    Value() = default;
    Value(const typename HttpRoutes<RQty, RSty>::RouteHandler& hdl,
          const HttpMethodValue& mth, const HttpAuthSupport& aut)
        : handler{hdl}, methods{mth}, auth{aut} {}
    typename HttpRoutes<RQty, RSty>::RouteHandler handler;
    HttpMethodValue methods = HttpConstants::Methods::kExtension;
    HttpAuthSupport auth = HttpAuthSupport::kDisabled;
  };

 public:
  // ---------------------------------------------------------------------------
  // Constructors/Destructors                                         [ public ]
  // ---------------------------------------------------------------------------
  HttpRouter() = default;
  HttpRouter(const HttpRouter&) = delete;
  HttpRouter(HttpRouter&&) noexcept = delete;
  ~HttpRouter() = default;
  // ---------------------------------------------------------------------------
  // Operators                                                        [ public ]
  // ---------------------------------------------------------------------------
  HttpRouter& operator=(const HttpRouter&) = delete;
  HttpRouter& operator=(HttpRouter&&) noexcept = delete;
  // ---------------------------------------------------------------------------
  // Methods                                                          [ public ]
  // ---------------------------------------------------------------------------
  // Adds a new <generic> handler to 'nominal' router structures
  void Handle(
      const std::string& route,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
      const HttpMethodValue& methods = HttpMethod::kAll,
      const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    std::unique_lock sync(map_mutex_);
    map_[route] = Value(handler, methods, auth);
  }
  // Adds a new <generic> handler to 'regex' router structures
  void Handle(
      const std::regex& regex,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
      const HttpMethodValue& methods = HttpMethod::kAll,
      const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    std::unique_lock sync(vec_mutex_);
    vec_.push_back(std::make_pair(regex, Value(handler, methods, auth)));
  }
  // Adds a new <options> handler to 'nominal' router structures
  void Options(
      const std::string& route,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
      const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(route, handler, HttpConstants::Methods::kOptions, auth);
  }
  // Adds a new <options> handler to 'regex' router structures
  void Options(
      const std::regex& regex,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
      const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(regex, handler, HttpConstants::Methods::kOptions, auth);
  }
  // Adds a new <get> handler to 'nominal' router structures
  void Get(const std::string& route,
           const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
           const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(route, handler, HttpConstants::Methods::kGet, auth);
  }
  // Adds a new <get> handler to 'regex' router structures
  void Get(const std::regex& regex,
           const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
           const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(regex, handler, HttpConstants::Methods::kGet, auth);
  }
  // Adds a new <head> handler to 'nominal' router structures
  void Head(const std::string& route,
            const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
            const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(route, handler, HttpConstants::Methods::kHead, auth);
  }
  // Adds a new <head> handler to 'regex' router structures
  void Head(const std::regex& regex,
            const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
            const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(regex, handler, HttpConstants::Methods::kHead, auth);
  }
  // Adds a new <post> handler to 'nominal' router structures
  void Post(const std::string& route,
            const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
            const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(route, handler, HttpConstants::Methods::kPost, auth);
  }
  // Adds a new <post> handler to 'regex' router structures
  void Post(const std::regex& regex,
            const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
            const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(regex, handler, HttpConstants::Methods::kPost, auth);
  }
  // Adds a new <put> handler to 'nominal' router structures
  void Put(const std::string& route,
           const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
           const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(route, handler, HttpConstants::Methods::kPut, auth);
  }
  // Adds a new <put> handler to 'regex' router structures
  void Put(const std::regex& regex,
           const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
           const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(regex, handler, HttpConstants::Methods::kPut, auth);
  }
  // Adds a new <delete> handler to 'nominal' router structures
  void Delete(
      const std::string& route,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
      const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(route, handler, HttpConstants::Methods::kDelete, auth);
  }
  // Adds a new <delete> handler to 'regex' router structures
  void Delete(
      const std::regex& regex,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
      const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(regex, handler, HttpConstants::Methods::kDelete, auth);
  }
  // Adds a new <trace> handler to 'nominal' router structures
  void Trace(
      const std::string& route,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
      const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(route, handler, HttpConstants::Methods::kTrace, auth);
  }
  // Adds a new <trace> handler to 'regex' router structures
  void Trace(
      const std::regex& regex,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
      const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(regex, handler, HttpConstants::Methods::kTrace, auth);
  }
  // Adds a new <connect> handler to 'nominal' router structures
  void Connect(
      const std::string& route,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
      const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(route, handler, HttpConstants::Methods::kConnect, auth);
  }
  // Adds a new <connect> handler to 'regex' router structures
  void Connect(
      const std::regex& regex,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& handler,
      const HttpAuthSupport& auth = HttpAuthSupport::kDisabled) override {
    Handle(regex, handler, HttpConstants::Methods::kConnect, auth);
  }
  // Tries to perform router enabled action
  bool Perform(
      const std::string& route, typename HttpRoutes<RQty, RSty>::Request req,
      typename HttpRoutes<RQty, RSty>::Response res,
      const std::shared_ptr<HttpAuthModule<RQty, RSty>>& auth) const override {
    std::optional<Value> value;
    do {
      {
        std::unique_lock sync(map_mutex_);
        auto const& itr = map_.find(route);
        if (itr != map_.end()) {
          value = itr->second;
          break;
        }
      }
      {
        std::unique_lock sync(vec_mutex_);
        for (auto& re : vec_) {
          std::sregex_iterator next(route.begin(), route.end(), re.first);
          if (next != std::sregex_iterator()) {
            value = re.second;
            break;
          }
        }
      }
    } while (false);
    if (value.has_value()) {
      if (value->handler != nullptr &&
          (req.Method.GetCode() & value->methods)) {
        bool authorized = true;
        if (value->auth == HttpAuthSupport::kEnabled) {
          auto checker = auth->GetChecker();
          if (auth != nullptr && checker != nullptr) {
            authorized = checker(req, res);
          }
        }
        if (value->auth == HttpAuthSupport::kReserved || authorized) {
          value->handler(req, res);
          return true;
        } else {
          res.Unauthorized_401();
        }
      }
    }
    return false;
  }

 private:
  // ---------------------------------------------------------------------------
  // Attributes                                                      [ private ]
  // ---------------------------------------------------------------------------
  std::unordered_map<std::string, Value> map_;
  std::vector<std::pair<std::regex, Value>> vec_;
  mutable std::mutex map_mutex_;
  mutable std::mutex vec_mutex_;
};
}  // namespace koobika::hook::network::protocol::http::v11

#endif