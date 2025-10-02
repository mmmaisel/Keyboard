/******************************************************************************\
    Split Keyboard
    Copyright (C) 2019-2025 - Max Maisel

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

#include "FreeRTOS/FreeRTOS.h"

class USBPhy;

class USBEndpoint {
    friend class USBPhy;
    public:
        enum { DIR_IN = 1, DIR_OUT = 2};

        explicit USBEndpoint(BYTE epnum);
        USBEndpoint(const USBEndpoint&) = delete;
        USBEndpoint(USBEndpoint&&) = delete;
        virtual ~USBEndpoint();

        void Enable(BYTE dir, WORD type);

        void Transmit(const WORD* pData, USHORT len);
        void Receive(WORD* pData, USHORT len);

        BYTE TXFIFOEmpty();

    protected:
        /// Called by ISR on data receive
        virtual void OnReceive(BaseType_t* task_woken);
        /// Called by ISR on setup receive
        virtual void OnSetup(BaseType_t* task_woken);
        /// Called by ISR on transmit complete
        virtual void OnTransmit(BaseType_t* task_woken);
        void OnRxData(const volatile WORD* data, USHORT len);

        static void operator delete(void* __attribute__((unused)));

        BYTE m_epnum;
        BYTE m_enabled;

        static const int BUFFER_SIZE = 32;
        template<USHORT S> union Buffer {
            BYTE  b[S];
            HWORD h[S/2];
            WORD  w[S/4];
        };
        Buffer<BUFFER_SIZE> m_buffer;
        BYTE m_bufferPos;
        USHORT m_length;
};

const BYTE USB_ENDPOINT_COUNT = 2;
extern USBEndpoint* eps[USB_ENDPOINT_COUNT];
