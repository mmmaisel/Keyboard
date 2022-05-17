/*********************************************************************\
 * Keyboard
 *
 * USB Endpoint class
 **********************************************************************
 * Copyright (C) 2019-2022 - Max Maisel
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
#include "usb_endpoint.h"
#include "usb_control_endpoint.h"
#include "hid_keyboard_endpoint.h"
#include "usb_phy.h"

USBEndpoint::USBEndpoint(BYTE epnum) :
    m_epnum(epnum),
    m_enabled(0),
    m_bufferPos(0),
    m_length(0)
{
}

USBEndpoint::~USBEndpoint() {
}

void USBEndpoint::operator delete(void* __attribute__((unused))) {
    /// Shut up stupid linker - there are no dynamic objects!!!
}

void USBEndpoint::Enable(BYTE dir, WORD type) {
    if(dir & DIR_IN)
        USBPhy::EnableInEndpoint(m_epnum, type);
    if(dir & DIR_OUT) {
        USBPhy::EnableOutEndpoint(m_epnum, type);
        USBPhy::PrepareRX(m_epnum);
    }
    m_enabled = 1;
}

void USBEndpoint::Transmit(const WORD* pData, USHORT len) {
    USBPhy::TransmitData(m_epnum, pData, len);
}

void USBEndpoint::Receive(WORD* pData, USHORT len) {
}

BYTE USBEndpoint::TXFIFOEmpty() {
    return USBPhy::TXFIFOEmpty(m_epnum);
}

void USBEndpoint::OnReceive() {
}

void USBEndpoint::OnSetup() {
}

void USBEndpoint::OnTransmit() {
}

void USBEndpoint::OnRxData(const volatile WORD* data, USHORT len) {
    USHORT wcnt = (len + 3) / 4;
    for(int i = 0; i < wcnt; ++i) {
        if(m_bufferPos == BUFFER_SIZE)
            WORD _dummy = *data;
        else
            m_buffer.w[m_bufferPos++] = *data;
    };
    m_length += len;
}

USBEndpoint* eps[USB_ENDPOINT_COUNT] = {&ep0, &ep1};
