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
// ServerTransportTcpIp                                                [ class ]
// -----------------------------------------------------------------------------
// This class holds for the server transport implementation (windows)
// -----------------------------------------------------------------------------
// Template parameters:
//    DEty - server transport decoder type being used
// =============================================================================
template <typename DEty>
class ServerTransportTcpIp : public ServerTransport<SOCKET, DEty> {
 private:
  // ---------------------------------------------------------------------------
  // Usings                                                          [ private ]
  // ---------------------------------------------------------------------------
  using Interface = ServerTransport<SOCKET, DEty>;

 public:
  // ---------------------------------------------------------------------------
  // Constructors/Destructors                                         [ public ]
  // ---------------------------------------------------------------------------
  ServerTransportTcpIp()
      : io_port_{INVALID_HANDLE_VALUE},
        listen_socket_{INVALID_SOCKET},
        fn_accept_ex_{nullptr} {}
  ServerTransportTcpIp(const ServerTransportTcpIp&) = delete;
  ServerTransportTcpIp(ServerTransportTcpIp&&) noexcept = delete;
  ~ServerTransportTcpIp() { Cleanup(); };
  // ---------------------------------------------------------------------------
  // Operators                                                        [ public ]
  // ---------------------------------------------------------------------------
  ServerTransportTcpIp& operator=(const ServerTransportTcpIp&) = delete;
  ServerTransportTcpIp& operator=(ServerTransportTcpIp&&) noexcept = delete;
  // ---------------------------------------------------------------------------
  // Methods                                                          [ public ]
  // ---------------------------------------------------------------------------
  // Sets up current transport object
  void Setup(const base::KeyValueStore<std::string>& configuration) override {
    if (io_port_ != INVALID_HANDLE_VALUE) {
      // [error] -> transport already initialized!
      throw std::logic_error("already initialized transport!");
    }
    SetupWinsock_();
    SetupListener_(configuration);
    SetupWorkers_(configuration);
  }
  // Cleans up current transport object
  void Cleanup(void) override {
    if (io_port_ != INVALID_HANDLE_VALUE) {
      CloseHandle(io_port_);
      closesocket(listen_socket_);
    }
    while (!threads_.empty()) {
      auto& thread = threads_.front();
      if (thread->joinable()) {
        thread->join();
      }
      threads_.pop();
    }
    while (!contexts_.empty()) {
      delete contexts_.front();
      contexts_.pop();
    }
    io_port_ = INVALID_HANDLE_VALUE;
    listen_socket_ = INVALID_SOCKET;
  }
  // Tries to send the specified buffer through the transport connection
  void Send(const SOCKET& id, const base::Stream& stream) override {
    char buffer[ServerTransportConstants::kDefaultWriteBufferSize];
    std::size_t length;
    while (length = stream.ReadSome(
               buffer, ServerTransportConstants::kDefaultWriteBufferSize)) {
      std::size_t offset = 0;
      while (offset < length) {
        std::size_t len = std::min<std::size_t>(INT_MAX, length - offset);
        auto res = ::send(id, &((const char*)buffer)[offset], (int)len, 0x0);
        if (res == SOCKET_ERROR) {
          // [error] -> while trying to send information to socket!
          // [to-do] -> inform user back?
          return;
        }
        offset += res;
      }
    }
  }
  // This method will be used to set the function in charge of accepting (or
  // not an incoming connection attempt within the transport (callback)
  void SetOnAcceptConnectionCallback(
      const typename Interface::OnAcceptConnection& fn) override {
    on_accept_connection_ = fn;
  }
  // This method will be used to set the function in charge of notifying for
  // every accepted connection within the transport (callback)
  void SetOnConnectionAcceptedCallback(
      const typename Interface::OnConnectionAccepted& fn) override {
    on_connection_accepted_ = fn;
  }
  // This method will be used to set the function in charge of notifying for
  // every rejected connection within the transport (callback)
  void SetOnConnectionRejectedCallback(
      const typename Interface::OnConnectionRejected& fn) override {
    on_connection_rejected_ = fn;
  }
  // This method will be used to set the function in charge of notifying for
  // incoming data within the provided connection (callback)
  void SetOnDataCallback(const typename Interface::OnData& fn) override {
    on_data_ = fn;
  }

