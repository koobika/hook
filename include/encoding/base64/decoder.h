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

#ifndef koobika_hook_encoding_base64_decoder_h
#define koobika_hook_encoding_base64_decoder_h

#include <stdexcept>
#include <string>

#include "constants.h"

namespace koobika::hook::encoding::base64 {
// =============================================================================
// Decoder                                                             ( class )
// -----------------------------------------------------------------------------
// This specification holds for the base64 decoder delegate.
// =============================================================================
class Decoder {
 public:
  // ---------------------------------------------------------------------------
  // METHODs                                                          ( public )
  // ---------------------------------------------------------------------------
  // Returns the encoded content.
  static std::string Decode(const std::string& in) {
    static const std::unordered_map<char, char> map = {
        {'A', 0x00}, {'B', 0x01}, {'C', 0x02}, {'D', 0x03}, {'E', 0x04},
        {'F', 0x05}, {'G', 0x06}, {'H', 0x07}, {'I', 0x08}, {'J', 0x09},
        {'K', 0x0A}, {'L', 0x0B}, {'M', 0x0C}, {'N', 0x0D}, {'O', 0x0E},
        {'P', 0x0F}, {'Q', 0x10}, {'R', 0x11}, {'S', 0x12}, {'T', 0x13},
        {'U', 0x14}, {'V', 0x15}, {'W', 0x16}, {'X', 0x17}, {'Y', 0x18},
        {'Z', 0x19}, {'a', 0x1A}, {'b', 0x1B}, {'c', 0x1C}, {'d', 0x1D},
        {'e', 0x1E}, {'f', 0x1F}, {'g', 0x20}, {'h', 0x21}, {'i', 0x22},
        {'j', 0x23}, {'k', 0x24}, {'l', 0x25}, {'m', 0x26}, {'n', 0x27},
        {'o', 0x28}, {'p', 0x29}, {'q', 0x2A}, {'r', 0x2B}, {'s', 0x2C},
        {'t', 0x2D}, {'u', 0x2E}, {'v', 0x2F}, {'w', 0x30}, {'x', 0x31},
        {'y', 0x32}, {'z', 0x33}, {'0', 0x34}, {'1', 0x35}, {'2', 0x36},
        {'3', 0x37}, {'4', 0x38}, {'5', 0x39}, {'6', 0x3A}, {'7', 0x3B},
        {'8', 0x3C}, {'9', 0x3D}, {'+', 0x3E}, {'/', 0x3F}, {'-', 0x3E},
        {'_', 0x3F}};
    std::string decoded;
    auto input_length = in.length();
    if (input_length % 4) {
      // ((Error)) -> input data is not 24-bit multiple!
      throw std::logic_error("invalid input data (not 24-bit multiple)!");
    }
    for (std::size_t i = 0; i < input_length; i += 4) {
      auto pad = Constants::kPadCh;
      // [first] byte extraction..
      auto l_idx = i;
      auto r_idx = i + 1;
      auto l_itr = map.find(in[l_idx]);
      auto r_itr = map.find(in[r_idx]);
      if ((l_itr == map.end() && in[l_idx] != pad) ||
          (r_itr == map.end() && in[r_idx] != pad)) {
        // ((Error)) -> invalid base64 character!
        throw std::logic_error("invalid base64 character found!");
      }
      auto l = in[l_idx] == pad ? pad : l_itr->second;
      auto r = in[r_idx] == pad ? pad : r_itr->second;
      int fB_i = ((l & 0b00111111) << 2) | ((r & 0b00110000) >> 4);
      decoded += (char)fB_i;
      // [second] byte extraction..
      r_idx = i + 2;
      if (in[r_idx] == Constants::kPadCh) break;
      r_itr = map.find(in[r_idx]);
      if (r_itr == map.end() && in[r_idx] != pad) {
        // ((Error)) -> invalid base64 character!
        throw std::logic_error("invalid base64 character found!");
      }
      l = r;
      r = in[r_idx] == pad ? pad : r_itr->second;
      int sB_i = ((l & 0b00001111) << 4) | ((r & 0b00111100) >> 2);
      decoded += (char)sB_i;
      // [third] byte extraction..
      r_idx = i + 3;
      if (in[r_idx] == Constants::kPadCh) break;
      r_itr = map.find(in[r_idx]);
      if (r_itr == map.end() && in[r_idx] != pad) {
        // ((Error)) -> invalid base64 character!
        throw std::logic_error("invalid base64 character found!");
      }
      l = r;
      r = in[r_idx] == pad ? pad : r_itr->second;
      int tB_i = ((l & 0b00000011) << 6) | (r & 0b00111111);
      decoded += (char)tB_i;
    }
    return decoded;
  }
};
}  // namespace koobika::hook::encoding::base64

#endif