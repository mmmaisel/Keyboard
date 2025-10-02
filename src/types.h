/******************************************************************************\
    Split Keyboard
    Copyright (C) 2019-2025 - Max Maisel

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
\******************************************************************************/
#pragma once
/// @file
/// @brief Definitions of common (int) types for easier reading.

#include <stdint.h>

// ARM processor dependent types
typedef uint16_t HWORD;
typedef uint32_t WORD;
typedef uint64_t DWORD;

// definition of common (int) types
typedef int8_t      TINY;
typedef uint8_t     BYTE;

typedef int16_t     SHORT;
typedef uint16_t    USHORT;

typedef int32_t     LONG;
typedef uint32_t    ULONG;

typedef int64_t     BIG;
typedef uint64_t    UBIG;

inline constexpr BYTE operator "" _u8(unsigned long long arg) noexcept {
    return static_cast<BYTE>(arg);
}
