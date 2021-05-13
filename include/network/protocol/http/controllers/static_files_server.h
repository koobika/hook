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
// network/protocol/http/controllers/static_files_server.h
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

#ifndef koobika_hook_network_protocol_http_controllers_staticfilesserver_h
#define koobika_hook_network_protocol_http_controllers_staticfilesserver_h

#include <unordered_map>

#include "network/protocol/http/http_controller.h"
#include "network/protocol/http/auth/modules/no_auth.h"
#include "network/protocol/http/constants/mime.h"

namespace koobika::hook::network::protocol::http::controllers {
// =============================================================================
// StaticFilesServer                                                   ( class )
// -----------------------------------------------------------------------------
// This specification holds for built-in <static-files-server> controller
// =============================================================================
template <typename AUty = auth::modules::NoAuth>
class StaticFilesServer : public HttpController<AUty> {
 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  StaticFilesServer(const std::string& route, const std::string& path) {
    this->Get(route, this->Authorize([this, route, path](const HttpRequest& req,
                                                         HttpResponse& res) {
      // Let's remove the prefix route from the original uri..
      auto final_route = req.Uri.GetPath();
      final_route.erase(0, route.length());
      // Let's remove extra '/' '\' characters..
      auto position = final_route.find_first_not_of("\"/");
      if (position != std::string::npos) {
        final_route.erase(0, position);
      }
      auto fixed_path = path;
      position = fixed_path.find_last_not_of("\"/");
      if (position == fixed_path.length() - 1) {
        fixed_path.append("/");
      }
      // Let's retrieve the resource..
      auto content = get(fixed_path + final_route);
      if (content.first.has_value()) {
        res.Headers.Set(constants::Headers::kContentType, content.second);
        res.Body.Write(content.first.value());
        res.Ok_200();
      } else {
        res.NotFound_404();
      }
    }));
  }
  StaticFilesServer(const StaticFilesServer&) = delete;
  StaticFilesServer(StaticFilesServer&&) noexcept = delete;
  ~StaticFilesServer() = default;
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  //
  StaticFilesServer& operator=(const StaticFilesServer&) = delete;
  StaticFilesServer& operator=(StaticFilesServer&&) noexcept = delete;

 protected:
  // ___________________________________________________________________________
  // METHODs                                                       ( protected )
  //
  // Retrieves (if possible) the required resource (using cache).
  std::pair<std::optional<base::AutoBuffer>, std::string> get(
      const std::string& resource_name) {
    std::ifstream file(resource_name, std::ios::binary);
    if (file.is_open()) {
      return std::make_pair(base::AutoBuffer(file), getMimeType(resource_name));
    }
    return std::make_pair(std::optional<base::AutoBuffer>{}, "");
  }
  // Tries to extract the mime-type from the resource name.
  std::string getMimeType(const std::string& resource_name) {
    static const std::unordered_map<std::string, std::string> extensions = {
        {"jpg", constants::Mime::kJPG},
        {"jpeg", constants::Mime::kJPEG},
        {"png", constants::Mime::kPNG}};
    std::string mime_type = constants::Mime::kBIN;
    auto position = resource_name.find_last_of(".");
    if (position != std::string::npos) {
      auto extension = resource_name.substr(position+1);
      auto itr = extensions.find(extension);
      if (itr != extensions.end()) {
        mime_type = itr->second;
      }
    }
    return mime_type;
  }
};
}  // namespace koobika::hook::network::protocol::http::controllers

#endif