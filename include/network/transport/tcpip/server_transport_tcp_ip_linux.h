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
// network/transport/tcpip/server_transport_tcp_ip_linux.h
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

#ifndef koobika_hook_network_transport_tcpip_servertransporttcpiplinux_h
#define koobika_hook_network_transport_tcpip_servertransporttcpiplinux_h

#include <climits>
#include <mutex>
#include <optional>
#include <queue>
#include <system_error>
#include <thread>
#include <vector>

#include "base/platform.h"
#include "network/transport/server_transport.h"
#include "network/transport/server_transport_constants.h"

namespace koobika::hook::network::transport::tcpip {
// =============================================================================
// ServerTransportTcpIp                                                ( class )
// -----------------------------------------------------------------------------
// This class holds for the server transport implementation (linux).
// -----------------------------------------------------------------------------
// Template parameters:
//    DEty - server transport decoder type being used
// =============================================================================
template <typename DEty>
class ServerTransportTcpIp : public ServerTransport<int, DEty> {
  // ___________________________________________________________________________
  // USINGs                                                          ( private )
  // 
  using Interface = ServerTransport<int, DEty>;

 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  // 
  ServerTransportTcpIp() = default;
  ServerTransportTcpIp(const ServerTransportTcpIp&) = delete;
  ServerTransportTcpIp(ServerTransportTcpIp&&) noexcept = delete;
  ~ServerTransportTcpIp() { Stop(); };
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  // 
  ServerTransportTcpIp& operator=(const ServerTransportTcpIp&) = delete;
  ServerTransportTcpIp& operator=(ServerTransportTcpIp&&) noexcept = delete;
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  // 
  // Starts current transport activity using the provided (json) configuration.
  void Start(const structured::json::Object& configuration,
             const typename DEty::RequestHandler& request_handler) override {
    // let's retrieve all needed parameters for this transport..
    std::string port = configuration[transport::ServerTransportConstants::kPortKey];
    int number_of_workers =
        configuration[transport::ServerTransportConstants::kNumberOfWorkersKey];
    int max_connections = configuration[transport::ServerTransportConstants::kMaxConnectionsKey];
    // let's assign the user-specified request handler function..
    request_handler_ = request_handler;
    // let's setup all the required resources..
    setupListener(port, number_of_workers);
    setupWorkers(number_of_workers);
    // let's start incoming connections loop!
    sockaddr clientaddress;
    socklen_t address_len = sizeof(clientaddress);
    epoll_event events[kEpollMaxEventsAtOnce_];
    while (running_) {
      int n_fds = epoll_wait(epoll_fd_, events, kEpollMaxEventsAtOnce_,
                             kEpollTimeoutMS_);
      if (n_fds >= 0) {
        for (auto i = 0; i < n_fds; i++) {
          Context* context = (Context*)events[i].data.ptr;
          int client = accept(context->fd, &clientaddress, &address_len);

          int a = 0x0;
        }
      } else {
        // ((Error)) -> trying to accept a new connection: shutting down?
        break;
      }
    }
    close(accept_socket_);
    close(epoll_fd_);
    epoll_fd_ = -1;
    accept_socket_ = -1;
    while (threads_.size()) {
      auto& thread = threads_.front();
      if (thread->joinable()) {
        thread->join();
      }
      threads_.pop();
    }
  }
  // Stops current transport activity.
  void Stop(void) override { running_ = false; }
  // Tries to send the specified buffer through the transport connection.
  bool Send(const SOCKET& handler, const base::AutoBuffer& buffer) override {
    char tmp[ServerTransportConstants::kDefaultWriteBufferSize];
    while (std::size_t length =
               buffer.ReadSome(tmp, ServerTransportConstants::kDefaultWriteBufferSize)) {
      std::size_t offset = 0;
      while (offset < length) {
        std::size_t len = std::min<std::size_t>(INT_MAX, length - offset);
        auto res = ::send(handler, &((const char*)tmp)[offset], (int)len, 0x0);
        if (res == SOCKET_ERROR) {
          // ((Error)) -> while trying to send information to socket!
          // ((To-Do)) -> inform user back?
          return false;
        } else {
          offset += res;
        }
      }
    }
    return true;
  }

 private:
  // ___________________________________________________________________________
  // TYPEs                                                           ( private )
  // 
  struct Context {
    SOCKET fd;
    std::shared_ptr<DEty> decoder;
    Context(const int& in = -1) {
      decoder = std::make_shared<DEty>();
      fd = in;
    }
    ~Context() = default;
  };
  // ___________________________________________________________________________
  // METHODs                                                         ( private )
  // 
  // Sets-up listener socket resources.
  void setupListener(const std::string& port, const int& number_of_workers) {
    int accept_fd, epoll_fd;
    sockaddr_in addr = {0};  
    memset(&addr, 0, sizeof(addr));
    if ((accept_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
      // ((Error)) -> could not create socket!
      throw std::runtime_error("could not create listening socket!");
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    uint16_t port_number = (uint16_t)atoi(port.c_str());
    addr.sin_port = htons(port_number);
    if (bind(accept_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
      close(accept_fd);
      throw std::runtime_error("could not bind to listening socket!");
    }
    if (listen(accept_fd, SOMAXCONN) < 0) {
      close(accept_fd);
      throw std::runtime_error("could not bind to listening socket!");
    }

    /*
    pepe
    */

    /*
    auto flags = fcntl(fd, F_GETFL, 0);
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
      close(fd);
      throw std::runtime_error("could not set socket to non-blocking!");
    }
    */

    /*
    pepe fin
    */

    if ((epoll_fd = epoll_create1(0)) < 0) {
      close(epoll_fd);
      throw std::runtime_error("could not create epoll handler!");
    }
    epoll_event event;
    event.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLERR;
    event.data.ptr = new Context(accept_fd);
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, accept_fd, &event) < 0) {
      close(accept_fd);
      throw std::runtime_error("could add socket to epoll handler!");
    }
    epoll_fd_ = epoll_fd;
    accept_socket_ = accept_fd;
  }
  // Sets-up all needed workers within this transport.
  void setupWorkers(const int& number_of_workers) {
    for (int i = 0; i < number_of_workers; i++) {
    }
  }
  // ___________________________________________________________________________
  // CONSTANTs                                                       ( private )
  // 
  static constexpr int kEpollTimeoutMS_ = 1000;
  static constexpr int kEpollMaxEventsAtOnce_ = 256;
  // ___________________________________________________________________________
  // ATTRIBUTEs                                                      ( private )
  // 
  bool running_ = true;
  int epoll_fd_ = -1;
  int accept_socket_ = -1;
  std::queue<std::shared_ptr<std::thread>> threads_;
  typename DEty::RequestHandler request_handler_;
};
}  // namespace koobika::hook::network::transport::tcpip

#endif