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
#include <stack>
#include <stdexcept>

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
 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  Router() { buffer_ = new char[kBufferSize]; }
  Router(const Router&) = delete;
  Router(Router&&) noexcept = delete;
  ~Router() { delete[] buffer_; }
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
    RoutesNode node;
    Parameters parameters;
    {
      auto route = req.Uri.GetPath();
      auto length = route.length();
      strncpy(buffer_, route.c_str(), length);
      buffer_[length] = 0;
      auto pch = strtok(buffer_, kSlash_);
      auto current = routes_;
      while (current && pch != nullptr) {
        auto itr = current->nodes.find(pch);
        if (itr == current->nodes.end()) {
          // ok, we were not able to find a direct match! let's try to find a
          // parametrized '{}' node!
          auto itr_p = current->nodes.begin();
          while (itr_p != current->nodes.end()) {
            if (itr_p->second->type == ElementSrcType::kParametrized) {
              // Let's extract the 'parametrized' segment and check for uri..
              std::string key = itr_p->first, val = pch;
              auto s_pos = key.find_first_of(kBraceStart_);
              auto e_pos = key.find_first_of(kBraceEnd_, s_pos);
              auto param = key.substr(s_pos + 1, e_pos - s_pos - 1);
              key.erase(s_pos, e_pos - s_pos + 1);
              auto off = val.find(key);
              if (off != std::string::npos) {
                val.erase(val.find(key), key.length());
              }
              parameters.insert(std::make_pair(param, val));
              itr = itr_p;
              break;
            }
            itr_p++;
          }
          if (itr == current->nodes.end()) {
            break;
          }
        }
        current = itr->second;
        pch = strtok(NULL, kSlash_);
      }
      node = current->data;
    }
    if (node.method == constants::Methods::kInvalid ||
        !(node.method & req.Method.GetCode())) {
      return false;
    }
    node.handler ? node.handler(req, res)
                 : node.handler_extended(req, res, parameters);
    return true;
  }

 protected:
  // ___________________________________________________________________________
  // USINGs                                                        ( protected )
  //
  struct Element;
  using ElementsMap = std::unordered_map<std::string, std::shared_ptr<Element>>;
  // ___________________________________________________________________________
  // TYPEs                                                         ( protected )
  //
  enum class ElementSrcType { kNominal, kParametrized };
  struct Element {
    ElementsMap nodes;
    RoutesNode data;
    ElementSrcType type = ElementSrcType::kNominal;
  };
  // ___________________________________________________________________________
  // METHODs                                                       ( protected )
  //
  // Adds current routing data to the handler.
  template <typename HDty>
  void handle(const std::string& route, const HDty& handler,
              const MethodValue& method) {
    if (routes_ == nullptr) {
      routes_ = std::make_shared<Element>();
    }
    std::shared_ptr<Element> node = routes_;
    char tmp_buffer[512];
    auto len = route.length();
    strncpy(tmp_buffer, route.c_str(), len);
    tmp_buffer[len] = 0;
    auto pch = strtok(tmp_buffer, kSlash_);
    while (pch) {
      auto itr = node->nodes.find(pch);
      if (itr == node->nodes.end()) {
        // First, let's try to determine node type (parametrized/nominal)..
        auto data = std::make_pair(pch, std::make_shared<Element>());
        itr = node->nodes.insert(data).first;
        itr->second->type = getType(pch);
      }
      node = itr->second;
      pch = strtok(NULL, kSlash_);
    }
    node->data = RoutesNode(handler, method);
  }
  // Checks and return element-src-type.
  ElementSrcType getType(const char* str) {
    std::stack<char> braces;
    auto braces_found = 0;
    auto len = strlen(str);
    auto ptr = str;
    while (ptr < (str + len)) {
      switch (*ptr) {
        case kBraceStart_:
          if (braces.size()) {
            if (braces.top() != kBraceEnd_) {
              // ((Error)) -> invalid braces sequence detected!
              throw std::logic_error("Invalid braces sequence detected!");
            }
            braces.pop();
          }
          braces.push(kBraceStart_);
          break;
        case kBraceEnd_:
          if (braces.size()) {
            if (braces.top() != kBraceStart_) {
              // ((Error)) -> invalid braces sequence detected!
              throw std::logic_error("Invalid braces sequence detected!");
            }
            braces.pop();
          } else {
            // ((Error)) -> invalid braces sequence detected!
            throw std::logic_error("Invalid braces sequence detected!");
          }
          braces_found++;
          break;
        default:
          break;
      }
      ptr++;
    }
    if (braces_found > 1 || braces.size()) {
      // ((Error)) -> invalid braces sequence detected!
      throw std::logic_error("Invalid braces sequence detected!");
    }
    return braces_found ? ElementSrcType::kParametrized
                        : ElementSrcType::kNominal;
  }
  // ___________________________________________________________________________
  // CONSTANTs                                                     ( protected )
  //
  static constexpr char kSlash_[] = "/";
  static constexpr char kWildcardAsterisk_[] = "*";
  static constexpr char kBraceStart_ = '{';
  static constexpr char kBraceEnd_ = '}';
  static constexpr std::size_t kBufferSize = 1024;
  // ___________________________________________________________________________
  // ATTRIBUTEs                                                    ( protected )
  //
  std::shared_ptr<Element> routes_;
  char* buffer_ = nullptr;
  // ___________________________________________________________________________
  // FRIENDs                                                       ( protected )
  //
  template <typename AUty>
  friend class RoutesController;
};
}  // namespace koobika::hook::network::protocol::http

#endif