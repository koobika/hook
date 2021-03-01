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

#ifndef koobika_hook_base_repositorymemory_h
#define koobika_hook_base_repositorymemory_h

#include <unordered_map>

#include "repository.h"

namespace koobika::hook::base {
// =============================================================================
// RepositoryMemory                                                    [ class ]
// -----------------------------------------------------------------------------
// This specification holds for the <memory-based> repository.
// -----------------------------------------------------------------------------
// Template parameters:
//    KEty - key type.
//    VAty - value type.
// =============================================================================
template <typename KEty, typename VAty>
class RepositoryMemory : public Repository<KEty, VAty> {
 public:
  // ---------------------------------------------------------------------------
  // Constructors/Destructors                                         [ public ]
  // ---------------------------------------------------------------------------
  RepositoryMemory() = default;
  RepositoryMemory(const std::initializer_list<std::pair<KEty, VAty>>& in) {
    for (auto const& element : in) {
      set(element.first, element.second);
    }
  }
  RepositoryMemory(const RepositoryMemory&) = default;
  RepositoryMemory(RepositoryMemory&&) noexcept = default;
  ~RepositoryMemory() = default;
  // ---------------------------------------------------------------------------
  // Operators                                                        [ public ]
  // ---------------------------------------------------------------------------
  RepositoryMemory& operator=(const RepositoryMemory&) = default;
  RepositoryMemory& operator=(RepositoryMemory&&) noexcept = default;
  // ---------------------------------------------------------------------------
  // Methods                                                          [ public ]
  // ---------------------------------------------------------------------------
  // Sets specified key/value pair.
  void Set(const KEty& key, const VAty& value) override { map_[key] = value; }
  // Erases specified key.
  void Erase(const KEty& key) override { map_.erase(key); }
  // Clears all the information.
  void Clear() override { map_.clear(); }
  // Gets the specified value using its key.
  std::optional<VAty> Get(const KEty& key) const override {
    auto itr = map_.find(key);
    return itr != map_.end() ? itr->second : std::optional<VAty>{};
  }

 private:
  // ---------------------------------------------------------------------------
  // Attributes                                                      [ private ]
  // ---------------------------------------------------------------------------
  std::unordered_map<KEty, VAty> map_;
};
}  // namespace koobika::hook::base

#endif