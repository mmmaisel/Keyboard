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

namespace priority {
    // Lower number means higher priority
    enum : BYTE {
        MAX_SYSCALL = 4,
        /// Does not modify any values outside. Must drive hardware in time.
        LED_MATRIX = 7,
        /// USB priority during key value request.
        USB_KEY_REQUEST = 8,
        /// Updates key values in ModularKeyboard and triggers HID report transmission.
        /// Should scan hardware in time.
        KEY_MATRIX = 9,
        /// Updates key values in ModularKeyboard and LED values.
        UART = 8,
        /// Can request key values, change LED values and change used HID protocol.
        USB = 10,
        /// Kernel interrupt has lowest priority.
        KERNEL = 15,
    };

    // FreeRTOS task priorities
    enum : BYTE {
        KEYBOARD = 1,
        UARTRX = 2,
    };
}
