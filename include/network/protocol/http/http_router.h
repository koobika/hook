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

#include "constants/methods.h"
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
      const HttpMethodValue& method = constants::Methods::kAll) override {
    std::unique_lock sync(routes_mutex_);
    routes_.push_back(std::make_pair(route, HttpRoutesNode(handler, method)));
  }
  // Adds a new <generic> handler to the internal map using an regex route.
  void Handle(
      const std::regex& regex, const typename HttpRoutesTypes::Handler& handler,
      const HttpMethodValue& method = constants ::Methods::kAll) override {
    std::unique_lock sync(routes_mutex_);
    routes_.push_back(std::make_pair(regex, HttpRoutesNode(handler, method)));
  }
  // Adds a new <options> handler to 'string-guided' router structures.
  void Options(const std::string& route,
               const typename HttpRoutesTypes::Handler& handler) override {
    Handle(route, handler, constants::Methods::kOptions);
  }
  // Adds a new <options> handler to 'regex-guided' router structures.
  void Options(const std::regex& regex,
               const typename HttpRoutesTypes::Handler& handler) override {
    Handle(regex, handler, constants::Methods::kOptions);
  }
  // Adds a new <get> handler to 'string-guided' router structures.
  void Get(const std::string& route,
           const typename HttpRoutesTypes::Handler& handler) override {
    Handle(route, handler, constants::Methods::kGet);
  }
  // Adds a new <get> handler to 'regex-guided' router structures.
  void Get(const std::regex& regex,
           const typename HttpRoutesTypes::Handler& handler) override {
    Handle(regex, handler, constants::Methods::kGet);
  }
  // Adds a new <head> handler to 'string-guided' router structures.
  void Head(const std::string& route,
            const typename HttpRoutesTypes::Handler& handler) override {
    Handle(route, handler, constants::Methods::kHead);
  }
  // Adds a new <head> handler to 'regex-guided' router structures.
  void Head(const std::regex& regex,
            const typename HttpRoutesTypes::Handler& handler) override {
    Handle(regex, handler, constants::Methods::kHead);
  }
  // Adds a new <post> handler to 'string-guided' router structures.
  void Post(const std::string& route,
            const typename HttpRoutesTypes::Handler& handler) override {
    Handle(route, handler, constants::Methods::kPost);
  }
  // Adds a new <post> handler to 'regex-guided' router structures.
  void Post(const std::regex& regex,
            const typename HttpRoutesTypes::Handler& handler) override {
    Handle(regex, handler, constants::Methods::kPost);
  }
  // Adds a new <put> handler to 'string-guided' router structures.
  void Put(const std::string& route,
           const typename HttpRoutesTypes::Handler& handler) override {
    Handle(route, handler, constants::Methods::kPut);
  }
  // Adds a new <put> handler to 'regex-guided' router structures.
  void Put(const std::regex& regex,
           const typename HttpRoutesTypes::Handler& handler) override {
    Handle(regex, handler, constants::Methods::kPut);
  }
  // Adds a new <delete> handler to 'string-guided' router structures.
  void Delete(const std::string& route,
              const typename HttpRoutesTypes::Handler& handler) override {
    Handle(route, handler, constants::Methods::kDelete);
  }
  // Adds a new <delete> handler to 'regex-guided' router structures.
  void Delete(const std::regex& regex,
              const typename HttpRoutesTypes::Handler& handler) override {
    Handle(regex, handler, constants::Methods::kDelete);
  }
  // Adds a new <trace> handler to 'string-guided' router structures.
  void Trace(const std::string& route,
             const typename HttpRoutesTypes::Handler& handler) override {
    Handle(route, handler, constants::Methods::kTrace);
  }
  // Adds a new <trace> handler to 'regex-guided' router structures.
  void Trace(const std::regex& route,
             const typename HttpRoutesTypes::Handler& handler) override {
    Handle(route, handler, constants::Methods::kTrace);
  }
  // Adds a new <connect> handler to 'string-guided' router structures.
  void Connect(const std::string& route,
               const typename HttpRoutesTypes::Handler& handler) override {
    Handle(route, handler, constants::Methods::kConnect);
  }
  // Adds a new <connect> handler to 'regex-guided' router structures.
  void Connect(const std::regex& regex,
               const typename HttpRoutesTypes::Handler& handler) override {
    Handle(regex, handler, constants::Methods::kConnect);
  }
  // Tries to perform router enabled action.
  bool Perform(const std::string& route,
               typename HttpRoutesTypes::Request request,
               typename HttpRoutesTypes::Response response) const override {
    HttpRoutesNode node;
    std::size_t last_matched_length = 0;
    bool string_based_already_matched = false;
    routes_mutex_.lock();
    for (auto const& r : routes_) {
      if (r.first.index() == 0) {
        auto stored_route = std::get<std::string>(r.first);
        auto position = route.find(stored_route);
        if (!position) {
          auto stored_length = stored_route.length();
          if (stored_length >= last_matched_length) {
            string_based_already_matched = true;
            last_matched_length = stored_length;
            node = r.second;
          }
        }
      } else {
        std::sregex_iterator next(route.begin(), route.end(),
                                  std::get<std::regex>(r.first));
        if (next != std::sregex_iterator() && !string_based_already_matched) {
          node = r.second;
        }
      }
    }
    routes_mutex_.unlock();
    if (node.handler == nullptr || !(node.method & request.Method.GetCode())) {
      return false;
    }
    node.handler(request, response);
    return true;
  }

 protected:
  // ___________________________________________________________________________
  // USINGs                                                         ( protected
  // )
  //
  using First = std::variant<std::string, std::regex>;
  using Second = HttpRoutesNode;
  // ___________________________________________________________________________
  // ATTRIBUTEs                                                    ( protected )
  //
  std::vector<std::pair<First, Second>> routes_;
  mutable std::mutex routes_mutex_;
};
}  // namespace koobika::hook::network::protocol::http

#endif