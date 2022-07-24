/**********************************************************************\
 * Keyboard
 *
 * Key matrix
 **********************************************************************
 * Copyright (C) 2022 - Max Maisel
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
\**********************************************************************/
#pragma once

#include "types.h"
#include "dev/gpio.h"

extern "C" void tim3_vector() __attribute__((error("calling ISR")));

class KeyMatrix {
    // Static class
    KeyMatrix() = delete;
    KeyMatrix(const KeyMatrix&) = delete;
    KeyMatrix(KeyMatrix&&) = delete;
    ~KeyMatrix() = delete;

    friend void tim3_vector();

    public:
        static const BYTE MAX_DIM = 16;
        static const BYTE MAX_KEYS = 16;

        struct Page {
            static const BYTE SIZE = MAX_KEYS;
            BYTE id;
            BYTE keys[MAX_KEYS];
        };

        static void initialize();
    private:
        struct Pin {
            volatile dev::GpioStruct* port;
            WORD pin;
        };

        static BYTE m_row_count;
        static BYTE m_column_count;
        static BYTE m_column;
        static Pin m_rows[MAX_DIM];
        static Pin m_columns[MAX_DIM];
        static BYTE m_phase;

        static const BYTE* m_key_layout;
        static BYTE m_key_state[MAX_DIM][MAX_DIM];
        static BYTE m_key_idx;
        static Page m_page;

        static void ISR();
};
