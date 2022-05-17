/*********************************************************************\
 * Keyboard
 *
 * USB Control Endpoint class
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
#include "dev/usb.h"
#include "usb_endpoint.h"
#include "usb_phy.h"
#include "usb_descriptor.h"
#include "usb_control_endpoint.h"
#include "hid_keyboard_endpoint.h"

#include "key_matrix.h"
#include "uart.h"
#include <string.h>

ControlEndpoint ep0;

ControlEndpoint::ControlEndpoint() :
    USBEndpoint(0)
{
}

ControlEndpoint::~ControlEndpoint() {
}

void ControlEndpoint::OnReceive() {
    //SimpleUart::Write('R');
}

void ControlEndpoint::OnSetup() {
    //SimpleUart::Write('S');

    BYTE bmRequestType = m_buffer.b[0];
    BYTE bRequest = m_buffer.b[1];
    SHORT wValue = m_buffer.h[1];
    SHORT wIndex = m_buffer.h[2];
    SHORT wLength = m_buffer.h[3];

    // TODO: handle 3 back-to-back setup packets
    // TODO: get len from m_bufferPos
    // TODO: refactor buffer handling
    // XXX: all setup commands land on EP0 (HID, CDC set line state, ...)

    if(bmRequestType == SET_STANDARD_DEVICE && bRequest == REQUEST_SET_ADDRESS) {
        BYTE addr = wValue & 0x7F;
        USBPhy::SetAddress(addr);
        USBPhy::TransmitData(m_epnum, 0, 0);
    } else if(bmRequestType == SET_STANDARD_DEVICE &&
        bRequest == REQUEST_SET_CONFIG)
    {
        BYTE index = wValue;
        if(index == 0) {
            // XXX: just reset here, send ACK
            USBPhy::TransmitData(m_epnum, 0, 0);
        } else if(index == 1) {
            // There is only one configuration, just reset DATA0 and ACK
            EnableAppEndpoints();
            USBPhy::EndpointSetDATA0(0xFF);
            USBPhy::TransmitData(m_epnum, 0, 0);
        } else {
            // NAK all other configurations
            USBPhy::TransmitStall(m_epnum);
        }
    } else if((bmRequestType == GET_STANDARD_DEVICE ||
        bmRequestType == GET_STANDARD_INTERFACE) &&
        bRequest == REQUEST_GET_DESCRIPTOR)
    {
        // XXX: ignore set/clear feature
        BYTE type = wValue >> 8;
        BYTE index = wValue;
        const Descriptor* pDescriptor = get_usb_descriptor(type, index);
        if(pDescriptor) {
            USHORT length = pDescriptor->length;
            if(wLength < length)
                length = wLength;
            USBPhy::TransmitData(m_epnum, (WORD*)pDescriptor->data, length);
            //SimpleUart::Write('D');
        } else {
            //SimpleUart::Write('z');
            USBPhy::TransmitStall(m_epnum);
        }
    } else if(bmRequestType == GET_STANDARD_INTERFACE &&
        bRequest == REQUEST_GET_INTERFACE)
    {
        BYTE index = wValue;
        if(index == 0 && wLength == 1) {
            // There is only one interface, just send 0
            WORD buffer[1] = {0};
            USBPhy::TransmitData(m_epnum, buffer, 1);
        } else {
            // NAK all other interfaces
            //SimpleUart::Write('z');
            USBPhy::TransmitStall(m_epnum);
        }
    } else if(bmRequestType == SET_STANDARD_INTERFACE &&
        bRequest == REQUEST_SET_INTERFACE)
    {
        BYTE index = wValue;
        if(index == 0 && wLength == 0) {
            // There is only one interface, just send ACK
            EnableAppEndpoints();
            USBPhy::TransmitData(m_epnum, 0, 0);
        } else {
            // NAK all other interfaces
            //SimpleUart::Write('z');
            USBPhy::TransmitStall(m_epnum);
        }
    } else if(bmRequestType == GET_STANDARD_DEVICE &&
        bRequest == REQUEST_GET_STATUS)
    {
        WORD status = APP_STATUS;
        USBPhy::TransmitData(m_epnum, &status, 2);
        //SimpleUart::Write('S');
    }
    // Application specific requests below
    else if(bmRequestType == SET_CLASS_INTERFACE &&
        bRequest == REQUEST_SET_LINE_CODING)
    {
        USBPhy::TransmitData(0, 0, 0);
    // TODO: move hid stuff to HidKeyboard class
    } else if(bmRequestType == GET_CLASS_INTERFACE && bRequest == REQUEST_HID_GET_REPORT) {
        WORD buffer[2];
        BYTE keys[KeyMatrix::MAX_KEYS];
        BYTE length = 8;
        KeyMatrix::get_keys(keys);
        HidKeyboardEndpoint::make_report((BYTE*)buffer, keys);
        if(wLength < length)
            length = wLength;
        USBPhy::TransmitData(m_epnum, buffer, length);
    } else if(bmRequestType == GET_CLASS_INTERFACE && bRequest == REQUEST_HID_GET_IDLE) {
        WORD buffer[1] = { 0 };
        buffer[0] = ep1.get_idle();
        USBPhy::TransmitData(m_epnum, buffer, 1);
    } else if(bmRequestType == SET_CLASS_INTERFACE && bRequest == REQUEST_HID_SET_IDLE) {
        BYTE duration = wValue >> 8;
        if(wIndex == 0) {
            ep1.set_idle(duration);
            USBPhy::TransmitData(m_epnum, 0, 0);
        } else {
            USBPhy::TransmitStall(m_epnum);
        }
    } else if(bmRequestType == GET_CLASS_INTERFACE && bRequest == REQUEST_HID_GET_PROTOCOL) {
        WORD buffer[1] = { 0 };
        // TODO: implement action
        USBPhy::TransmitData(m_epnum, buffer, 1);
    } else if(bmRequestType == SET_CLASS_INTERFACE && bRequest == REQUEST_HID_SET_PROTOCOL) {
        // TODO: implement action
        USBPhy::TransmitData(m_epnum, 0, 0);
    } else {
        // Unknown request
        //SimpleUart::Write('x');
        USBPhy::TransmitStall(m_epnum);
    }

    m_bufferPos = 0;
    // XXX: just in case
    m_buffer.w[0] = 0;
    m_buffer.w[1] = 0;
    m_buffer.w[2] = 0;
    m_buffer.w[3] = 0;

    USBPhy::PrepareRX(m_epnum);
    //SimpleUart::Write('0');
}

void ControlEndpoint::OnTransmit() {
    //SimpleUart::Write('T');
}

void ControlEndpoint::EnableAppEndpoints() {
    using namespace dev::usb;
    eps[1]->Enable(USBEndpoint::DIR_IN, EPTYP_INT);
}
