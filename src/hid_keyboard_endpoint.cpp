/*********************************************************************\
 * Keyboard
 *
 * USB HID keyboard endpoint class
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
#include "hid_keyboard_endpoint.h"
#include "key_matrix.h"

#include "dev/usb.h"
#include "usb_endpoint.h"
#include "usb_phy.h"

#include "uart.h"
#include <string.h>

HidKeyboardEndpoint ep1;

HidKeyboardEndpoint::HidKeyboardEndpoint() :
    USBEndpoint(1),
    m_idle(0),
    m_transmitting(0)
{
}

HidKeyboardEndpoint::~HidKeyboardEndpoint() {
}

void HidKeyboardEndpoint::send_report(const BYTE* keys) {
    if(m_transmitting || !m_enabled)
        return;

    WORD buffer[2];
    make_report((BYTE*)buffer, keys);
    m_transmitting = 1;

    BYTE* pbuf = (BYTE*)buffer;
    for(BYTE i = 0; i < 8; ++i) {
        UART6.write(pbuf[i]);
    }
    USBPhy::TransmitData(m_epnum, buffer, 8);
}

void HidKeyboardEndpoint::make_report(BYTE* buffer, const BYTE* keys) {
    using namespace keycodes;
    memset(buffer, 0, 8);
    BYTE pos = 2;
    for(BYTE i = 0; i < KeyMatrix::MAX_KEYS; ++i) {
        switch(keys[i]) {
            case KEY_LEFTCTRL:
                buffer[0] |= MOD_LCTRL;
                break;
            case KEY_LEFTSHIFT:
                buffer[0] |= MOD_LSHIFT;
                break;
            case KEY_LEFTALT:
                buffer[0] |= MOD_LALT;
                break;
            case KEY_LEFTSUPER:
                buffer[0] |= MOD_LSUPER;
                break;
            case KEY_RIGHTCTRL:
                buffer[0] |= MOD_RCTRL;
                break;
            case KEY_RIGHTSHIFT:
                buffer[0] |= MOD_RSHIFT;
                break;
            case KEY_RIGHTALT:
                buffer[0] |= MOD_RALT;
                break;
            case KEY_RIGHTSUPER:
                buffer[0] |= MOD_RSUPER;
                break;
            default:
                if(pos < 8) {
                    buffer[pos++] = keys[i];
                }
                break;
        }
    }
}

void HidKeyboardEndpoint::OnTransmit() {
    m_transmitting = 0;
}
