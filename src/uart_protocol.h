/**********************************************************************\
 * Keyboard
 *
 * UART protocol class
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
#include "uart.h"
#include "key_matrix.h"
#include "led_matrix.h"

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/queue.h"

class UartProtocolHandler {
    public:
        UartProtocolHandler();
        void OnReceive(UartMessage& message);
        void reset(Uart* uart);

    private:
        enum : BYTE {
            STATE_IDLE,
            STATE_KEY_PAYLOAD,
            STATE_LED_PAYLOAD,
        };

        BYTE m_state;
        KeyMatrix::Page m_page;
};

class UartProtocol {
    // Static class
    UartProtocol() = delete;
    UartProtocol(const UartProtocol&) = delete;
    UartProtocol(UartProtocol&&) = delete;
    ~UartProtocol() = delete;

    public:
        enum : BYTE {
            MSG_KEYS = 0x10,
            MSG_LEDS = 0x20,
            MSG_TYPE_MASK = 0xF0,
            MSG_PAGE_MASK = 0x0F
        };

        static void initialize();
        [[noreturn]] static void task(void* pContext);

        static void send_key_page(KeyMatrix::Page& page);
        static void send_led(const LedMatrix::Led& led);

    private:
        static BYTE m_key_was_pressed;
        static const BYTE QUEUE_LENGTH = 3;
        static const BYTE RX_TIMEOUT = 20;
        static QueueHandle_t m_queue;
        static UartMessage m_message;
        static UartMessage m_queue_items[QUEUE_LENGTH];
        static StaticQueue_t m_queue_mem;
        static UartProtocolHandler m_handlers[Uart::UART_COUNT];
        static TickType_t m_ticks[Uart::UART_COUNT];
};
