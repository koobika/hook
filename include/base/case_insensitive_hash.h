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
// base/case_insensitive_hash.h
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

#ifndef koobika_hook_base_caseinsensitivehash_h
#define koobika_hook_base_caseinsensitivehash_h

#include <algorithm>
#include <string>

namespace koobika::hook::base {
// =============================================================================
// CaseInsensitiveHash                                                 ( class )
// -----------------------------------------------------------------------------
// This specification holds for generic case insensitive hash generator.
// =============================================================================
struct CaseInsensitiveHash {
  // sdbm: this algorithm was created for sdbm (a public-domain
  // reimplementation of ndbm) database library. it was found to do
  // well in scrambling bits, causing better distribution of the keys
  // and fewer splits. it also happens to be a good general hashing
  // function with good distribution. the actual function is hash(i) =
  // hash(i - 1) * 65599 + str[i]; what is included below is the faster
  // version used in gawk. [there is even a faster, duff-device version] the
  // magic constant 65599 was picked out of thin air while experimenting with
  // different constants, and turns out to be a prime. this is one of the
  // algorithms used in berkeley db (see sleepycat) and elsewhere.
  size_t operator()(const std::string& k) const {
    std::size_t hash = 0;
    std::for_each(k.begin(), k.end(), [&](char c) {
      hash = ::tolower(c) + (hash << 6) + (hash << 16) - hash;
    });
    return hash;
  }
};
}  // namespace koobika::hook::base

#endif