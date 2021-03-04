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

#ifndef koobika_hook_base_stream_h
#define koobika_hook_base_stream_h

#include <stdio.h>

#include <mutex>
#include <string>
#include <variant>

namespace koobika::hook::base {
// =============================================================================
// Stream                                                              ( class )
// -----------------------------------------------------------------------------
// This specification holds for the stream class.
// =============================================================================
class Stream {
 public:
  // ---------------------------------------------------------------------------
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  // ---------------------------------------------------------------------------
  Stream() = default;
  Stream(const std::string& str) { Write(str); }
  Stream(const char& character) { Write(character); }
  Stream(const char* str) { Write(str); }
  Stream(const char* str, const std::size_t& length) { Write(str, length); }
  Stream(void* buf, const std::size_t& length) { Write(buf, length); }
  Stream(const Stream& stream) { operator=(stream); }
  Stream(Stream&& stream) noexcept { operator=(std::move(stream)); }
  ~Stream() { Close(); }
  // ---------------------------------------------------------------------------
  // OPERATORs                                                        ( public )
  // ---------------------------------------------------------------------------
  Stream& operator=(const Stream& stream) {
    data_.read_cursor = 0;
    data_.write_cursor = 0;
    return Write(stream);
  }
  Stream& operator=(Stream&& stream) noexcept {
    Close();
    stream.Flush();
    data_ = stream.data_;
    memory_mode_ = stream.memory_mode_;
    memory_limit_ = stream.memory_limit_;
    stream.data_.buffer = nullptr;
    stream.data_.buffer_size = 0;
    stream.data_.read_cursor = 0;
    stream.data_.write_cursor = 0;
    stream.data_.file = nullptr;
    stream.data_.filename.clear();
    stream.memory_limit_ = kDefaultMemoryBufferLimit;
    stream.memory_mode_ = true;
    return *this;
  }
  // ---------------------------------------------------------------------------
  // METHODs                                                          ( public )
  // ---------------------------------------------------------------------------
  // Writes the specified Stream to the internal buffer.
  Stream& Write(const Stream& stream) {
    stream.Flush();
    char tmp_buffer[kReadSomeBufferSize];
    while (auto sz = stream.ReadSome(tmp_buffer, kReadSomeBufferSize)) {
      Write_(tmp_buffer, sz);
    }
    return *this;
  }
  // Writes the specified std::string to the internal buffer.
  Stream& Write(const std::string& str) {
    return Write_((void*)str.data(), str.length());
  }
  // Writes the specified char to the internal buffer.
  Stream& Write(const char& character) { return Write_((void*)character, 0x1); }
  // Writes the specified char* to the internal buffer.
  Stream& Write(const char* str) { return Write_((void*)str, strlen(str)); }
  // Writes the specified char* + length to the internal buffer.
  Stream& Write(const char* str, const std::size_t& length) {
    return Write_((void*)str, length);
  }
  // Writes the specified void* + length to the internal buffer.
  Stream& Write(void* buffer, const std::size_t& length) {
    return Write_(buffer, length);
  }
  // Closes current stream.
  void Close() {
    if (memory_mode_) {
      free(data_.buffer);
      data_.buffer = nullptr;
      data_.buffer_size = 0;
      data_.read_cursor = 0;
      data_.write_cursor = 0;
    } else {
      if (data_.file != nullptr) {
        if (!fclose(data_.file)) {
          if (std::remove(data_.filename.c_str())) {
            // [error] -> trying to remove the file!
            // [to-do] -> raise an exception?
          }
          data_.filename.clear();
          data_.file = nullptr;
        } else {
          // [error] -> trying to close the file!
          // [to-do] -> raise an exception?
        }
      }
    }
  }
  // Returns the effective length of this stream.
  std::size_t Length() const {
    std::size_t length = 0;
    if (memory_mode_) {
      // we always take the writer cursor as the limit for reading!
      length = data_.write_cursor;
    } else {
      auto current = ftell(data_.file);
      if (!fseek(data_.file, 0, SEEK_END)) {
        length = ftell(data_.file);
        if (fseek(data_.file, current, SEEK_SET)) {
          // [error] -> trying to perform seek operation!
          // [to-do] -> raise an exception?
        } else {
          // [error] -> trying to perform seek operation!
          // [to-do] -> raise an exception?
        }

      } else {
        // [error] -> trying to perform seek operation!
        // [to-do] -> raise an exception?
      }
    }
    return length;
  }
  // Reads the specified user bytes (if available).
  std::size_t ReadSome(void* user_buffer, const std::size_t& length) const {
    std::size_t sz = 0;
    sz = std::min(length, Length() - data_.read_cursor);
    if (memory_mode_) {
      memcpy(user_buffer, &((const char*)data_.buffer)[data_.read_cursor], sz);
    } else {
      if (!fseek(data_.file, (long)data_.read_cursor, SEEK_SET)) {
        sz = fread(user_buffer, 1, sz, data_.file);
      } else {
        // [error] -> trying to perform seek operation!
        // [to-do] -> raise an exception?
      }
    }
    data_.read_cursor += sz;
    return sz;
  }
  // Flushes stream content (if required).
  void Flush() const {
    if (!memory_mode_ && data_.file != nullptr) {
      if (fflush(data_.file)) {
        // [error] -> trying to perform flush operation!
        // [to-do] -> raise an exception?
      }
    }
  }

