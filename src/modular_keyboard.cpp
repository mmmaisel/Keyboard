/**********************************************************************\
 * Keyboard
 *
 * Modular keyboard class
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
#include "modular_keyboard.h"
#include "module.h"
#include "priority.h"
#include "hid_keyboard_endpoint.h"
#include "uart.h"
#include "key_layout.h"

#include "dev/core.h"

#include <cstring>

QueueHandle_t ModularKeyboard::m_queue = 0;
KeyMatrix::Page ModularKeyboard::m_queue_item = {0};
StaticQueue_t ModularKeyboard::m_queue_mem = {0};

KeyMatrix::Page ModularKeyboard::m_buffer = {0};
KeyMatrix::Page ModularKeyboard::m_pages[PAGE_COUNT] = {0};

void ModularKeyboard::initialize() {
    m_queue = xQueueCreateStatic(1, sizeof(KeyMatrix::Page),
        reinterpret_cast<BYTE*>(&m_queue_item), &m_queue_mem);
}

[[noreturn]] void ModularKeyboard::task(void* pContext __attribute((unused))) {
    // XXX: store led state in flash?
    for(;;) {
        if(xQueueReceive(m_queue, &m_buffer, portMAX_DELAY) == pdTRUE) {
            BYTE is_fn = 0;
            BYTE fn_code = 0;

            for(BYTE i = 0; i < KeyMatrix::Page::SIZE; ++i) {
                BYTE keycode = m_buffer.keys[i];

                if(keycode == keycodes::KEY_FN)
                    is_fn = 1;
                if(fn_code == 0)
                    fn_code = keycode;
            }

            if(is_fn) {
                // TODO: process fn_code to keycode here
                memset(m_pages[m_buffer.id].keys, 0, KeyMatrix::Page::SIZE);
            } else {
                memcpy(&m_pages[m_buffer.id], &m_buffer, sizeof(KeyMatrix::Page));
            }
        }
    }
}

void ModularKeyboard::send_page(KeyMatrix::Page* page) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR(m_queue, page, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/*void ModularKeyboard::OnReceive(Uart* uart, BYTE data) {
    switch(data & MSG_TYPE_MASK) {
        case MSG_KEYS: {
            BYTE page = data & MSG_PAGE_MASK;
            BYTE buffer[KeyMatrix::MAX_KEYS];
            // XXX: read ISR never gets called because of priorities (reentrant ISR)
            uart->read(buffer, KeyMatrix::MAX_KEYS);
            update_keys(page, buffer);
            break;
        }
        case MSG_LEDS: {
            BYTE length = data & MSG_PAGE_MASK;
            LedMatrix::Led leds[16];
            uart->read(
                reinterpret_cast<BYTE*>(leds), length*sizeof(LedMatrix::Led));
            for(BYTE i = 0; i < length; ++i)
                LedMatrix::set_led(leds[i]);
            break;
        }
    }
}*/

void ModularKeyboard::update_keys(BYTE page, const BYTE* keycodes) {
/*    if(Module::get_id() == Module::RIGHT) {
        memcpy(m_keys[page], keycodes, KeyMatrix::MAX_KEYS);
        if(page == 0) {
            BYTE buffer[BUFFER_SIZE];
            process_keys(buffer);
            ep1.send_report(&m_keys[0][0]);
        }
    } else if(*keycodes) {
        BYTE buffer[KeyMatrix::MAX_KEYS+1];
        buffer[0] = MSG_KEYS | page;
        memcpy(buffer+1, keycodes, KeyMatrix::MAX_KEYS);

        // XXX: adjust priorities, this causes freeze
        //Uart1.write(buffer, KeyMatrix::MAX_KEYS+1);
    }*/
}

void ModularKeyboard::get_keys(BYTE* buffer) {
    //dev::set_basepri(priority::USB_KEY_REQUEST);
    //process_keys(buffer);
    //dev::set_basepri(priority::BASE);
}

void ModularKeyboard::set_led(LedMatrix::Led led) {
    if(!LedMatrix::set_led(led) && Module::get_id() == Module::RIGHT) {
        // Broadcast new LED state to all secondary modules since they can
        // be connected in arbitrary order.
        BYTE buffer[sizeof(LedMatrix::Led)+1];
        buffer[0] = MSG_LEDS | 0x01;
        memcpy(buffer+1, reinterpret_cast<BYTE*>(&led), sizeof(LedMatrix::Led));
        // XXX: group transactions
        // XXX: adjust priorities, this causes freeze
        //Uart1.write(buffer, sizeof(LedMatrix::Led)+1);
        //Uart2.write(buffer, sizeof(LedMatrix::Led)+1);
        //Uart6.write(buffer, sizeof(LedMatrix::Led)+1);
    }
}
