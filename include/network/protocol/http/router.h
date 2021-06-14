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
// network/protocol/http/router.h
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

#ifndef koobika_hook_network_protocol_http_router_h
#define koobika_hook_network_protocol_http_router_h

#include <functional>
#include <iostream>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>
#include <deque>

#include "constants/methods.h"
#include "network/transport/server_constants.h"
#include "method_helper.h"
#include "routes_manager.h"
#include "routes_node.h"
#include "routes_performer.h"
#include "parameters.h"

namespace koobika::hook::network::protocol::http {
// =============================================================================
// Router                                                              ( class )
// -----------------------------------------------------------------------------
// This class is in charge of providing the default http router class.
// =============================================================================
class Router : public RoutesManager, public RoutesPerformer {
 protected:
  // ___________________________________________________________________________
  // TYPEs                                                         ( protected )
  //
  struct NodeData {
    NodeData() = default;
    std::pair<std::string, std::shared_ptr<NodeData>> parameter;
    std::unordered_map<std::string, std::shared_ptr<NodeData>> child_nodes;
    std::unordered_map<MethodValue,
                       std::variant<RoutingHandler, RoutingHandlerExtended>>
        handlers;
  };

 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  Router() = default;
  Router(const Router&) = delete;
  Router(Router&&) noexcept = delete;
  ~Router() = default;
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  //
  Router& operator=(const Router&) = delete;
  Router& operator=(Router&&) noexcept = delete;
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  // Adds a new <generic> handler to the internal map using an string route.
  void Handle(const std::string& route, const RoutingHandler& handler,
              const MethodValue& method = constants::Methods::kAll) override {
    handle(route, handler, method);
  }
  // Adds a new <generic> handler to the internal map using an string route.
  void Handle(const std::string& route,
              const RoutingHandlerExtended& handler_extended,
              const MethodValue& method = constants::Methods::kAll) override {
    handle(route, handler_extended, method);
  }
  // Adds a new <options> handler to 'string-guided' router structures.
  void Options(const std::string& route,
               const RoutingHandler& handler) override {
    Handle(route, handler, constants::Methods::kOptions);
  }
  // Adds a new <options> handler to 'string-guided' router structures.
  void Options(const std::string& route,
               const RoutingHandlerExtended& handler_extended) override {
    Handle(route, handler_extended, constants::Methods::kOptions);
  }
  // Adds a new <get> handler to 'string-guided' router structures.
  void Get(const std::string& route, const RoutingHandler& handler) override {
    Handle(route, handler, constants::Methods::kGet);
  }
  // Adds a new <get> handler to 'string-guided' router structures.
  void Get(const std::string& route,
           const RoutingHandlerExtended& handler_extended) override {
    Handle(route, handler_extended, constants::Methods::kGet);
  }
  // Adds a new <head> handler to 'string-guided' router structures.
  void Head(const std::string& route, const RoutingHandler& handler) override {
    Handle(route, handler, constants::Methods::kHead);
  }
  // Adds a new <head> handler to 'string-guided' router structures.
  void Head(const std::string& route,
            const RoutingHandlerExtended& handler_extended) override {
    Handle(route, handler_extended, constants::Methods::kHead);
  }
  // Adds a new <post> handler to 'string-guided' router structures.
  void Post(const std::string& route, const RoutingHandler& handler) override {
    Handle(route, handler, constants::Methods::kPost);
  }
  // Adds a new <post> handler to 'string-guided' router structures.
  void Post(const std::string& route,
            const RoutingHandlerExtended& handler_extended) override {
    Handle(route, handler_extended, constants::Methods::kPost);
  }
  // Adds a new <put> handler to 'string-guided' router structures.
  void Put(const std::string& route, const RoutingHandler& handler) override {
    Handle(route, handler, constants::Methods::kPut);
  }
  // Adds a new <put> handler to 'string-guided' router structures.
  void Put(const std::string& route,
           const RoutingHandlerExtended& handler_extended) override {
    Handle(route, handler_extended, constants::Methods::kPut);
  }
  // Adds a new <delete> handler to 'string-guided' router structures.
  void Delete(const std::string& route,
              const RoutingHandler& handler) override {
    Handle(route, handler, constants::Methods::kDelete);
  }
  // Adds a new <delete> handler to 'string-guided' router structures.
  void Delete(const std::string& route,
              const RoutingHandlerExtended& handler_extended) override {
    Handle(route, handler_extended, constants::Methods::kDelete);
  }
  // Adds a new <trace> handler to 'string-guided' router structures.
  void Trace(const std::string& route, const RoutingHandler& handler) override {
    Handle(route, handler, constants::Methods::kTrace);
  }
  // Adds a new <trace> handler to 'string-guided' router structures.
  void Trace(const std::string& route,
             const RoutingHandlerExtended& handler_extended) override {
    Handle(route, handler_extended, constants::Methods::kTrace);
  }
  // Adds a new <connect> handler to 'string-guided' router structures.
  void Connect(const std::string& route,
               const RoutingHandler& handler) override {
    Handle(route, handler, constants::Methods::kConnect);
  }
  // Adds a new <connect> handler to 'string-guided' router structures.
  void Connect(const std::string& route,
               const RoutingHandlerExtended& handler_extended) override {
    Handle(route, handler_extended, constants::Methods::kConnect);
  }
  // Tries to perform router enabled action.
  bool Perform(const Request& req, Response& res) const override {
    if (!routes_.size()) return false;
    Parameters parameters;
    auto path = req.Uri.GetPath();
    auto ptr = path.c_str();
    auto end = ptr + req.Uri.GetPath().length();
    const char* cur = strchr(ptr, kSlash);
    std::shared_ptr<NodeData> node = routes_.begin()->second;
    while (cur++) {
      auto last = cur;
      cur = strchr(cur, kSlash);
      std::string value(last, !cur ? end - last : cur - last);
      if (node->parameter.second == nullptr) {
        if (!node->child_nodes.size()) {
          break;
        }
        auto found = node->child_nodes.find(value);
        if (found == node->child_nodes.end()) {
          return false;
        }
        node = found->second;
      } else {
        parameters.insert(std::make_pair(node->parameter.first, value));
        node = node->parameter.second;
      }
    }
    auto caller = node->handlers.find(constants::Methods::kAll);
    if (caller == node->handlers.end()) {
      caller = node->handlers.find(req.Method.GetCode());
      if (caller == node->handlers.end()) {
        return false;
      }
    }
    !caller->second.index() ? std::get<0>(caller->second)(req, res)
                            : std::get<1>(caller->second)(req, res, parameters);
    return true;
  }
  // Adds the incoming router configuration to the actual one!
  void Add(const Router& in) {
    if (!in.routes_.size()) return;
    std::deque<std::string> route;
    add(kSlashStr, in.routes_.begin()->second, route, true);
  }

