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

#include "event.h"

struct __attribute__((packed)) UartMessage {
    static const BYTE STATE_LEN = 5;
    static const BYTE KEY_MSG_LEN = 2 + STATE_LEN;

    // Fixed data
    BYTE hdr; // counter:4 type:2 page:2
    BYTE crc;

    // Variable data
    union {
        struct __attribute__((packed)) {
            BYTE state[STATE_LEN];
        } keys;
        struct __attribute__((packed)) {
            BYTE red_green;
            BYTE blue_x;
            BYTE state[STATE_LEN];
        } leds;
    };

    inline BYTE type() { return ((hdr & 0x0C) >> 2) + 1; }
    KeyEvent key_event();

    static UartMessage serialize_keys(BYTE ctr, BYTE page, DWORD state);
};