 private:
  // ---------------------------------------------------------------------------
  // Types                                                           [ private ]
  // ---------------------------------------------------------------------------
  struct Context_ : public WSAOVERLAPPED {
    Context_() {
      ZeroMemory(&Internal, sizeof(Internal));
      ZeroMemory(&InternalHigh, sizeof(InternalHigh));
      ZeroMemory(&Offset, sizeof(Offset));
      ZeroMemory(&OffsetHigh, sizeof(OffsetHigh));
      ZeroMemory(&hEvent, sizeof(hEvent));
      wsa_recv_data.buf =
          new char[transport::ServerTransportConstants::kDefaultReadBufferSize];
      wsa_recv_data.len =
          transport::ServerTransportConstants::kDefaultReadBufferSize;
      sock = INVALID_SOCKET;
      status = 0;
    }
    ~Context_() { delete[] wsa_recv_data.buf; }
    WSABUF wsa_recv_data;
    DEty decoder;
    SOCKET sock;
    char status;
  };
  // ---------------------------------------------------------------------------
  // Methods                                                         [ private ]
  // ---------------------------------------------------------------------------
  // Sets up winsock resources
  void SetupWinsock_() {
    struct WsaInitializer_ {
      WsaInitializer_() {
        // try to initialize Winsock static resources..
        auto wsa_startup_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
        if (wsa_startup_result != 0) {
          // [error] -> winsock could not be initialized!
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
  // Sets up listener resources
  void SetupListener_(const base::KeyValueStore<std::string>& configuration) {
    // let's extract needed parameters from configuration!
    auto port_itr =
        configuration.find(transport::ServerTransportConstants::kKeyPort);
    if (port_itr == configuration.end()) {
      // [error] -> needed configuration parameter not found!
      throw std::logic_error(
          "needed configuration parameter 'port' not specified!");
    }
    auto port_number = port_itr->second.Get<std::string>();
    if (!port_number.has_value()) {
      // [error] -> needed configuration parameter not found!
      throw std::logic_error(
          "needed configuration parameter 'port' not specified as "
          "'std::string'!");
    }
    // let's create our main i/o completion port!
    auto io_port = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
    if (io_port == nullptr) {
      // [error] -> while setting up the i/o completion port!
      throw std::runtime_error("could not setup i/o completion port!");
    }
    // let's setup our main listening socket (server)!
    SOCKET sock = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0,
                             WSA_FLAG_OVERLAPPED);
    if (sock == INVALID_SOCKET) {
      // [error] -> could not create socket!
      CloseHandle(io_port);
      throw std::runtime_error("could not create listening socket!");
    }
    // let's associate the listening port to the i/o completion port!
    if (CreateIoCompletionPort((HANDLE)sock, io_port, 0UL, 0) == nullptr) {
      // [error] -> while setting up the i/o completion port!
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
      // [error] -> could not change blocking mode on socket!
      CloseHandle(io_port);
      closesocket(sock);
      throw std::runtime_error("could not set listening socket i/o mode!");
    }
    sockaddr_in address = {0};
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_family = PF_INET;
    address.sin_port = htons(atoi(port_number->data()));
    auto bind_res = bind(sock, (const sockaddr*)&address, sizeof(address));
    if (bind_res == SOCKET_ERROR) {
      // [error] -> could not bind socket!
      CloseHandle(io_port);
      closesocket(sock);
      throw std::runtime_error("could not bind to listening socket!");
    }
    if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
      // [error] -> could not listen socket!
      CloseHandle(io_port);
      closesocket(sock);
      throw std::runtime_error("could not bind to listening socket!");
    }
    // load the AcceptEx function into memory using WSAIoctl. The WSAIoctl
    // function is an extension of the ioctlsocket()function that can use
    // overlapped I/O. The function's 3rd through 6th parameters are input
    // and output buffers where we pass the pointer to our AcceptEx function.
    // This is used so that we can call the AcceptEx function directly, rather
    // than refer to the Mswsock.lib library.
    DWORD dw_chars;
    LPFN_ACCEPTEX fn_accept_ex = nullptr;
    GUID guid_accept_ex = WSAID_ACCEPTEX;
    auto ioctl_res =
        WSAIoctl(sock, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid_accept_ex,
                 sizeof(guid_accept_ex), &fn_accept_ex, sizeof(fn_accept_ex),
                 &dw_chars, NULL, NULL);
    if (ioctl_res == SOCKET_ERROR) {
      // [error] -> could not call ioctl for accepting socket!
      CloseHandle(io_port);
      closesocket(sock);
      throw std::runtime_error("could not perform WSAIoctl!");
    }
    // ok!, if we reach this point means that everything went fine!
    fn_accept_ex_ = fn_accept_ex;
    listen_socket_ = sock;
    io_port_ = io_port;
  }
  // Sets up workers resources
  void SetupWorkers_(const base::KeyValueStore<std::string>& configuration) {
    // let's extract [kMaximumNumberOfSimultaneousConnections]!
    auto max_connections_itr = configuration.find(
        transport::ServerTransportConstants::kKeyMaxConnections);
    if (max_connections_itr == configuration.end()) {
      // [error] -> needed configuration parameter not found!
      throw std::logic_error(
          "needed configuration parameter 'maxConnections' not specified!");
    }
    auto number_of_contexts = max_connections_itr->second.Get<int>();
    if (!number_of_contexts.has_value()) {
      // [error] -> needed configuration parameter not found!
      throw std::logic_error(
          "needed configuration parameter 'maxConnections' not specified as "
          "'int'!");
    }
    // let's extract [kNumberOfWorkers]!
    auto number_of_workers_itr = configuration.find(
        transport::ServerTransportConstants::kKeyNumberOfWorkers);
    if (number_of_workers_itr == configuration.end()) {
      // [error] -> needed configuration parameter not found!
      throw std::logic_error(
          "needed configuration parameter 'numberOfWorkers' not specified!");
    }
    auto number_of_workers = number_of_workers_itr->second.Get<int>();
    if (!number_of_workers.has_value()) {
      // [error] -> needed configuration parameter not found!
      throw std::logic_error(
          "needed configuration parameter 'numberOfWorkers' not specified as "
          "'int'!");
    }
    // first, let's create all available contexts (including sockets)!
    auto contexts = *number_of_contexts;
    auto workers = *number_of_workers;
    for (auto i = 0; i < contexts; i++) {
      auto new_context = new Context_();
      new_context->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      if (new_context->sock == INVALID_SOCKET) {
        // [error] -> trying to create new socket resource!
        delete new_context;
        throw std::runtime_error("could not create accept socket!");
      }
      if (PrepareContextForUse_(new_context)) {
        contexts_.push(new_context);
      } else {
        // [error] -> trying to prepare context for use!
        delete new_context;
        throw std::runtime_error("could not prepare context for use!");
      }
    }
    // second, let's create all working threads!
    auto number_of_entries = contexts / workers;
    for (auto i = 0; i < workers; i++) {
      threads_.push(std::make_shared<std::thread>([this, number_of_entries]() {
        OVERLAPPED_ENTRY* entries = new OVERLAPPED_ENTRY[number_of_entries];
        while (true) {
          ULONG entries_removed;
          auto status =
              GetQueuedCompletionStatusEx(io_port_, entries, number_of_entries,
                                          &entries_removed, INFINITE, FALSE);
          if (!status) {
            // ok, this is fine, we're shutting down..
            break;
          }
          for (ULONG i = 0; i < entries_removed; i++) {
            Context_* ctx = (Context_*)entries[i].lpOverlapped;
            if (!ctx) {
              // [error] -> this should never happen! anyway let's skip it!
              continue;
            }
            // let's us our callback to ask for socket acceptance..
            if (!ctx->status) {
              bool accepted = (on_accept_connection_ == nullptr) ||
                              (on_accept_connection_(ctx->sock));
              if (accepted) {
                if (on_connection_accepted_ != nullptr) {
                  on_connection_accepted_(ctx->sock);
                }
              } else {
                if (!PrepareContextForUse_(ctx, true)) {
                  // [error] -> trying to reuse socket!
                }
                if (on_connection_rejected_ != nullptr) {
                  on_connection_rejected_(ctx->sock);
                }
              }
              ctx->status = ~ctx->status;
            }
            if (!entries[i].dwNumberOfBytesTransferred) {
              // [error] -> disconnected! let's recycle it for later use!
              if (!PrepareContextForUse_(ctx, true)) {
                // [error] -> trying to reuse socket!
              }
              continue;
            }
            if (!ctx->decoder.Add(ctx->wsa_recv_data.buf,
                                  entries[i].dwNumberOfBytesTransferred)) {
              if (!PrepareContextForUse_(ctx, true)) {
                // [error] -> trying to reuse socket!
              }
            }
            auto sock = ctx->sock;
            if (on_data_ != nullptr) {
              on_data_(
                  ctx->decoder, sock,
                  [this, sock](const base::Stream& stream) {
                    Send(sock, stream);
                  },
                  [this, ctx]() {
                    if (!PrepareContextForUse_(ctx, true)) {
                      // [error] -> trying to reuse socket!
                    }
                  });
            }
            DWORD recv_flags = 0;
            DWORD chars_returned;
            auto res = WSARecv(ctx->sock, &ctx->wsa_recv_data, 1,
                               &chars_returned, &recv_flags, ctx, nullptr);
            if (res == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) {
              // [error] -> the socket has been closed!
              closesocket(ctx->sock);
            }
          }
        }
        delete[] entries;
      }));
    }
  }
  // Prepares context data for later use
  bool PrepareContextForUse_(Context_* context, const bool& close_it = false) {
    DWORD dw_chars;
    do {
      // let's close the socket resources (if needed)..
      if (close_it) {
        if (TransmitFile(context->sock, NULL, 0, 0, nullptr, nullptr,
                         TF_DISCONNECT | TF_REUSE_SOCKET) != TRUE) {
          // [error] -> trying to close socket!
          break;
        }
      }
      // we reset the 'status' flag to indicate that this connection is not
      // a valid (accepted) one, then we start the 'accept' procedure..
      context->status = 0;
      auto accept_res = fn_accept_ex_(
          listen_socket_, context->sock, context->wsa_recv_data.buf,
          transport::ServerTransportConstants::kDefaultReadBufferSize -
              ((sizeof(sockaddr_in) + 16) * 2),
          sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, &dw_chars,
          context);
      if (accept_res == FALSE && WSAGetLastError() != WSA_IO_PENDING) {
        // [error] -> trying to add socket to the accept list!
        break;
      }
      // ok, let's associate the accept socket with the i/o port!
      if (CreateIoCompletionPort((HANDLE)context->sock, io_port_,
                                 (ULONG_PTR)context->sock, 0) == nullptr) {
        // [error] -> trying to associate socket to the i/o port!
        break;
      }
      return true;
    } while (false);
    return false;
  }
  // ---------------------------------------------------------------------------
  // Attributes                                                      [ private ]
  // ---------------------------------------------------------------------------
  HANDLE io_port_;
  SOCKET listen_socket_;
  std::queue<std::shared_ptr<std::thread>> threads_;
  std::queue<Context_*> contexts_;
  LPFN_ACCEPTEX fn_accept_ex_;
  typename Interface::OnAcceptConnection on_accept_connection_;
  typename Interface::OnConnectionAccepted on_connection_accepted_;
  typename Interface::OnConnectionRejected on_connection_rejected_;
  typename Interface::OnData on_data_;
};
}  // namespace koobika::hook::network::transport::tcpip

#endif