 protected:
  // ___________________________________________________________________________
  // METHODs                                                       ( protected )
  //
  // Adds current routing data to the handler.
  template <typename HDty>
  void handle(const std::string& r, const HDty& h, const MethodValue& m) {
    auto itr = routes_.begin();
    if (itr == routes_.end()) {
      itr = routes_
                .insert(std::make_pair(kSlashStr, std::make_shared<NodeData>()))
                .first;
    }
    std::shared_ptr<NodeData> node = itr->second;
    char buffer[kMaxRouteLength] = {0};
    strncpy(buffer, r.c_str(), r.length());
    buffer[r.length()] = 0;
    auto pch = strtok(buffer, kSlashStr);
    std::string parameter_name;
    while (pch) {
      bool is_parameter = isParameter(pch, parameter_name);
      if (is_parameter) {
        node->parameter.first.assign(parameter_name);
        node->parameter.second = std::make_shared<NodeData>();
        node = node->parameter.second;
      } else {
        auto found = node->child_nodes.find(pch);
        if (found == node->child_nodes.end()) {
          node = node->child_nodes
                     .insert(std::make_pair(pch, std::make_shared<NodeData>()))
                     .first->second;
        } else {
          node = found->second;
        }
      }
      pch = strtok(NULL, kSlashStr);
    }
    node->handlers.insert(std::make_pair(m, h));
  }
  // Adds (recursively) incoming routing structure to internals!
  void add(const std::string& node_str,
           const std::shared_ptr<NodeData>& node_data,
           std::deque<std::string>& route, const bool& is_root = false) {
    route.push_back(node_str);
    if (!is_root) route.push_back(kSlashStr);
    for (auto const& child : node_data->child_nodes) {
      add(child.first, child.second, route);
    }
    std::string composed;
    for (auto const& node : route) {
      composed.append(node);
    }
    for (auto const& handler : node_data->handlers) {
      if (!handler.second.index()) {
        Handle(composed, std::get<0>(handler.second), handler.first);
      } else {
        Handle(composed, std::get<1>(handler.second), handler.first);
      }
    }
    route.pop_back();
    if (!is_root) route.pop_back();
  }
  // Checks for incoming string to be a parametrized one!
  bool isParameter(const char* str, std::string& parameter_name) const {
    auto start = strchr(str, kCurlyBraceStart);
    auto end = strchr(str, kCurlyBraceEnd);
    if (start++ && end) {
      parameter_name.assign(start, end - start);
      return true;
    }
    return false;
  }
  // ___________________________________________________________________________
  // CONSTANTs                                                     ( protected )
  //
  static constexpr char kSlash = '/';
  static constexpr char kCurlyBraceStart = '{';
  static constexpr char kCurlyBraceEnd = '}';
  static constexpr char kSlashStr[] = {kSlash, 0};
  static constexpr std::size_t kMaxRouteLength = 1024;
  // ___________________________________________________________________________
  // ATTRIBUTEs                                                    ( protected )
  //
  std::unordered_map<std::string, std::shared_ptr<NodeData>> routes_;
  // ___________________________________________________________________________
  // FRIENDs                                                       ( protected )
  //
  template <typename AUty>
  friend class RoutesController;
};
}  // namespace koobika::hook::network::protocol::http

#endif