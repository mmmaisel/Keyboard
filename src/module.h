/******************************************************************************\
    Split Keyboard
    Copyright (C) 2022-2025 - Max Maisel

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

#include "types.h"

class Module {
    // Static class
    Module() = delete;
    Module(const Module&) = delete;
    Module(Module&&) = delete;
    ~Module() = delete;

    public:
        enum {
            RIGHT,
            LEFT,
            NAV,
            NUM
        };

        /// Detects module through GPIO straps
        static void detect();
        /// Get current board type
        static inline BYTE get_id() { return m_module; }

    private:
        static BYTE m_module;
};
