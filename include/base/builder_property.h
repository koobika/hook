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

#ifndef koobika_hook_base_builderproperty_h
#define koobika_hook_base_builderproperty_h

namespace koobika::hook::base {
// =============================================================================
// BuilderProperty                                                     ( class )
// -----------------------------------------------------------------------------
// This specification holds for builder property class.
// -----------------------------------------------------------------------------
// Template parameters:
//    DAty - data type of the property being stored.
//    OBty - target obtect type being built.
// =============================================================================
template <typename DAty, typename OBty>
class BuilderProperty {
 public:
  // ---------------------------------------------------------------------------
  // CONSTRUCTORs/DESTRUCTORs                                         ( public )
  // ---------------------------------------------------------------------------
  BuilderProperty(OBty* const parent) : parent_{parent}, data_{DAty()} {}
  BuilderProperty(OBty* const parent, const DAty& default_value)
      : parent_{parent}, data_{default_value} {}
  BuilderProperty(const BuilderProperty&) = default;
  BuilderProperty(BuilderProperty&&) noexcept = default;
  // ---------------------------------------------------------------------------
  // OPERATORs                                                        ( public )
  // ---------------------------------------------------------------------------
  BuilderProperty& operator=(const BuilderProperty&) = default;
  BuilderProperty& operator=(BuilderProperty&&) noexcept = default;
  operator DAty() const { return data_; }
  OBty& operator()(const DAty& in) {
    data_ = in;
    return *parent_;
  }

 private:
  // ---------------------------------------------------------------------------
  // ATTRIBUTEs                                                      ( private )
  // ---------------------------------------------------------------------------
  OBty* const parent_;
  DAty data_;
};
}  // namespace koobika::hook::base

#endif