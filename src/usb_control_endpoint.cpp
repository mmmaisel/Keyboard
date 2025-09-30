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
#include "modular_keyboard.h"

#include "key_layout.h"
#include "key_matrix.h"
#include "led_matrix.h"
#include "uart.h"
#include <string.h>

ControlEndpoint ep0;

ControlEndpoint::ControlEndpoint() :
    USBEndpoint(0),
    m_last_command(0)
{
}

ControlEndpoint::~ControlEndpoint() {
}

void ControlEndpoint::OnReceive() {
    //Uart6.write('R');
    //Uart6.write(m_bufferPos);

    // Set Num / Caps / Scroll LEDs
    if(m_last_command == REQUEST_HID_SET_REPORT) {
        BYTE leds = m_buffer.b[8];
        LedMatrix::Led led_on {
            .red = 16,
            .green = 0,
            .blue = 0,
        };
        LedMatrix::Led led_off {
            .red = 0,
            .green = 0,
            .blue = 0,
        };

        if(leds & NUMLOCK_MASK) {
            led_on.keycode = keycodes::LED_NUMLOCK;
            LedMatrix::set_led(led_on);
        } else {
            led_off.keycode = keycodes::LED_NUMLOCK;
            LedMatrix::set_led(led_off);
        }
        if(leds & CAPSLOCK_MASK) {
            led_on.keycode = keycodes::LED_CAPSLOCK;
            LedMatrix::set_led(led_on);
        } else {
            led_off.keycode = keycodes::LED_CAPSLOCK;
            LedMatrix::set_led(led_off);
        }
        if(leds & SCROLL_MASK) {
            led_on.keycode = keycodes::LED_SCROLL;
            LedMatrix::set_led(led_on);
        } else {
            led_off.keycode = keycodes::LED_SCROLL;
            LedMatrix::set_led(led_off);
        }
        m_last_command = 0;
    }

    m_length = 0;
    m_bufferPos = 0;
    // XXX: just in case
    m_buffer.w[0] = 0;
    m_buffer.w[1] = 0;
    m_buffer.w[2] = 0;
    m_buffer.w[3] = 0;
    USBPhy::PrepareRX(m_epnum);
}

void ControlEndpoint::OnSetup() {
    //Uart6.write('S');
    //Uart6.write(m_bufferPos);
    HandleSetup(*reinterpret_cast<Buffer<BUFFER_SIZE>*>(
        m_buffer.w + m_bufferPos - SETUP_PKT_WSIZE));
    USBPhy::PrepareRX(m_epnum);
}

void ControlEndpoint::OnTransmit() {
    //SimpleUart::Write('T');
}

void ControlEndpoint::HandleSetup(const Buffer<BUFFER_SIZE>& buffer) {
    BYTE bmRequestType = buffer.b[0];
    BYTE bRequest = buffer.b[1];
    SHORT wValue = buffer.h[1];
    SHORT wIndex = buffer.h[2];
    SHORT wLength = buffer.h[3];

    // All setup commands land on EP0 (HID, CDC set line state, ...)
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
            USBPhy::TransmitData(
                m_epnum, reinterpret_cast<WORD*>(pDescriptor->data), length);
            //Uart6.write('D');
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
            WORD txbuf[1] = {0};
            USBPhy::TransmitData(m_epnum, txbuf, 1);
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
            // XXX: use this to switch between bootp and report descriptor
            // XXX: needs two interface descriptors
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
        USBPhy::TransmitData(m_epnum, 0, 0);
    // TODO: move hid stuff to HidKeyboard class
    } else if(bmRequestType == GET_CLASS_INTERFACE && bRequest == REQUEST_HID_GET_REPORT) {
        WORD txbuf[2];
        BYTE keys[16];
        BYTE length = 8;
        //ModularKeyboard::get_keys(keys);
        HidKeyboardEndpoint::make_report(reinterpret_cast<BYTE*>(txbuf), keys);
        if(wLength < length)
            length = wLength;
        USBPhy::TransmitData(m_epnum, txbuf, length);
    } else if(bmRequestType == SET_CLASS_INTERFACE && bRequest == REQUEST_HID_SET_REPORT) {
        // Postpone command processing until the following data fragment
        // was received.
        m_last_command = REQUEST_HID_SET_REPORT;
        USBPhy::TransmitData(m_epnum, 0, 0);
    } else if(bmRequestType == GET_CLASS_INTERFACE && bRequest == REQUEST_HID_GET_IDLE) {
        WORD txbuf[1] = { 0 };
        txbuf[0] = ep1.get_idle();
        USBPhy::TransmitData(m_epnum, txbuf, 1);
    } else if(bmRequestType == SET_CLASS_INTERFACE && bRequest == REQUEST_HID_SET_IDLE) {
        BYTE duration = wValue >> 8;
        if(wIndex == 0) {
            ep1.set_idle(duration);
            USBPhy::TransmitData(m_epnum, 0, 0);
        } else {
            USBPhy::TransmitStall(m_epnum);
        }
    } else if(bmRequestType == GET_CLASS_INTERFACE && bRequest == REQUEST_HID_GET_PROTOCOL) {
        WORD txbuf[1] = { 0 };
        // XXX: 0 == boot protocol, 1 == report protocol
        // TODO: implement action
        USBPhy::TransmitData(m_epnum, txbuf, 1);
    } else if(bmRequestType == SET_CLASS_INTERFACE && bRequest == REQUEST_HID_SET_PROTOCOL) {
        // TODO: implement action
        // XXX: switch between boot and report protocol
        USBPhy::TransmitData(m_epnum, 0, 0);
    } else {
        // Unknown request
        //Uart6.write('x');
        USBPhy::TransmitStall(m_epnum);
    }
}

void ControlEndpoint::EnableAppEndpoints() {
    using namespace dev::usb;
    eps[1]->Enable(USBEndpoint::DIR_IN, EPTYP_INT);
}
