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

#include "pin.h"

extern "C" void tim3_vector() __attribute__((error("calling ISR")));

struct KeyMatrixConfig {
    static const BYTE MAX_DIM = 16;
    static const BYTE MAX_KEYS = 64;

    BYTE page;
    BYTE row_count;
    BYTE col_count;
    Pin row_pins[MAX_DIM];
    Pin col_pins[MAX_DIM];

    /// Maps matrix coordinates to key numbers
    BYTE mapping[MAX_DIM][MAX_DIM];
};

class KeyMatrix {
    // Static class
    KeyMatrix() = delete;
    KeyMatrix(const KeyMatrix&) = delete;
    KeyMatrix(KeyMatrix&&) = delete;
    ~KeyMatrix() = delete;

    friend void tim3_vector();

    public:
        static void initialize(const KeyMatrixConfig* config);

    private:
        enum {
            PHASE_DRIVE,
            PHASE_READ,
        };
        static const BYTE STATE_CNT = 3;

        /// Reference to key matrix configuration
        static const KeyMatrixConfig* _config;
        /// Currently driven column
        static BYTE _col;
        /// Scan phases: drive and read
        static BYTE _phase;
        /// Last three key states as bitfield defined by mapping
        static DWORD _key_state[STATE_CNT];
        /// Index for _key_state
        static BYTE _state_idx;

        static void ISR();
};
