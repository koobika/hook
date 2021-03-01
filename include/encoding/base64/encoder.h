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

#ifndef koobika_hook_encoding_base64_encoder_h
#define koobika_hook_encoding_base64_encoder_h

#include <string>

#include "constants.h"
#include "encoder_type.h"

namespace koobika::hook::encoding::base64 {
// =============================================================================
// Encoder                                                             [ class ]
// -----------------------------------------------------------------------------
// This specification holds for the base64 encoder delegate
// =============================================================================
class Encoder {
 public:
  // ---------------------------------------------------------------------------
  // Methods                                                          [ public ]
  // ---------------------------------------------------------------------------
  // Returns the encoded content (buffer-based)
  static std::string Encode(
      const std::string& in,
      const EncoderType& encoder_type = EncoderType::Base64) {
    std::string encoded;
    const char* alphabet = encoder_type == EncoderType::Base64
                               ? Constants::kBase64EncodingAlphabet
                               : Constants::kBase64UrlEncodingAlphabet;
    // check if input data is less than 24-bit
    bool byte_added = ((in.length() * 8) % 6);
    // encode input data
    const char* input_buffer_str = (const char*)in.data();
    std::size_t input_buffer_str_ori_len = in.length();
    std::size_t input_buffer_str_len = input_buffer_str_ori_len + byte_added;
    unsigned char r = 0;
    static constexpr unsigned char c_disps[] = {0x02, 0x04, 0x06};
    static constexpr unsigned char r_disps[] = {0x04, 0x02, 0x00};
    static constexpr unsigned char c_masks[] = {0xFC, 0xF0, 0xC0};
    static constexpr unsigned char r_masks[] = {0x03, 0x0F, 0x3F};
    std::size_t elements = sizeof(c_masks);
    for (std::size_t i = 0; i < input_buffer_str_len; i++) {
      bool last_element = (i == (input_buffer_str_len - 1));
      bool last_loop_element = (i && (i % elements) == 0);
      if (last_loop_element) {
        encoded += alphabet[r];
        r = 0;
      }
      unsigned char val =
          last_element ? byte_added ? Constants::kZeroCh : input_buffer_str[i]
                       : input_buffer_str[i];
      auto this_byte_mask = c_masks[i % elements];
      auto remainder_mask = r_masks[i % elements];
      auto index = ((val & this_byte_mask) >> c_disps[i % elements]) | r;
      r = (val & remainder_mask) << r_disps[i % elements];
      encoded += alphabet[index];
    }
    if (r) encoded += alphabet[r];
    // check for final padding..
    if (input_buffer_str_ori_len % 3) {
      while (encoded.length() % 4) {
        encoded += Constants::kPadCh;
      }
    }
    return encoded;
  }
};
}  // namespace koobika::hook::encoding::base64

#endif