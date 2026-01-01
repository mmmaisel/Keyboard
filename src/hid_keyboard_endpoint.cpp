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
#include "hid_keyboard_endpoint.h"

#include "usb_endpoint.h"
#include "usb_phy.h"

HidKeyboardEndpoint ep1;

HidKeyboardEndpoint::HidKeyboardEndpoint() :
    USBEndpoint(1),
    m_idle(0),
    m_transmitting(0)
{
}

HidKeyboardEndpoint::~HidKeyboardEndpoint() {
}

void HidKeyboardEndpoint::send_report(const HidKeyboardReport* report) {
    if(m_transmitting || !m_enabled)
        return;

    m_transmitting = 1;

    USBPhy::TransmitData(m_epnum, report->buffer, 8);
}

void HidKeyboardEndpoint::OnTransmit(BaseType_t* task_woken) {
    m_transmitting = 0;
}
