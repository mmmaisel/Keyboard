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
#include "dev/uart.h"
#include "dev/dma.h"

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/queue.h"
#include "FreeRTOS/semphr.h"

extern "C" void uart1_vector() __attribute__((error("calling ISR")));
extern "C" void uart2_vector() __attribute__((error("calling ISR")));
extern "C" void uart6_vector() __attribute__((error("calling ISR")));
extern "C" void dma1s5_vector() __attribute__((error("calling ISR")));
extern "C" void dma1s6_vector() __attribute__((error("calling ISR")));
extern "C" void dma2s1_vector() __attribute__((error("calling ISR")));
extern "C" void dma2s2_vector() __attribute__((error("calling ISR")));
extern "C" void dma2s6_vector() __attribute__((error("calling ISR")));
extern "C" void dma2s7_vector() __attribute__((error("calling ISR")));

struct UartConfig {
    volatile dev::UsartStruct* uart;
    volatile dev::DmaStruct* dma;
    BYTE rx_stream;
    BYTE tx_stream;
    BYTE(*hw_init_fn)();
#ifdef DEBUG
    const char* rx_queue_name;
    const char* tx_queue_name;
#endif
};

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
        explicit Uart(UartConfig* config);

        /// Directly writes a single byte to FIFO.
        void debug_write(BYTE data);
        /// Writes buffer with DMA. Not reentrant.
        void write(const BYTE* buffer, BYTE length);
        /// Reads to buffer with DMA. Not reentrant.
        BYTE read(BYTE* buffer, BYTE length);

    private:
        UartConfig* _config;

        SemaphoreHandle_t _rx_semaphore;
        StaticSemaphore_t _rx_semaphore_mem;
        SemaphoreHandle_t _tx_semaphore;
        StaticSemaphore_t _tx_semaphore_mem;

        void ISR();
        void DMA_TX_ISR();
        void DMA_RX_ISR();
};

extern Uart uart1;
extern Uart uart2;
extern Uart uart6;
