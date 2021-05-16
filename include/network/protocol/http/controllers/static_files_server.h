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
#include <filesystem>

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
    // Let's setup all default (built-in) supported extensions.
    extensions_ = {
        {".3g2", constants::Mime::k3G2},
        {".3gp", constants::Mime::k3GP},
        {".7z", constants::Mime::k7Z},
        {".aac", constants::Mime::kAAC},
        {".abw", constants::Mime::kABW},
        {".arc", constants::Mime::kARC},
        {".avi", constants::Mime::kAVI},
        {".azw", constants::Mime::kAZW},
        {".bz", constants::Mime::kBZ},
        {".bz2", constants::Mime::kBZ2},
        {".csh", constants::Mime::kCSH},
        {".css", constants::Mime::kCSS},
        {".csv", constants::Mime::kCSV},
        {".doc", constants::Mime::kDOC},
        {".docx", constants::Mime::kDOCX},
        {".gz", constants::Mime::kGZ},
        {".htm", constants::Mime::kHTM},
        {".html", constants::Mime::kHTML},
        {".ico", constants::Mime::kICO},
        {".ics", constants::Mime::kICS},
        {".jar", constants::Mime::kJAR},
        {".jpg", constants::Mime::kJPG},
        {".jpeg", constants::Mime::kJPEG},
        {".js", constants::Mime::kJS},
        {".json", constants::Mime::kJSON},
        {".mid", constants::Mime::kMID},
        {".midi", constants::Mime::kMIDI},
        {".mjs", constants::Mime::kMJS},
        {".mp3", constants::Mime::kMP3},
        {".mpeg", constants::Mime::kMPEG},
        {".mpkg", constants::Mime::kMPKG},
        {".odp", constants::Mime::kODP},
        {".ods", constants::Mime::kODS},
        {".odt", constants::Mime::kODT},
        {".oga", constants::Mime::kOGA},
        {".ogv", constants::Mime::kOGV},
        {".ogx", constants::Mime::kOGX},
        {".opus", constants::Mime::kOPUS},
        {".orf", constants::Mime::kOTF},
        {".pdf", constants::Mime::kPDF},
        {".php", constants::Mime::kPHP},
        {".png", constants::Mime::kPNG},
        {".ppt", constants::Mime::kPPT},
        {".pptx", constants::Mime::kPPTX},
        {".rar", constants::Mime::kRAR},
        {".rtf", constants::Mime::kRTF},
        {".sh", constants::Mime::kSH},
        {".svg", constants::Mime::kSVG},
        {".swf", constants::Mime::kSWF},
        {".tar", constants::Mime::kTAR},
        {".tif", constants::Mime::kTIF},
        {".tiff", constants::Mime::kTIFF},
        {".ts", constants::Mime::kTS},
        {".ttf", constants::Mime::kTTF},
        {".txt", constants::Mime::kTXT},
        {".vsd", constants::Mime::kVSD},
        {".wav", constants::Mime::kWAV},
        {".weba", constants::Mime::kWEBA},
        {".webm", constants::Mime::kWEBM},
        {".webp", constants::Mime::kWEBP},
        {".woff", constants::Mime::kWOFF},
        {".xhtml", constants::Mime::kXHTML},
        {".xls", constants::Mime::kXLS},
        {".xlsx", constants::Mime::kXLSX},
        {".xmid", constants::Mime::kXMID},
        {".xmidi", constants::Mime::kXMIDI},
        {".xml", constants::Mime::kXMLNONREADABLE},
        {".xul", constants::Mime::kXUL},
        {".zip", constants::Mime::kZIP},
    };
    // Let's setup the <get> end-point.
    this->Get(route, this->Authorize([this, route, path](const HttpRequest& req,
                                                         HttpResponse& res) {
      // Let's check if given uri path is starting from the base one..
      auto base_path = std::filesystem::path(path);
      auto base_path_absolute = std::filesystem::absolute(base_path);
      auto uri_path_absolute = std::filesystem::absolute(
          base_path.concat(std::filesystem::path(req.Uri.GetPath()).string()));
      auto uri_string = uri_path_absolute.string();
      if (!uri_string.find(base_path_absolute.string())) {
        // Let's try to retrieve the required content..
        auto content = get(uri_string);
        if (content.first.has_value()) {
          res.Headers.Set(constants::Headers::kContentType, content.second);
          res.Body.Write(content.first.value());
          res.Ok_200();
        } else {
          // ((Error)) -> while trying to access an invalid resource!
          res.NotFound_404();
        }
      } else {
        // ((Error)) -> while trying to access a forbidden path!
        res.BadRequest_400();
      }
    }));
  }
  StaticFilesServer(
      const std::string& route, const std::string& path,
      const std::unordered_map<std::string, std::string>& additional_mime_types)
      : StaticFilesServer(route, path) {
    for (auto const& additional : additional_mime_types) {
      extensions_.insert(additional);
    }
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
    std::string mime_type = constants::Mime::kBIN;
    auto ex = std::filesystem::path(resource_name).extension().string();
    std::transform(ex.begin(), ex.end(), ex.begin(), ::tolower);
    auto itr = extensions_.find(ex);
    if (itr != extensions_.end()) {
      mime_type = itr->second;
    }
    return mime_type;
  }

 protected:
  // ___________________________________________________________________________
  // ATTRIBUTEs                                                    ( protected )
  //
  std::unordered_map<std::string, std::string> extensions_;
};
}  // namespace koobika::hook::network::protocol::http::controllers

#endif