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
#include "uart_handler.h"
#include "key_matrix.h"

class Uart;

class ModularKeyboard : public UartHandler {
    public:
        ModularKeyboard();
        ModularKeyboard(const ModularKeyboard&) = delete;
        ModularKeyboard(ModularKeyboard&&) = delete;
        virtual ~ModularKeyboard();

        static const BYTE PAGE_COUNT = 4;
        static const BYTE BUFFER_SIZE = PAGE_COUNT * KeyMatrix::MAX_KEYS;

        // XXX: implement key transmitter and led receiver
        virtual void OnReceive(Uart* uart, BYTE data) override;
        void update_keys(BYTE page, const BYTE* buffer);
        void get_keys(BYTE* buffer);

    private:
        BYTE m_keys[PAGE_COUNT][KeyMatrix::MAX_KEYS];

        static void operator delete(void* __attribute__((unused)));
};

extern ModularKeyboard keyboard;
