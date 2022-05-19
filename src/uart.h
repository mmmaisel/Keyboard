/**********************************************************************\
 * Keyboard
 *
 * UART class
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
#pragma once

#include "types.h"
#include "dev/uart.h"
#include "dev/dma.h"

#include "ringbuffer.h"

extern "C" void uart1_vector() __attribute__((error("calling ISR")));
extern "C" void uart2_vector() __attribute__((error("calling ISR")));
extern "C" void uart6_vector() __attribute__((error("calling ISR")));
extern "C" void dma1s5_vector() __attribute__((error("calling ISR")));
extern "C" void dma1s6_vector() __attribute__((error("calling ISR")));
extern "C" void dma2s1_vector() __attribute__((error("calling ISR")));
extern "C" void dma2s2_vector() __attribute__((error("calling ISR")));
extern "C" void dma2s6_vector() __attribute__((error("calling ISR")));
extern "C" void dma2s7_vector() __attribute__((error("calling ISR")));

class Uart
{
    Uart(const Uart&) = delete;
    Uart(Uart&&) = delete;

    friend void uart1_vector();
    friend void uart2_vector();
    friend void uart6_vector();
    friend void dma1s5_vector();
    friend void dma1s6_vector();
    friend void dma2s1_vector();
    friend void dma2s2_vector();
    friend void dma2s6_vector();
    friend void dma2s7_vector();

    public:
        explicit Uart(volatile dev::UsartStruct* usart);
        ~Uart() {}

        void write(BYTE* buffer, BYTE length);
        BYTE read();
        void read(BYTE* buffer, BYTE length);

    private:
        volatile dev::UsartStruct* const m_uart;
        volatile BYTE m_tx;
        volatile BYTE m_rx;
        static const BYTE BUFFER_SIZE = 64;
        BYTE m_tx_buffer[BUFFER_SIZE];
        volatile Ringbuffer<BYTE, BYTE, BUFFER_SIZE> m_rx_buffer;

        void ISR();
        void DMA_TX_ISR();
        void DMA_RX_ISR();
};

extern Uart Uart6;
