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

#include "uart.h"

class UartReceiver {
    public:
        UartReceiver(Uart* uart);
        UartReceiver(const UartReceiver&) = delete;
        UartReceiver(UartReceiver&&) = delete;

        [[noreturn]] void task();
        [[noreturn]] static void task_trampoline(void* pContext);

    private:
        static const BYTE BUFFER_SIZE = 32;

        Uart* _uart;
        BYTE _buffer[BUFFER_SIZE];
};

extern UartReceiver uart1_receiver;
extern UartReceiver uart2_receiver;
extern UartReceiver uart6_receiver;
