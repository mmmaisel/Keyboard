/**********************************************************************\
 * Keyboard
 *
 * UART protocol class
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
#include "uart_protocol.h"
#include "modular_keyboard.h"

void UartProtocol::send_key_page(KeyMatrix::Page& page) {
    if(page.keys[0] == 0)
        return;

    page.id |= MSG_KEYS;
    Uart1.write(reinterpret_cast<BYTE*>(&page), sizeof(KeyMatrix::Page));
    page.id &= ~ MSG_KEYS;
}
