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
// network/protocol/http/http_router.h
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

#ifndef koobika_hook_network_protocol_http_httprouter_h
#define koobika_hook_network_protocol_http_httprouter_h

#include <functional>
#include <iostream>
#include <mutex>
#include <optional>
#include <regex>
#include <string>
#include <unordered_map>

#include "http_method.h"
#include "http_routes_manager.h"
#include "http_routes_node.h"
#include "http_routes_performer.h"

namespace koobika::hook::network::protocol::http {
// =============================================================================
// HttpRouter                                                          ( class )
// -----------------------------------------------------------------------------
// This class is in charge of providing the default http router class
// =============================================================================
class HttpRouter : public HttpRoutesManager, public HttpRoutesPerformer {
 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  // 
  HttpRouter() = default;
  HttpRouter(const HttpRouter&) = delete;
  HttpRouter(HttpRouter&&) noexcept = delete;
  ~HttpRouter() = default;
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  // 
  HttpRouter& operator=(const HttpRouter&) = delete;
  HttpRouter& operator=(HttpRouter&&) noexcept = delete;
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  // 
  // Adds a new <generic> handler to the internal map using an string route.
  void Handle(
      const std::string& route,
      const typename HttpRoutesTypes::Handler& handler,
      const HttpMethodValue& method = HttpConstants::Methods::kAll) override {
    std::unique_lock sync(map_mutex_);
    map_[route] = HttpRoutesNode(handler, method);
  }
  // Adds a new <generic> handler to the internal map using an regex route.
  void Handle(
      const std::regex& regex, const typename HttpRoutesTypes::Handler& handler,
      const HttpMethodValue& method = HttpConstants ::Methods::kAll) override {
    std::unique_lock sync(vec_mutex_);
    vec_.push_back(std::make_pair(regex, HttpRoutesNode(handler, method)));
  }
  // Adds a new <options> handler to 'string-guided' router structures.
  void Options(const std::string& route,
               const typename HttpRoutesTypes::Handler& handler) override {
    Handle(route, handler, HttpConstants::Methods::kOptions);
  }
  // Adds a new <options> handler to 'regex-guided' router structures.
  void Options(const std::regex& regex,
               const typename HttpRoutesTypes::Handler& handler) override {
    Handle(regex, handler, HttpConstants::Methods::kOptions);
  }
  // Adds a new <get> handler to 'string-guided' router structures.
  void Get(const std::string& route,
           const typename HttpRoutesTypes::Handler& handler) override {
    Handle(route, handler, HttpConstants::Methods::kGet);
  }
  // Adds a new <get> handler to 'regex-guided' router structures.
  void Get(const std::regex& regex,
           const typename HttpRoutesTypes::Handler& handler) override {
    Handle(regex, handler, HttpConstants::Methods::kGet);
  }
  // Adds a new <head> handler to 'string-guided' router structures.
  void Head(const std::string& route,
            const typename HttpRoutesTypes::Handler& handler) override {
    Handle(route, handler, HttpConstants::Methods::kHead);
  }
  // Adds a new <head> handler to 'regex-guided' router structures.
  void Head(const std::regex& regex,
            const typename HttpRoutesTypes::Handler& handler) override {
    Handle(regex, handler, HttpConstants::Methods::kHead);
  }
  // Adds a new <post> handler to 'string-guided' router structures.
  void Post(const std::string& route,
            const typename HttpRoutesTypes::Handler& handler) override {
    Handle(route, handler, HttpConstants::Methods::kPost);
  }
  // Adds a new <post> handler to 'regex-guided' router structures.
  void Post(const std::regex& regex,
            const typename HttpRoutesTypes::Handler& handler) override {
    Handle(regex, handler, HttpConstants::Methods::kPost);
  }
  // Adds a new <put> handler to 'string-guided' router structures.
  void Put(const std::string& route,
           const typename HttpRoutesTypes::Handler& handler) override {
    Handle(route, handler, HttpConstants::Methods::kPut);
  }
  // Adds a new <put> handler to 'regex-guided' router structures.
  void Put(const std::regex& regex,
           const typename HttpRoutesTypes::Handler& handler) override {
    Handle(regex, handler, HttpConstants::Methods::kPut);
  }
  // Adds a new <delete> handler to 'string-guided' router structures.
  void Delete(const std::string& route,
              const typename HttpRoutesTypes::Handler& handler) override {
    Handle(route, handler, HttpConstants::Methods::kDelete);
  }
  // Adds a new <delete> handler to 'regex-guided' router structures.
  void Delete(const std::regex& regex,
              const typename HttpRoutesTypes::Handler& handler) override {
    Handle(regex, handler, HttpConstants::Methods::kDelete);
  }
  // Adds a new <trace> handler to 'string-guided' router structures.
  void Trace(const std::string& route,
             const typename HttpRoutesTypes::Handler& handler) override {
    Handle(route, handler, HttpConstants::Methods::kTrace);
  }
  // Adds a new <trace> handler to 'regex-guided' router structures.
  void Trace(const std::regex& route,
             const typename HttpRoutesTypes::Handler& handler) override {
    Handle(route, handler, HttpConstants::Methods::kTrace);
  }
  // Adds a new <connect> handler to 'string-guided' router structures.
  void Connect(const std::string& route,
               const typename HttpRoutesTypes::Handler& handler) override {
    Handle(route, handler, HttpConstants::Methods::kConnect);
  }
  // Adds a new <connect> handler to 'regex-guided' router structures.
  void Connect(const std::regex& regex,
               const typename HttpRoutesTypes::Handler& handler) override {
    Handle(regex, handler, HttpConstants::Methods::kConnect);
  }
  // Tries to perform router enabled action.
  HttpRoutesPerformerResult Perform(
      const std::string& route, typename HttpRoutesTypes::Request request,
      typename HttpRoutesTypes::Response response) const override {
    std::optional<HttpRoutesNode> value = MatchStringRoute(route);
    if (!value.has_value()) value = MatchRegExRoute(route);
    if (value.has_value() && value->handler &&
        request.Method.GetCode() & value->method) {
      value->handler(request, response);
      return HttpRoutesPerformerResult::kOk;
    }
    return HttpRoutesPerformerResult::kNotFound;
  }

 protected:
  // ___________________________________________________________________________
  // METHODs                                                       ( protected )
  // 
  // Searchs for the specified string route and returns the associated node.
  inline std::optional<HttpRoutesNode> MatchStringRoute(
      const std::string& route) const {
    std::lock_guard<std::mutex> sync{map_mutex_};
    auto const& itr = map_.find(route);
    return (itr != map_.end()) ? itr->second : std::optional<HttpRoutesNode>{};
  }
  // Searchs for the specified regex route and returns the associated node.
  inline std::optional<HttpRoutesNode> MatchRegExRoute(
      const std::string& route) const {
    std::lock_guard<std::mutex> sync{vec_mutex_};
    for (auto& re : vec_) {
      std::sregex_iterator next(route.begin(), route.end(), re.first);
      if (next != std::sregex_iterator()) {
        return re.second;
      }
    }
    return {};
  }
  // ___________________________________________________________________________
  // ATTRIBUTEs                                                    ( protected )
  // 
  std::unordered_map<std::string, HttpRoutesNode> map_;
  std::vector<std::pair<std::regex, HttpRoutesNode>> vec_;
  mutable std::mutex map_mutex_;
  mutable std::mutex vec_mutex_;
};
}  // namespace koobika::hook::network::protocol::http

#endif