// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 02.full_cycle.cpp
// -----------------------------------------------------------------------------
// Copyright (c) 2020 koobika corporation. All rights reserved.
// Author: Marcos Rojas (marcos.rojas@koobika.io)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <iostream>

#include "base/buffer.h"
#include "encoding/base64/encoder.h"
#include "encoding/base64/decoder.h"

using namespace koobika::hook::base;
using namespace koobika::hook::encoding;

int main() {
  try {
    // try standard (base64) encode/decode..
    Buffer initial = "hello base64 world!";
    auto encoded = base64::Encoder::Encode(initial);
    auto decoded = base64::Decoder::Decode(encoded);
    std::cout << "initial: " << initial.ToStringView() << std::endl
              << "encoded(base64): " << encoded << std::endl
              << "decoded(base64): " << decoded.ToStringView() << std::endl;
    // try standard (base64Url) encode/decode..
    encoded = base64::Encoder::Encode(initial, base64::EncoderType::Base64Url);
    decoded = base64::Decoder::Decode(encoded);
    std::cout << "initial: " << initial.ToStringView() << std::endl
              << "encoded(base64Url): " << encoded << std::endl
              << "decoded(base64Url): " << decoded.ToStringView() << std::endl;
  } catch (const std::exception& exception) {
    // [error] -> while performing required operations!
    std::cout << exception.what() << std::endl;
    return -1;
  }
  return 0;
}
