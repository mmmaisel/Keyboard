/*****************************************************************************\
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
#include "uart_receiver.h"

#include "event.h"
#include "uart_serde.h"

UartReceiver uart1_receiver(&uart1);
UartReceiver uart2_receiver(&uart2);
UartReceiver uart6_receiver(&uart6);

UartReceiver::UartReceiver(Uart* uart) :
    _uart(uart),
    _key_ctr(0),
    _cmd_ctr(0)
{
}

void UartReceiver::task() {
    for(;;) {
        UartMessage* msg = reinterpret_cast<UartMessage*>(&_buffer);
        BYTE len = _uart->read(_buffer, BUFFER_SIZE);

        if(!msg->is_valid(len))
            continue;

        // TODO: ignore keys message on out modules
        // TODO: ignore led message on main module
        if(len == UartMessage::KEY_MSG_LEN && msg->type() == EVENT_KEYS) {
            if(msg->ctr() == _key_ctr)
                continue;
            _key_ctr = msg->ctr();

            Event event = {
                .type = EVENT_KEYS,
                .keys = msg->key_event(),
            };
            EventDispatcher::send(&event);
        } else if(
            len == UartMessage::EFF_MSG_LEN && msg->type() == EVENT_EFFECT)
        {
            if(msg->ctr() == _cmd_ctr)
                continue;
            _cmd_ctr = msg->ctr();

            Event event = {
                .type = EVENT_EFFECT,
                .effect = msg->effect_event(),
            };
            EventDispatcher::send(&event);
        }
    }
}

void UartReceiver::task_trampoline(void* pContext) {
    UartReceiver* rx = reinterpret_cast<UartReceiver*>(pContext);
    rx->task();
}
