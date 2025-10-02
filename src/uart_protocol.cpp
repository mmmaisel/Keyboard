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
#include "uart_protocol.h"

#include <cstring>

BYTE UartProtocol::m_key_was_pressed = 0;
QueueHandle_t UartProtocol::m_queue = 0;
UartMessage UartProtocol::m_message;
UartMessage UartProtocol::m_queue_items[QUEUE_LENGTH] = {0};
StaticQueue_t UartProtocol::m_queue_mem = {0};
UartProtocolHandler UartProtocol::m_handlers[Uart::UART_COUNT];
TickType_t UartProtocol::m_ticks[Uart::UART_COUNT] = {0};

UartProtocolHandler::UartProtocolHandler() :
    m_state(STATE_IDLE)
{
}

void UartProtocolHandler::OnReceive(UartMessage& msg) {
    /*switch(m_state) {
        case STATE_IDLE:
            if(msg.is_dma()) {
                return;
            }
            switch(msg.data() & UartProtocol::MSG_TYPE_MASK) {
                case UartProtocol::MSG_KEYS:
                    m_page.id = msg.data() & UartProtocol::MSG_PAGE_MASK;
                    Uart::BY_ID[msg.id()]->start_read(
                        m_page.keys, KeyMatrix::MAX_KEYS);
                    m_state = STATE_KEY_PAYLOAD;
                    break;
                case UartProtocol::MSG_LEDS:
                    m_led_count = msg.data() & UartProtocol::MSG_PAGE_MASK;
                    Uart::BY_ID[msg.id()]->start_read(
                        reinterpret_cast<BYTE*>(m_leds),
                        m_led_count*sizeof(LedMatrix::Led)
                    );
                    m_state = STATE_LED_PAYLOAD;
                    break;
            }
            break;
        case STATE_KEY_PAYLOAD:
            if(!msg.is_dma()) {
                return;
            }
            ModularKeyboard::send_page(&m_page);
            Uart::BY_ID[msg.id()]->start_read_byte();

            m_state = STATE_IDLE;
            break;

        case STATE_LED_PAYLOAD:
            if(!msg.is_dma()) {
                return;
            }

            for(BYTE i = 0; i < m_led_count; ++i)
                LedMatrix::set_led(m_leds[i]);

            m_state = STATE_IDLE;
            break;
    }*/
}

void UartProtocolHandler::reset(Uart* uart) {
    m_state = STATE_IDLE;
    uart->abort_read();
    uart->start_read_byte();
}

void UartProtocol::initialize() {
    m_queue = xQueueCreateStatic(QUEUE_LENGTH, sizeof(UartMessage),
        reinterpret_cast<BYTE*>(&m_queue_items), &m_queue_mem);
    Uart1.set_queue(m_queue);
    Uart2.set_queue(m_queue);
    Uart6.set_queue(m_queue);
#ifdef DEBUG
    vQueueAddToRegistry(m_queue, "UartProto");
#endif
}

[[noreturn]] void UartProtocol::task(void* pContext __attribute((unused))) {
    m_handlers[0].reset(Uart::BY_ID[0]);
    m_handlers[1].reset(Uart::BY_ID[1]);
    m_handlers[2].reset(Uart::BY_ID[2]);

    m_ticks[0] = xTaskGetTickCount();
    m_ticks[1] = m_ticks[0];
    m_ticks[2] = m_ticks[0];

    for(;;) {
        if(xQueueReceive(m_queue, &m_message, RX_TIMEOUT) != pdTRUE) {
            TickType_t ticks = xTaskGetTickCount();
            for(BYTE i = 0; i < Uart::UART_COUNT; ++i) {
                if(m_ticks[i] + RX_TIMEOUT < ticks) {
                    m_ticks[i] = ticks;
                    m_handlers[i].reset(Uart::BY_ID[i]);
                }
            }
        } else {
            m_ticks[m_message.id()] = xTaskGetTickCount();
            m_handlers[m_message.id()].OnReceive(m_message);
        }
    }
}

/*void UartProtocol::send_key_page(KeyMatrix::Page& page) {
    page.id |= MSG_KEYS;
    Uart1.write(reinterpret_cast<BYTE*>(&page), sizeof(KeyMatrix::Page));
    page.id &= ~MSG_KEYS;
}

void UartProtocol::send_led(const LedMatrix::Led& led) {
    // Broadcast new LED state to all secondary modules since they can
    // be connected in arbitrary order.
    BYTE buffer[sizeof(LedMatrix::Led)+1];
    buffer[0] = MSG_LEDS | 0x01;
    memcpy(buffer+1, reinterpret_cast<const BYTE*>(&led), sizeof(LedMatrix::Led));
    // XXX: group transactions
    Uart1.write(buffer, sizeof(LedMatrix::Led)+1);
    Uart2.write(buffer, sizeof(LedMatrix::Led)+1);
    Uart6.write(buffer, sizeof(LedMatrix::Led)+1);
}*/
