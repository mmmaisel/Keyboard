/**********************************************************************\
 * Keyboard
 *
 * USB driver class
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
#pragma once

#include "types.h"

extern "C" void usb_vector() __attribute__((error("calling ISR")));
extern "C" void usb_wakeup_vector() __attribute__((error("calling ISR")));

class USBPhy
{
    // Static class
    USBPhy() = delete;
    USBPhy(const USBPhy&) = delete;
    USBPhy(USBPhy&&) = delete;
    ~USBPhy() = delete;

    friend void usb_vector();
    friend void usb_wakeup_vector();

    public:
        static void Initialize();

        static void PrepareRX(BYTE epnum);
        static void TransmitData(BYTE epnum, const WORD* pBuffer, USHORT len);
        static BYTE TXFIFOEmpty(BYTE epnum);
        static void TransmitStall(BYTE epnum);
        static void SetAddress(BYTE adr);
        static void ResetAllEndpoints();
        static void EnableInEndpoint(BYTE num, WORD type);
        static void EnableOutEndpoint(BYTE num, WORD type);
        static void EndpointSetDATA0(BYTE num);

    private:
        static void ISR();
        static void WakeupISR();
};

