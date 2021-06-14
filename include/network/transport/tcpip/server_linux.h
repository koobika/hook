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
// network/transport/tcpip/server_linux.h
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

#ifndef koobika_hook_network_transport_tcpip_serverlinux_h
#define koobika_hook_network_transport_tcpip_serverlinux_h

#include <climits>
#include <mutex>
#include <optional>
#include <queue>
#include <system_error>
#include <thread>
#include <vector>

#include "base/platform.h"
#include "network/transport/server_interface.h"
#include "network/transport/server_constants.h"

namespace koobika::hook::network::transport::tcpip {
// =============================================================================
// Server                                                              ( class )
// -----------------------------------------------------------------------------
// This class holds for the server transport implementation (linux).
// -----------------------------------------------------------------------------
// Template parameters:
//    DEty - server transport decoder type being used
// =============================================================================
template <typename DEty>
class Server : public ServerInterface<int, DEty> {
 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  Server() = default;
  Server(const Server&) = delete;
  Server(Server&&) noexcept = delete;
  ~Server() { Stop(); };
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  //
  Server& operator=(const Server&) = delete;
  Server& operator=(Server&&) noexcept = delete;
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  // Starts current transport activity using the provided (json) configuration.
  void Start(const structured::json::Object& configuration,
             const typename DEty::RequestHandler& request_handler) override {
    // let's retrieve all needed parameters for this transport..
    std::string port = configuration[ServerConstants::kPortKey];
    unsigned int workers = configuration[ServerConstants::kNumberOfWorkersKey];
    // let's assign the user-specified request handler function..
    request_handler_ = request_handler;
    // let's setup all the required resources..
    setupListener(port, workers);
    setupWorkers(workers);
    // let's start incoming connections loop!
    unsigned int next = 0;
    sockaddr client_address;
    socklen_t address_len = sizeof(client_address);
    epoll_event events[kEpollMaxEvents_];
    while (keep_running_) {
      int n_fds =
          epoll_wait(epoll_fd_, events, kEpollMaxEvents_, kEpollTimeout_);
      if (n_fds < 0) {
        // ((Error)) -> trying to read epoll events!
        break;
      }
      for (auto i = 0; i < n_fds; i++) {
        Context* context = (Context*)events[i].data.ptr;
        if (events[i].events & EPOLLIN) {
          int fd = accept(context->fd, &client_address, &address_len);
          if (fd < 0) {
            // ((Error)) -> trying to accept a new connection!
            continue;
          }
          auto flags = fcntl(fd, F_GETFL, 0);
          if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
            // ((Error)) -> trying to set socket mode to non-blocking!
            shutdown(fd, SHUT_RDWR);
            close(fd);
            continue;
          }
          if (addToEpoll(workers_[next++].first, fd) == nullptr) {
            // ((Error)) -> trying to add socket to epoll handlers list!
            shutdown(fd, SHUT_RDWR);
            close(fd);
          }
          if (next == workers) next = 0;
        }
      }
    }
    close(accept_socket_);
    close(epoll_fd_);
    epoll_fd_ = INVALID_SOCKET;
    accept_socket_ = INVALID_SOCKET;
    for (auto& worker : workers_) {
      if (worker.second->joinable()) {
        worker.second->join();
      }
    }
    delete accept_ctx_;
  }
  // Stops current transport activity.
  void Stop(void) override { keep_running_ = false; }
  // Tries to send the specified buffer through the transport connection.
  bool Send(const SOCKET& handler, const base::AutoBuffer& buffer) override {
    const char* buf = nullptr;
    std::size_t len = 0;
    if (buffer.GetInternalBuffer(buf, len)) {
      std::size_t off = 0;
      while (off < len) {
        std::size_t cur = std::min<std::size_t>(INT_MAX, len - off);
        auto res = send(handler, &buf[off], (int)cur, MSG_NOSIGNAL);
        if (res == SOCKET_ERROR) {
          if (errno != EAGAIN && errno != EWOULDBLOCK) {
            // ((Error)) -> while trying to send information to socket!
            return false;
          }
        } else {
          off += res;
        }
      }
    } else {
      static constexpr auto kBufSize = ServerConstants::kDefaultWriteBufferSize;
      char* buf = new char[kBufSize];
      while (auto len = buffer.ReadSome(buf, kBufSize)) {
        std::size_t offset = 0;
        while (offset < len) {
          std::size_t amount = std::min<std::size_t>(INT_MAX, len - offset);
          auto res = send(handler, &buf[offset], (int)amount, MSG_NOSIGNAL);
          if (res == SOCKET_ERROR) {
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
              // ((Error)) -> while trying to send information to socket!
              delete[] buf;
              return false;
            }
          } else {
            offset += res;
          }
        }
      }
      delete[] buf;
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
    Context(const int& in = INVALID_SOCKET) {
      decoder = std::make_shared<DEty>();
      fd = in;
    }
    ~Context() = default;
  };
  // ___________________________________________________________________________
  // METHODs                                                         ( private )
  //
  // Sets-up listener socket resources.
  void setupListener(const std::string& port,
                     const unsigned int& number_of_workers) {
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
    auto flags = fcntl(accept_fd, F_GETFL, 0);
    if (fcntl(accept_fd, F_SETFL, flags | O_NONBLOCK) < 0) {
      close(accept_fd);
      throw std::runtime_error("could not set socket to non-blocking!");
    }
    if ((epoll_fd = epoll_create1(0)) < 0) {
      throw std::runtime_error("could not create epoll handler!");
    }
    if ((accept_ctx_ = addToEpoll(epoll_fd, accept_fd)) != nullptr) {
      epoll_fd_ = epoll_fd;
      accept_socket_ = accept_fd;
    } else {
      throw std::runtime_error("could add accept-socket to epoll handler!");
    }
  }
  // Sets-up all needed workers within this transport.
  void setupWorkers(const int& number_of_workers) {
    for (int i = 0; i < number_of_workers; i++) {
      int efd = epoll_create1(0);
      if (efd < 0) {
        // ((Error)) -> trying to create epoll handler!
        throw std::runtime_error(
            "could not create private (per-thread) epoll handler!");
      }
      workers_.push_back(std::make_pair(
          efd, std::make_shared<std::thread>([efd, this]() {
            epoll_event ev[kEpollMaxEvents_];
            char buffer[ServerConstants::kDefaultReadBufferSize];
            while (keep_running_) {
              int n_fds = epoll_wait(efd, ev, kEpollMaxEvents_, kEpollTimeout_);
              if (n_fds < 0) {
                // ((Error)) -> trying to complete wait: shutting down?
                break;
              }
              for (auto i = 0; i < n_fds; i++) {
                Context* ctx = (Context*)ev[i].data.ptr;
                if (ev[i].events & EPOLLIN) {
                  auto bytes_returned = read(
                      ctx->fd, buffer, ServerConstants::kDefaultReadBufferSize);
                  if (bytes_returned < 0 && errno != EAGAIN &&
                      errno != EWOULDBLOCK) {
                    removeFromEpollAndClose(efd, ctx);
                    continue;
                  }
                  ctx->decoder->Add(buffer, bytes_returned);
                  ctx->decoder->Decode(
                      request_handler_,
                      [this, ctx, efd]() { removeFromEpollAndClose(efd, ctx); },
                      [this, ctx, efd](const base::AutoBuffer& buffer) {
                        if (!Send(ctx->fd, buffer)) {
                          removeFromEpollAndClose(efd, ctx);
                        }
                      });
                }
                if (ev[i].events & EPOLLHUP || ev[i].events & EPOLLRDHUP ||
                    ev[i].events & EPOLLERR) {
                  removeFromEpollAndClose(efd, ctx);
                }
              }
            }
          })));
    }
  }
  // Adds the specifed socket to the epoll handlers list.
  Context* addToEpoll(const int& efd, const int& fd) {
    epoll_event event;
    Context* ctx = new Context(fd);
    event.events = EPOLLIN | EPOLLHUP | EPOLLRDHUP | EPOLLERR;
    event.data.ptr = ctx;
    if (epoll_ctl(efd, EPOLL_CTL_ADD, fd, &event) < 0) {
      // ((Error)) -> trying to register within the internal handlers list!
      delete ctx;
      ctx = nullptr;
    }
    return ctx;
  }
  // Removes the associated context from epoll (closing resources).
  void removeFromEpollAndClose(const int& efd, const Context* ctx) {
    if (epoll_ctl(efd, EPOLL_CTL_DEL, ctx->fd, NULL) < 0) {
      // ((Error)) -> trying to de-register from internal handlers list!
    }
    shutdown(ctx->fd, SHUT_RDWR);
    close(ctx->fd);
    delete ctx;
  }
  // ___________________________________________________________________________
  // CONSTANTs                                                       ( private )
  //
  static constexpr int kEpollTimeout_ = 1000;
  static constexpr int kEpollMaxEvents_ = 256;
  // ___________________________________________________________________________
  // ATTRIBUTEs                                                      ( private )
  //
  int epoll_fd_ = INVALID_SOCKET;
  int accept_socket_ = INVALID_SOCKET;
  Context* accept_ctx_ = nullptr;
  bool keep_running_ = true;
  std::vector<std::pair<int, std::shared_ptr<std::thread>>> workers_;
  typename DEty::RequestHandler request_handler_;
};
}  // namespace koobika::hook::network::transport::tcpip

#endif