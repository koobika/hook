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
// base/associative_container.h
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

#ifndef koobika_hook_base_associative_container_h
#define koobika_hook_base_associative_container_h

#include <utility>

namespace koobika::hook::base {
// =============================================================================
// AssociativeContainer                                            ( interface )
// -----------------------------------------------------------------------------
// This specification holds for associative container interface.
// =============================================================================
template <typename KEty, typename VAty>
class AssociativeContainer {
 public:
  // ___________________________________________________________________________
  // USINGs                                                           ( public )
  //
  using Key = KEty;
  using Value = VAty;
  // ___________________________________________________________________________
  // OPERATORs                                                        ( public )
  //
  virtual const Value& operator[](Key&&) const = 0;
  virtual const Value& operator[](const Key&) const = 0;
  virtual Value& operator[](Key&&) = 0;
  virtual Value& operator[](const Key&) = 0;
  // ___________________________________________________________________________
  // METHODs                                                          ( public )
  //
  // Adds the specified tuple to the object.
  virtual void Add(const std::pair<Key, Value>&) = 0;
  virtual void Add(std::pair<Key, Value>&&) = 0;
  // Erases the specified value (located by key) from the object.
  virtual void Erase(const Key&) = 0;
  virtual void Erase(Key&&) = 0;
  // Checks for an existent key.
  virtual bool Exist(const Key& key) const = 0;
  virtual bool Exist(Key&&) const = 0;
  // Returns the associated value for an existent key.
  virtual const Value& Get(const Key&) const = 0;
  virtual const Value& Get(Key&&) const = 0;
  // Returns the associated value for an existent key.
  virtual Value& Get(const Key&) = 0;
  virtual Value& Get(Key&&) = 0;
};
}  // namespace koobika::hook::base

#endif