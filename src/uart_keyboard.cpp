/******************************************************************************\
    Split Keyboard
    Copyright (C) 2025 - Max Maisel

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
#include "uart_keyboard.h"

#include "event.h"
#include "uart.h"
#include "uart_protocol.h"

#include <cstring>

UartKeyboard uart_keyboard;

UartKeyboard::UartKeyboard() :
    _ctr(0)
{
}

void UartKeyboard::on_event(Event* event) {
    if(event->type != EVENT_KEYS)
        return;

    // TODO: fill counter with live and ACK
    UartMessage msg = UartMessage::serialize_keys(
        event->keys.page, _ctr, event->keys.state);
    uart1.write(reinterpret_cast<BYTE*>(&msg), sizeof(msg));
}
