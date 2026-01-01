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

extern "C" void tim2_vector() __attribute__((error("calling ISR")));

struct Color {
    BYTE red;
    BYTE green;
    BYTE blue;
};

struct LedMatrixConfig {
    static const BYTE MAX_DIM = 16;
    static const BYTE MAX_LED = 48;

    struct Coord {
        BYTE row;
        BYTE col;
    };

    BYTE rows;
    BYTE cols;
    BYTE nops;
    Pin row_pins[MAX_DIM];
    Pin col_pins[MAX_DIM];

    BYTE led_count;
    /// Maps LED numbers to coordinates
    Coord mapping[MAX_LED];
};

class LedMatrix {
    // Static class
    LedMatrix() = delete;
    LedMatrix(const LedMatrix&) = delete;
    LedMatrix(LedMatrix&&) = delete;
    ~LedMatrix() = delete;

    friend void tim2_vector();

    public:
        static void initialize(const LedMatrixConfig* config);
        static void clear();
        static void set_led(BYTE num, Color color);

    private:
        enum {
            PHASE_DRIVE,
            PHASE_NOP,
        };
        static const BYTE FRAC_COUNT = 16;

        /// Reference to LED matrix configuration
        static const LedMatrixConfig* _config;
        /// Drive of Nop
        static BYTE _phase;
        /// Fraction of LED on time between 0 and FRAC_COUNT
        static BYTE _frac;
        /// Currently driven column
        static BYTE _col;
        /// To be displayed colors by matrix coordinates
        static BYTE _colors[LedMatrixConfig::MAX_DIM][LedMatrixConfig::MAX_DIM];

        static void ISR();
};
