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
#include "modular_keyboard.h"
#include "hid_keyboard_endpoint.h"
#include "uart.h"

#include <cstring>

// XXX: only valid on master module
ModularKeyboard keyboard;

ModularKeyboard::ModularKeyboard() {
    memset(m_keys, 0, BUFFER_SIZE);
}

ModularKeyboard::~ModularKeyboard() {
}

void ModularKeyboard::operator delete(void* __attribute__((unused))) {
    /// Shut up stupid linker - there are no dynamic objects!!!
}

void ModularKeyboard::OnReceive(Uart* uart, BYTE data) {
    BYTE buffer[KeyMatrix::MAX_KEYS];
    uart->read(buffer, KeyMatrix::MAX_KEYS);
    if(uart == &Uart1) {
        update_keys(1, buffer);
    } else if(uart == &Uart2) {
        update_keys(2, buffer);
    } else if(uart == &Uart6) {
        update_keys(3, buffer);
    }
}

void ModularKeyboard::update_keys(BYTE page, const BYTE* buffer) {
    memcpy(m_keys[page], buffer, KeyMatrix::MAX_KEYS);
    if(page == 0)
        ep1.send_report(&m_keys[0][0]);
}

void ModularKeyboard::get_keys(BYTE* buffer) {
    // XXX: needs sync: disable all uart rx ISRs and key matrix
    memcpy(buffer, &m_keys[0][0], BUFFER_SIZE);
}
