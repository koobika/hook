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

#include "http_auth_support.h"
#include "http_method.h"
#include "http_routes_manager.h"
#include "http_routes_performer.h"

namespace koobika::hook::network::protocol::http::v11 {
// =============================================================================
// HttpRouter                                                          ( class )
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
  // TYPEs                                                           ( private )
  // ---------------------------------------------------------------------------
  struct Value {
    Value() = default;
    Value(const typename HttpRoutes<RQty, RSty>::RouteHandler& hdl,
          const HttpMethodValue& mth, const HttpAuthSupport& aut)
        : route_handler{hdl}, methods{mth}, auth_support{aut} {}
    typename HttpRoutes<RQty, RSty>::RouteHandler route_handler;
    HttpMethodValue methods = HttpConstants::Methods::kExtension;
    HttpAuthSupport auth_support = HttpAuthSupport::kDisabled;
  };

 public:
  // ---------------------------------------------------------------------------
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  // ---------------------------------------------------------------------------
  HttpRouter() = default;
  HttpRouter(const HttpRouter&) = delete;
  HttpRouter(HttpRouter&&) noexcept = delete;
  ~HttpRouter() = default;
  // ---------------------------------------------------------------------------
  // OPERATORs                                                        ( public )
  // ---------------------------------------------------------------------------
  HttpRouter& operator=(const HttpRouter&) = delete;
  HttpRouter& operator=(HttpRouter&&) noexcept = delete;
  // ---------------------------------------------------------------------------
  // METHODs                                                          ( public )
  // ---------------------------------------------------------------------------
  // Adds a new <generic> handler to the internal map using an string route.
  void Handle(
      const std::string& route,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
      const HttpMethodValue& method = HttpMethod::kAll,
      const HttpAuthSupport& auth_support =
          HttpAuthSupport::kDisabled) override {
    std::unique_lock sync(map_mutex_);
    map_[route] = Value(route_handler, method, auth_support);
  }
  // Adds a new <generic> handler to the internal map using an regex route.
  void Handle(
      const std::regex& regex,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
      const HttpMethodValue& method = HttpMethod::kAll,
      const HttpAuthSupport& auth_support =
          HttpAuthSupport::kDisabled) override {
    std::unique_lock sync(vec_mutex_);
    vec_.push_back(
        std::make_pair(regex, Value(route_handler, method, auth_support)));
  }
  // Adds a new <options> handler to 'string-guided' router structures.
  void Options(
      const std::string& route,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
      const HttpAuthSupport& auth_support =
          HttpAuthSupport::kDisabled) override {
    Handle(route, route_handler, HttpConstants::Methods::kOptions,
           auth_support);
  }
  // Adds a new <options> handler to 'regex-guided' router structures.
  void Options(
      const std::regex& regex,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
      const HttpAuthSupport& auth_support =
          HttpAuthSupport::kDisabled) override {
    Handle(regex, route_handler, HttpConstants::Methods::kOptions,
           auth_support);
  }
  // Adds a new <get> handler to 'string-guided' router structures.
  void Get(const std::string& route,
           const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
           const HttpAuthSupport& auth_support =
               HttpAuthSupport::kDisabled) override {
    Handle(route, route_handler, HttpConstants::Methods::kGet, auth_support);
  }
  // Adds a new <get> handler to 'regex-guided' router structures.
  void Get(const std::regex& regex,
           const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
           const HttpAuthSupport& auth_support =
               HttpAuthSupport::kDisabled) override {
    Handle(regex, route_handler, HttpConstants::Methods::kGet, auth_support);
  }
  // Adds a new <head> handler to 'string-guided' router structures.
  void Head(const std::string& route,
            const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
            const HttpAuthSupport& auth_support =
                HttpAuthSupport::kDisabled) override {
    Handle(route, route_handler, HttpConstants::Methods::kHead, auth_support);
  }
  // Adds a new <head> handler to 'regex-guided' router structures.
  void Head(const std::regex& regex,
            const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
            const HttpAuthSupport& auth_support =
                HttpAuthSupport::kDisabled) override {
    Handle(regex, route_handler, HttpConstants::Methods::kHead, auth_support);
  }
  // Adds a new <post> handler to 'string-guided' router structures.
  void Post(const std::string& route,
            const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
            const HttpAuthSupport& auth_support =
                HttpAuthSupport::kDisabled) override {
    Handle(route, route_handler, HttpConstants::Methods::kPost, auth_support);
  }
  // Adds a new <post> handler to 'regex-guided' router structures.
  void Post(const std::regex& regex,
            const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
            const HttpAuthSupport& auth_support =
                HttpAuthSupport::kDisabled) override {
    Handle(regex, route_handler, HttpConstants::Methods::kPost, auth_support);
  }
  // Adds a new <put> handler to 'string-guided' router structures.
  void Put(const std::string& route,
           const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
           const HttpAuthSupport& auth_support =
               HttpAuthSupport::kDisabled) override {
    Handle(route, route_handler, HttpConstants::Methods::kPut, auth_support);
  }
  // Adds a new <put> handler to 'regex-guided' router structures.
  void Put(const std::regex& regex,
           const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
           const HttpAuthSupport& auth_support =
               HttpAuthSupport::kDisabled) override {
    Handle(regex, route_handler, HttpConstants::Methods::kPut, auth_support);
  }
  // Adds a new <delete> handler to 'string-guided' router structures.
  void Delete(
      const std::string& route,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
      const HttpAuthSupport& auth_support =
          HttpAuthSupport::kDisabled) override {
    Handle(route, route_handler, HttpConstants::Methods::kDelete, auth_support);
  }
  // Adds a new <delete> handler to 'regex-guided' router structures.
  void Delete(
      const std::regex& regex,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
      const HttpAuthSupport& auth_support =
          HttpAuthSupport::kDisabled) override {
    Handle(regex, route_handler, HttpConstants::Methods::kDelete, auth_support);
  }
  // Adds a new <trace> handler to 'string-guided' router structures.
  void Trace(const std::string& route,
             const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
             const HttpAuthSupport& auth_support =
                 HttpAuthSupport::kDisabled) override {
    Handle(route, route_handler, HttpConstants::Methods::kTrace, auth_support);
  }
  // Adds a new <trace> handler to 'regex-guided' router structures.
  void Trace(const std::regex& route,
             const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
             const HttpAuthSupport& auth_support =
                 HttpAuthSupport::kDisabled) override {
    Handle(route, route_handler, HttpConstants::Methods::kTrace, auth_support);
  }
  // Adds a new <connect> handler to 'string-guided' router structures.
  void Connect(
      const std::string& route,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
      const HttpAuthSupport& auth_support =
          HttpAuthSupport::kDisabled) override {
    Handle(route, route_handler, HttpConstants::Methods::kConnect,
           auth_support);
  }
  // Adds a new <connect> handler to 'regex-guided' router structures.
  void Connect(
      const std::regex& regex,
      const typename HttpRoutes<RQty, RSty>::RouteHandler& route_handler,
      const HttpAuthSupport& auth_support =
          HttpAuthSupport::kDisabled) override {
    Handle(regex, route_handler, HttpConstants::Methods::kConnect,
           auth_support);
  }
  // Tries to perform router enabled action.
  bool Perform(const std::string& route,
               typename HttpRoutes<RQty, RSty>::Request request,
               typename HttpRoutes<RQty, RSty>::Response response,
               const std::shared_ptr<
                   HttpAuthModule<typename HttpRoutes<RQty, RSty>::Request,
                                  typename HttpRoutes<RQty, RSty>::Response>>&
                   auth_module) const override {
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
      if (value->route_handler != nullptr &&
          (request.Method.GetCode() & value->methods)) {
        bool authorized = true;
        if (value->auth_support == HttpAuthSupport::kEnabled &&
            auth_module != nullptr) {
          auto checker = auth_module->GetChecker();
          if (checker != nullptr) {
            authorized = checker(request, response);
          }
        }
        if (authorized) {
          value->route_handler(request, response);
          return true;
        } else {
          response.Unauthorized_401();
        }
      }
    }
    return false;
  }

 private:
  // ---------------------------------------------------------------------------
  // ATTRIBUTEs                                                      ( private )
  // ---------------------------------------------------------------------------
  std::unordered_map<std::string, Value> map_;
  std::vector<std::pair<std::regex, Value>> vec_;
  mutable std::mutex map_mutex_;
  mutable std::mutex vec_mutex_;
};
}  // namespace koobika::hook::network::protocol::http::v11

#endif