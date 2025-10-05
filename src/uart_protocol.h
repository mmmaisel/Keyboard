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

struct __attribute__((packed)) UartMessage {
    static const BYTE STATE_LEN = 5;

    BYTE hdr; // counter:4 type:2 page:2
    union {
        struct __attribute__((packed)) {
            BYTE state[STATE_LEN];
        } keys;
        struct __attribute__((packed)) {
            BYTE red_green;
            BYTE blue_x;
            BYTE state[STATE_LEN];
        } leds;
    };
    BYTE crc;

    static UartMessage serialize_keys(BYTE ctr, BYTE page, DWORD state);
};

// TODO: serde
// TODO: 1 receiver task per UART

#if 0
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
        /*union {
            KeyMatrix::Page m_page;
            struct {
                BYTE m_led_count;
                LedMatrix::Led m_leds[8];
            };
        };*/
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

        //static void send_key_page(KeyMatrix::Page& page);
        //static void send_led(const LedMatrix::Led& led);

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
#endif
