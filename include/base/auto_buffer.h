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
// base/auto_buffer.h
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

#ifndef koobika_hook_base_autobuffer_h
#define koobika_hook_base_autobuffer_h

#include "base/platform.h"

namespace koobika::hook::base {
// =============================================================================
// AutoBuffer                                                          ( class )
// -----------------------------------------------------------------------------
// This specification holds for the automatic buffer class.
// =============================================================================
class AutoBuffer {
 public:
  // ___________________________________________________________________________
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  //
  AutoBuffer() = default;
  AutoBuffer(const std::string& str) { Write(str); }
  AutoBuffer(std::istream& input_stream) { Write(input_stream); }
  AutoBuffer(const char& character) { Write(character); }
  AutoBuffer(const char* str) { Write(str); }
  AutoBuffer(const char* str, const std::size_t& length) { Write(str, length); }
  AutoBuffer(void* buf, const std::size_t& length) { Write(buf, length); }
  AutoBuffer(const AutoBuffer& buffer) { operator=(buffer); }
  AutoBuffer(AutoBuffer&& buffer) noexcept { operator=(std::move(buffer)); }
  ~AutoBuffer() { Close(); }
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  //
  AutoBuffer& operator=(const AutoBuffer& buffer) {
    data_.read_cursor = 0;
    data_.write_cursor = 0;
    return Write(buffer);
  }
  AutoBuffer& operator=(AutoBuffer&& buffer) noexcept {
    Close();
    buffer.Flush();
    data_ = buffer.data_;
    memory_mode_ = buffer.memory_mode_;
    memory_limit_ = buffer.memory_limit_;
    buffer.data_.buffer = nullptr;
    buffer.data_.buffer_size = 0;
    buffer.data_.read_cursor = 0;
    buffer.data_.write_cursor = 0;
    buffer.data_.file = nullptr;
    buffer.data_.filename.clear();
    buffer.memory_limit_ = kDefaultMemoryBufferLimit_;
    buffer.memory_mode_ = true;
    return *this;
  }
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  // Writes the specified AutoBuffer to the internal buffer.
  AutoBuffer& Write(const AutoBuffer& buffer) {
    buffer.Flush();
    char tmp_buffer[kReadSomeBufferSize_];
    while (auto sz = buffer.ReadSome(tmp_buffer, kReadSomeBufferSize_)) {
      write(tmp_buffer, sz);
    }
    return *this;
  }
  // Writes the specified std::istream to the internal buffer.
  AutoBuffer& Write(std::istream& input_stream) { return write(input_stream); }
  // Writes the specified std::string to the internal buffer.
  AutoBuffer& Write(const std::string& str) {
    return write((void*)str.data(), str.length());
  }
  // Writes the specified char to the internal buffer.
  AutoBuffer& Write(const char& character) {
    return write((void*)&character, 0x1);
  }
  // Writes the specified char* to the internal buffer.
  AutoBuffer& Write(const char* str) { return write((void*)str, strlen(str)); }
  // Writes the specified char* + length to the internal buffer.
  AutoBuffer& Write(const char* str, const std::size_t& length) {
    return write((void*)str, length);
  }
  // Writes the specified void* + length to the internal buffer.
  AutoBuffer& Write(void* buffer, const std::size_t& length) {
    return write(buffer, length);
  }
  // Closes current AutoBuffer.
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
            // ((Error)) -> trying to remove the file!
            // ((To-Do)) -> raise an exception?
          }
          data_.filename.clear();
          data_.file = nullptr;
        } else {
          // ((Error)) -> trying to close the file!
          // ((To-Do)) -> raise an exception?
        }
      }
    }
  }
  // Returns the effective length of this AutoBuffer.
  std::size_t Length() const {
    std::size_t length = 0;
    if (memory_mode_) {
      // we always take the writer cursor as the limit for reading!
      length = data_.write_cursor;
    } else if (data_.file != nullptr) {
      auto current = ftell(data_.file);
      if (!fseek(data_.file, 0, SEEK_END)) {
        length = ftell(data_.file);
        if (fseek(data_.file, current, SEEK_SET)) {
          // ((Error)) -> trying to perform seek operation!
          // ((To-Do)) -> raise an exception?
        } else {
          // ((Error)) -> trying to perform seek operation!
          // ((To-Do)) -> raise an exception?
        }

      } else {
        // ((Error)) -> trying to perform seek operation!
        // ((To-Do)) -> raise an exception?
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
        // ((Error)) -> trying to perform seek operation!
        // ((To-Do)) -> raise an exception?
      }
    }
    data_.read_cursor += sz;
    return sz;
  }
  // Reads all the stored bytes (if available).
  void ReadAll(std::string& out) const {
    char tmp_buffer[kReadSomeBufferSize_];
    while (auto sz = ReadSome(tmp_buffer, kReadSomeBufferSize_)) {
      out.append(tmp_buffer, sz);
    }
  }
  // Reads all the stored bytes (if available).
  std::string ToString() const {
    std::string out;
    ReadAll(out);
    return out;
  }
  // Reads all the stored bytes (if available).
  void ReadAll(std::stringstream& out) const {
    char tmp_buffer[kReadSomeBufferSize_];
    while (auto sz = ReadSome(tmp_buffer, kReadSomeBufferSize_)) {
      out << std::string_view{tmp_buffer, sz};
    }
  }
  // Reads all the stored bytes (if available).
  std::stringstream ToStringStream() const {
    std::stringstream out;
    ReadAll(out);
    return out;
  }
  // Flushes AutoBuffer content (if required).
  void Flush() const {
    if (!memory_mode_ && data_.file != nullptr) {
      if (fflush(data_.file)) {
        // ((Error)) -> trying to perform flush operation!
        // ((To-Do)) -> raise an exception?
      }
    }
  }

 private:
  // ___________________________________________________________________________
  // CONSTANTs                                                       ( private )
  //
  static constexpr std::size_t kDefaultMemoryBufferLimit_ = 65536;
  static constexpr std::size_t kReadSomeBufferSize_ = 4096;
  static constexpr std::size_t kMaxFilenameLength_ = 32;
  static constexpr char kFilenameBase_[] = "autobuffer_tmp_";
  static constexpr char kFilenameExt_[] = "dat";
  // ___________________________________________________________________________
  // STATIC-ATTRIBUTEs                                               ( private )
  //
  static inline int32_t counter_ = 0;
  // ___________________________________________________________________________
  // TYPEs                                                           ( private )
  //
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
  static std::string getNextFilename() {
    static std::mutex counter_lock_;
    static char filename[kMaxFilenameLength_] = {0};
    std::unique_lock<std::mutex> unique{counter_lock_};
    std::size_t result = snprintf(filename, kMaxFilenameLength_, "%s%010d.%s",
                                  kFilenameBase_, counter_++, kFilenameExt_);
    if (result <= 0 || result >= kMaxFilenameLength_) {
      // ((Error)) -> trying to build filename!
      // ((To-Do)) -> raise an exception?
    }
    return filename;
  }
  // Allocates the needed space to allow required length.
  void allocate(const std::size_t& length) {
    std::size_t off = data_.write_cursor + length;
    if (off > data_.buffer_size) {
      auto amount = std::max(off - data_.buffer_size, kDefaultBufferChunkSize_);
      auto new_buffer = (data_.buffer == nullptr)
                            ? malloc(amount)
                            : realloc(data_.buffer, data_.buffer_size + amount);
      if (new_buffer == nullptr) {
        // ((Error)) -> trying to allocate memory!
        // ((To-Do)) -> raise an exception?
        return;
      }
      data_.buffer = (char*)new_buffer;
      data_.buffer_size += amount;
    }
  }
  // Checks for the current size (in memory) and switches to disk (if needed).
  void checkAndSwitch(const std::size_t& length) {
    if (memory_mode_) {
      if ((data_.write_cursor + length) > kDefaultMemoryBufferLimit_) {
        auto filename = getNextFilename();
        FILE* file = fopen(filename.c_str(), "w+b");
        if (file) {
          auto old_buffer = data_.buffer;
          auto old_length = Length();
          memory_mode_ = false;
          data_.file = file;
          data_.filename = filename;
          data_.read_cursor = 0;
          data_.write_cursor = 0;
          if (old_buffer != nullptr) {
            write(old_buffer, old_length);
            free(old_buffer);
          }
          data_.buffer = nullptr;
          data_.buffer_size = 0;
        } else {
          // ((Error)) -> trying to open file!
          // ((To-Do)) -> raise an exception?
        }
      }
    } else if (data_.file == nullptr) {
      auto filename = getNextFilename();
      FILE* file = fopen(filename.c_str(), "w+b");
      if (file) {
        data_.file = file;
        data_.filename = filename;
      } else {
        // ((Error)) -> trying to open file!
        // ((To-Do)) -> raise an exception?
      }
    }
  }
  // Adds the specified buffer fragment to the internal decoder data.
  AutoBuffer& write(void* buffer, const std::size_t& length) {
    checkAndSwitch(length);
    if (memory_mode_) {
      allocate(length);
      memcpy(&((char*)data_.buffer)[data_.write_cursor], buffer, length);
    } else {
      std::size_t written = 0;
      const char* buf = (const char*)buffer;
      if (!fseek(data_.file, (long)data_.write_cursor, SEEK_SET)) {
        while (written < length) {
          written += fwrite(&buf[written], 1, length - written, data_.file);
        }
      } else {
        // ((Error)) -> trying to perform seek operation!
        // ((To-Do)) -> raise an exception?
      }
    }
    data_.write_cursor += length;
    return *this;
  }
  // Adds the specified buffer fragment to the internal decoder data.
  AutoBuffer& write(std::istream& input_stream) {
    input_stream.seekg(0, input_stream.end);
    std::size_t length = (std::size_t)input_stream.tellg();
    input_stream.seekg(0, input_stream.beg);
    char* buffer = new char[length];
    input_stream.read(buffer, length);
    write(buffer, length);
    delete[] buffer;
    return *this;
  }
  // ___________________________________________________________________________
  // CONSTANTs                                                       ( private )
  //
  static constexpr std::size_t kDefaultBufferChunkSize_ = 4096;

  // ___________________________________________________________________________
  // ATTRIBUTEs                                                      ( private )
  //
  bool memory_mode_ = true;
  // Memory buffer limit.
  std::size_t memory_limit_ = kDefaultMemoryBufferLimit_;
  // Stores the information withing this AutoBuffer.
  mutable Data data_;
};
}  // namespace koobika::hook::base

#endif