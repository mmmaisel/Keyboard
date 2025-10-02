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
#include "usb_endpoint.h"

struct HidKeyboardReport {
    static const BYTE LEN = 8;
    union {
        BYTE report[LEN];
        WORD buffer[LEN/4];
    };
};

class HidKeyboardEndpoint : public USBEndpoint {
    public:
        HidKeyboardEndpoint();
        HidKeyboardEndpoint(const HidKeyboardEndpoint&) = delete;
        HidKeyboardEndpoint(HidKeyboardEndpoint&&) = delete;
        virtual ~HidKeyboardEndpoint();

        inline void set_idle(BYTE idle) { m_idle = idle; }
        inline BYTE get_idle() { return m_idle; }

        void send_report(const HidKeyboardReport* report);

    protected:
        virtual void OnTransmit(BaseType_t* task_woken) override;

    private:
        BYTE m_idle;
        BYTE m_transmitting;
};

extern HidKeyboardEndpoint ep1;
