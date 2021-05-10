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
// network/transport/tcpip/server_transport_tcp_ip_win.h
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

#ifndef koobika_hook_network_transport_tcpip_servertransporttcpipwin_h
#define koobika_hook_network_transport_tcpip_servertransporttcpipwin_h

#include <limits>
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
// This class holds for the server transport implementation (windows).
// -----------------------------------------------------------------------------
// Template parameters:
//    DEty - server transport decoder type being used
// =============================================================================
template <typename DEty>
class ServerTransportTcpIp : public ServerTransport<SOCKET, DEty> {
  // ___________________________________________________________________________
  // USINGs                                                          ( private )
  // 
  using Interface = ServerTransport<SOCKET, DEty>;

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
    if (io_port_ != INVALID_HANDLE_VALUE) {
      // ((Error)) -> transport already initialized!
      throw std::logic_error("Already initialized transport!");
    }
    // let's retrieve all needed parameters for this transport..
    std::string port =
        configuration[transport::ServerTransportConstants::kPortKey];
    int number_of_workers =
        configuration[transport::ServerTransportConstants::kNumberOfWorkersKey];
    // let's assign the user-specified request handler function..
    request_handler_ = request_handler;
    // let's setup all the required resources..
    setupWinsock();
    setupListener(port, number_of_workers);
    setupWorkers(number_of_workers);
    // let's start incoming connections loop!
    while (true) {
      SOCKET client = WSAAccept(accept_socket_, NULL, NULL, NULL, NULL);
      if (client == INVALID_SOCKET) {
        // ((Error)) -> trying to accept a new connection: shutting down?
        break;
      }
      // set the socket i/o mode: In this case FIONBIO enables or disables the
      // blocking mode for the socket based on the numerical value of iMode.
      // iMode = 0, blocking mode; iMode != 0, non-blocking mode.
      ULONG i_mode = 1;
      auto ioctl_socket_res = ioctlsocket(client, FIONBIO, &i_mode);
      if (ioctl_socket_res != NO_ERROR) {
        // ((Error)) -> could not change blocking mode on socket!
        // ((To-Do)) -> raise an exception?
      }
      // let's associate the accept socket with the i/o port!
      ULONG_PTR key = (ULONG_PTR) new Context(client);
      if (!CreateIoCompletionPort((HANDLE)client, io_port_, key, 0)) {
        // ((Error)) -> trying to associate socket to the i/o port!
        // ((To-Do)) -> raise an exception?
      }
      // let's notify waiting thread for the new connection!
      if (!PostQueuedCompletionStatus(io_port_, 0, key, NULL)) {
        // ((Error)) -> trying to notify waiting thread!
        // ((To-Do)) -> raise an exception?
      }
    }
    if (io_port_ != INVALID_HANDLE_VALUE) {
      for (auto& worker : workers_) {
        if (worker->joinable()) {
          worker->join();
        }
      }
      io_port_ = INVALID_HANDLE_VALUE;
      accept_socket_ = INVALID_SOCKET;
    }
  }
  // Stops current transport activity.
  void Stop(void) override { 
    CloseHandle(io_port_);
    closesocket(accept_socket_);
  }
  // Tries to send the specified buffer through the transport connection.
  bool Send(const SOCKET& handler, const base::AutoBuffer& buffer) override {
    char tmp[ServerTransportConstants::kDefaultWriteBufferSize];
    while (std::size_t length = buffer.ReadSome(
               tmp, ServerTransportConstants::kDefaultWriteBufferSize)) {
      std::size_t offset = 0;
      while (offset < length) {
        std::size_t len = std::min<std::size_t>(INT_MAX, length - offset);
        auto res = ::send(handler, &((const char*)tmp)[offset], (int)len, 0x0);
        if (res == SOCKET_ERROR) {
          if (WSAGetLastError() != WSAEWOULDBLOCK) {
            // ((Error)) -> while trying to send information to socket!
            // ((To-Do)) -> inform user back?
            return false;
          }
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
    WSAOVERLAPPED overlapped;
    WSABUF data;
    SOCKET socket;
    std::shared_ptr<DEty> decoder;
    Context(const SOCKET& in = INVALID_SOCKET) {
      decoder = std::make_shared<DEty>();
      ZeroMemory(&overlapped, sizeof(WSAOVERLAPPED));
      data.buf = new CHAR[ServerTransportConstants::kDefaultReadBufferSize];
      data.len = ServerTransportConstants::kDefaultReadBufferSize;
      socket = in;
    }
    ~Context() { delete[] data.buf; }
  };
  // ___________________________________________________________________________
  // METHODs                                                         ( private )
  // 
  // Sets-up winsock resources.
  void setupWinsock() {
    struct WsaInitializer_ {
      WsaInitializer_() {
        if (WSAStartup(MAKEWORD(2, 2), &wsa_data)) {
          // ((Error)) -> winsock could not be initialized!
          throw std::runtime_error("could not initialize winsock!");
        }
      }
      ~WsaInitializer_() { WSACleanup(); }
      WSADATA wsa_data;
    };
    static std::shared_ptr<WsaInitializer_> wsa_initializer_ptr_;
    if (wsa_initializer_ptr_ == nullptr) {
      wsa_initializer_ptr_ = std::make_shared<WsaInitializer_>();
    }
  }
  // Sets-up listener socket resources.
  void setupListener(const std::string& port, const int& number_of_workers) {
    // let's create our main i/o completion port!
    auto io_port = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL,
                                          number_of_workers);
    if (io_port == nullptr) {
      // ((Error)) -> while setting up the i/o completion port!
      throw std::runtime_error("could not setup i/o completion port!");
    }
    // let's setup our main listening socket (server)!
    SOCKET sock = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0,
                             WSA_FLAG_OVERLAPPED);
    if (sock == INVALID_SOCKET) {
      // ((Error)) -> could not create socket!
      CloseHandle(io_port);
      throw std::runtime_error("could not create listening socket!");
    }
    // let's associate the listening port to the i/o completion port!
    if (CreateIoCompletionPort((HANDLE)sock, io_port, 0UL, 0) == nullptr) {
      // ((Error)) -> while setting up the i/o completion port!
      CloseHandle(io_port);
      closesocket(sock);
      throw std::runtime_error("could not setup i/o completion port!");
    }
    // set the socket i/o mode: In this case FIONBIO enables or disables the
    // blocking mode for the socket based on the numerical value of iMode.
    // iMode = 0, blocking mode; iMode != 0, non-blocking mode.
    ULONG i_mode = 0;
    auto ioctl_socket_res = ioctlsocket(sock, FIONBIO, &i_mode);
    if (ioctl_socket_res != NO_ERROR) {
      // ((Error)) -> could not change blocking mode on socket!
      CloseHandle(io_port);
      closesocket(sock);
      throw std::runtime_error("could not set listening socket i/o mode!");
    }
    sockaddr_in address = {0};
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_family = PF_INET;
    address.sin_port = htons(atoi(port.c_str()));
    auto bind_res = bind(sock, (const sockaddr*)&address, sizeof(address));
    if (bind_res == SOCKET_ERROR) {
      // ((Error)) -> could not bind socket!
      CloseHandle(io_port);
      closesocket(sock);
      throw std::runtime_error("could not bind to listening socket!");
    }
    if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
      // ((Error)) -> could not listen socket!
      CloseHandle(io_port);
      closesocket(sock);
      throw std::runtime_error("could not bind to listening socket!");
    }
    accept_socket_ = sock;
    io_port_ = io_port;
  }
  // Sets-up all needed workers within this transport.
  void setupWorkers(const int& number_of_workers) {
    for (int i = 0; i < number_of_workers; i++) {
      workers_.push_back(std::make_shared<std::thread>([this]() {
        ULONG_PTR completion_key = NULL;
        LPOVERLAPPED overlapped = NULL;
        DWORD bytes_returned = 0;
        while (true) {
          if (!GetQueuedCompletionStatus(io_port_, &bytes_returned,
                                         (PULONG_PTR)&completion_key,
                                         &overlapped, INFINITE)) {
            // ok, this is fine, we're shutting down our completion port..
            // at this point, we need to perform a controlled shutdown.
            break;
          }
          Context* context = (Context*)completion_key;
          if (overlapped) {
            if (!bytes_returned) {
              // connection closed! let's free the associated resources!
              closesocket(context->socket);
              delete context;
              continue;
            }
          } else if (bytes_returned) {
            // connection closed! let's free the associated resources!
            closesocket(context->socket);
            delete context;
            continue;
          }
          if (bytes_returned &&
              context->decoder->Add(context->data.buf, bytes_returned)) {
            context->decoder->Decode(
                request_handler_,
                [context]() {
                  // connection closed! let's free the associated resources!
                  closesocket(context->socket);
                },
                [this, context](const base::AutoBuffer& buffer) {
                  if (!Send(context->socket, buffer)) {
                    // connection closed! let's free the associated resources!
                    closesocket(context->socket);
                  }
                });
          }
          DWORD recv_flags = 0;
          DWORD bytes_transmitted = 0;
          int wsarecv_result = WSARecv(
              context->socket, &context->data, 0x1, &bytes_transmitted,
              &recv_flags, (LPWSAOVERLAPPED)&context->overlapped, nullptr);
          if (wsarecv_result == SOCKET_ERROR) {
            if (WSAGetLastError() != WSA_IO_PENDING) {
              // connection closed! let's free the associated resources!
              closesocket(context->socket);
              delete context;
            }
          }
        }
      }));
    }
  }
  // ___________________________________________________________________________
  // ATTRIBUTEs                                                      ( private )
  // 
  HANDLE io_port_ = INVALID_HANDLE_VALUE;
  SOCKET accept_socket_ = INVALID_SOCKET;
  std::vector<std::shared_ptr<std::thread>> workers_;
  typename DEty::RequestHandler request_handler_;
};
}  // namespace koobika::hook::network::transport::tcpip

#endif