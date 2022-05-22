/**********************************************************************\
 * Keyboard
 *
 * LED matrix
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

extern "C" void tim2_vector() __attribute__((error("calling ISR")));

class LedMatrix {
    // Static class
    LedMatrix() = delete;
    LedMatrix(const LedMatrix&) = delete;
    LedMatrix(LedMatrix&&) = delete;
    ~LedMatrix() = delete;

    friend void tim2_vector();

    public:
        static void initialize();
        static void set_led(BYTE row, BYTE column, BYTE red, BYTE green, BYTE blue);

    private:
        static const BYTE MAX_DIM = 16;
        static const BYTE PHASE_COUNT = 16;

        struct Pin {
            volatile dev::GpioStruct* port;
            WORD pin;
        };

        static BYTE m_phase;
        static BYTE m_row;
        static BYTE m_row_count;
        static BYTE m_column_count;
        static Pin m_rows[MAX_DIM];
        static Pin m_columns[MAX_DIM];
        static BYTE m_phases[MAX_DIM][MAX_DIM];

        static void ISR();
};
