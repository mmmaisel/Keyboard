/**********************************************************************\
 * Keyboard
 *
 * Modular keyboard class
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
#include "key_matrix.h"
#include "led_matrix.h"

class ModularKeyboard {
    // Static class
    ModularKeyboard() = delete;
    ModularKeyboard(const ModularKeyboard&) = delete;
    ModularKeyboard(ModularKeyboard&&) = delete;
    ~ModularKeyboard() = delete;

    public:
        static const BYTE PAGE_COUNT = 4;
        static const BYTE BUFFER_SIZE = PAGE_COUNT * KeyMatrix::MAX_KEYS;

        [[noreturn]] static void task(void* pContext);

        static void update_keys(BYTE page, const BYTE* buffer);
        static void get_keys(BYTE* buffer);
        static void set_led(LedMatrix::Led led);

    private:
        enum : BYTE {
            MSG_KEYS = 0x10,
            MSG_LEDS = 0x20,
            MSG_TYPE_MASK = 0xF0,
            MSG_PAGE_MASK = 0x0F
        };

        static BYTE m_keys[PAGE_COUNT][KeyMatrix::MAX_KEYS];

        static void process_keys(BYTE* buffer);
};
