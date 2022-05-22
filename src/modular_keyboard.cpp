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
#include "module.h"
#include "hid_keyboard_endpoint.h"
#include "uart.h"

#include <cstring>

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
    // XXX: store led state in flash?
    switch(data & MSG_TYPE_MASK) {
        case MSG_KEYS: {
            BYTE page = data & MSG_PAGE_MASK;
            BYTE buffer[KeyMatrix::MAX_KEYS];
            uart->read(buffer, KeyMatrix::MAX_KEYS);
            update_keys(page, buffer);
            break;
        }
        case MSG_LEDS: {
            BYTE page = data & MSG_PAGE_MASK;
            // XXX: map pages to individual LEDs
            // XXX: 16 LEDs per page, 16 pages, 3 byte per LED
            BYTE buffer[16*3];
            uart->read(buffer, 16*3);
            break;
        }
    }
}

void ModularKeyboard::update_keys(BYTE page, const BYTE* buffer) {
    if(Module::get_id() == Module::RIGHT) {
        memcpy(m_keys[page], buffer, KeyMatrix::MAX_KEYS);
        if(page == 0)
            // XXX: process some fn keys on master before
            ep1.send_report(&m_keys[0][0]);
    } else {
        BYTE buffer[KeyMatrix::MAX_KEYS+1];
        buffer[0] = MSG_KEYS | Module::get_id();
        memcpy(buffer+1, buffer, KeyMatrix::MAX_KEYS);
        Uart1.write(buffer, KeyMatrix::MAX_KEYS+1);
    }
}

void ModularKeyboard::get_keys(BYTE* buffer) {
    // XXX: needs sync: disable all uart rx ISRs and key matrix
    // XXX: process some fn keys on master
    memcpy(buffer, &m_keys[0][0], BUFFER_SIZE);
}