 private:
  // ---------------------------------------------------------------------------
  // CONSTANTs                                                       ( private )
  // ---------------------------------------------------------------------------
  static constexpr std::size_t kDefaultMemoryBufferLimit = 65536;
  static constexpr std::size_t kReadSomeBufferSize = 4096;
  static constexpr std::size_t kMaxFilenameLength = 32;
  static constexpr char kFilenameBase_[] = "stream_tmp_";
  static constexpr char kFilenameExt_[] = "dat";
  // ---------------------------------------------------------------------------
  // STATIC-ATTRIBUTEs                                               ( private )
  // ---------------------------------------------------------------------------
  static inline int32_t counter_ = 0;
  // ---------------------------------------------------------------------------
  // TYPEs                                                           ( private )
  // ---------------------------------------------------------------------------
  struct Data {
    // Memory buffer.
    void* buffer = nullptr;
    // Memory buffer size.
    std::size_t buffer_size = 0;
    // Disk buffer.
    FILE* file = nullptr;
    // Disk buffer filename.
    std::string filename;
    // Read cursor.
    std::size_t read_cursor = 0;
    // Write cursor.
    std::size_t write_cursor = 0;
  };
  // Gets next buffer filename.
  static std::string GetNextFilename_() {
    static std::mutex counter_lock_;
    static char filename[kMaxFilenameLength] = {0};
    std::unique_lock<std::mutex> unique{counter_lock_};
    auto result = snprintf(filename, kMaxFilenameLength, "%s%010d.%s",
                           kFilenameBase_, counter_++, kFilenameExt_);
    if (result <= 0 || result >= kMaxFilenameLength) {
      // [error] -> trying to build filename!
      // [to-do] -> raise an exception?
    }
    return filename;
  }
  // Allocates the needed space to allow required length.
  void Allocate_(const std::size_t& length) {
    constexpr std::size_t kDefaultBufferChunkSize = 4096;
    std::size_t off = data_.write_cursor + length;
    if (off > data_.buffer_size) {
      auto amount = std::max(off - data_.buffer_size, kDefaultBufferChunkSize);
      auto new_buffer = (data_.buffer == nullptr)
                            ? malloc(amount)
                            : realloc(data_.buffer, data_.buffer_size + amount);
      if (new_buffer == nullptr) {
        // [error] -> trying to allocate memory!
        // [to-do] -> raise an exception?
        return;
      }
      data_.buffer = (char*)new_buffer;
      data_.buffer_size += amount;
    }
  }
  // Checks for the current size (in memory) and switches to disk (if needed).
  void CheckAndSwitch_(const std::size_t& length) {
    if (memory_mode_) {
      if ((data_.write_cursor + length) > kDefaultMemoryBufferLimit) {
        auto filename = GetNextFilename_();
        FILE* file = nullptr;
        errno_t result = fopen_s(&file, filename.c_str(), "w+b");
        if (!result) {
          auto old_buffer = data_.buffer;
          auto old_length = Length();
          memory_mode_ = false;
          data_.file = file;
          data_.filename = filename;
          data_.read_cursor = 0;
          data_.write_cursor = 0;
          if (old_buffer != nullptr) {
            Write_(old_buffer, old_length);
            free(old_buffer);
          }
          data_.buffer = nullptr;
          data_.buffer_size = 0;
        } else {
          // [error] -> trying to open file!
          // [to-do] -> raise an exception?
        }
      }
    } else if (data_.file == nullptr) {
      FILE* file = nullptr;
      auto filename = GetNextFilename_();
      errno_t result = fopen_s(&file, filename.c_str(), "w+b");
      if (!result) {
        data_.file = file;
        data_.filename = filename;
      } else {
        // [error] -> trying to open file!
        // [to-do] -> raise an exception?
      }
    }
  }
  // Adds the specified buffer fragment to the internal decoder data.
  Stream& Write_(void* buffer, const std::size_t& length) {
    CheckAndSwitch_(length);
    if (memory_mode_) {
      Allocate_(length);
      memcpy(&((char*)data_.buffer)[data_.write_cursor], buffer, length);
    } else {
      std::size_t written = 0;
      const char* buf = (const char*)buffer;
      if (!fseek(data_.file, (long)data_.write_cursor, SEEK_SET)) {
        while (written < length) {
          written += fwrite(&buf[written], 1, length - written, data_.file);
        }
      } else {
        // [error] -> trying to perform seek operation!
        // [to-do] -> raise an exception?
      }
    }
    data_.write_cursor += length;
    return *this;
  }
  // ---------------------------------------------------------------------------
  // ATTRIBUTEs                                                      ( private )
  // ---------------------------------------------------------------------------
  // Controls the current mode (memory/disk). By default: memory.
  bool memory_mode_ = true;
  // Memory buffer limit.
  std::size_t memory_limit_ = kDefaultMemoryBufferLimit;
  // Stores the information withing this stream.
  mutable Data data_;
};
}  // namespace koobika::hook::base

#endif