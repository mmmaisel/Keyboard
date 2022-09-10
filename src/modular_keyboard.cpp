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
#include "uart_protocol.h"
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
#ifdef DEBUG
    vQueueAddToRegistry(m_queue, "ModKbd");
#endif
}

[[noreturn]] void ModularKeyboard::task(void* pContext __attribute((unused))) {
    // XXX: store led state in flash?
    for(;;) {
        if(xQueueReceive(m_queue, &m_buffer, portMAX_DELAY) != pdTRUE)
            continue;

        if(Module::get_id() == Module::RIGHT) {
            update_keys();
        } else {
            UartProtocol::send_key_page(m_buffer);
        }
    }
}

void ModularKeyboard::send_page(KeyMatrix::Page* page) {
    xQueueSend(m_queue, page, portMAX_DELAY);
}

void ModularKeyboard::send_page_from_isr(KeyMatrix::Page* page) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR(m_queue, page, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void ModularKeyboard::get_keys(BYTE* buffer) {
    //dev::set_basepri(priority::USB_KEY_REQUEST);
    //process_keys(buffer);
    //dev::set_basepri(priority::BASE);
}

void ModularKeyboard::set_led(const LedMatrix::Led& led) {
    if(!LedMatrix::set_led(led) && Module::get_id() == Module::RIGHT)
        UartProtocol::send_led(led);
}

void ModularKeyboard::update_keys() {
    BYTE id = m_buffer.id;
    BYTE buffer[BUFFER_SIZE];
    memcpy(&m_pages[id], &m_buffer, sizeof(KeyMatrix::Page));
    process_keys(buffer);
    // disabled for testing, no strange keystrokes
    // ep1.send_report(buffer);
}

void ModularKeyboard::process_keys(BYTE* buffer) {
    BYTE pos = 0;
    BYTE is_fn = 0;
    BYTE fn_code = 0;

//    LedMatrix::clear();

    for(BYTE i = 0; i < PAGE_COUNT; ++i) {
        for(BYTE j = 0; j < KeyMatrix::MAX_KEYS; ++j) {
            BYTE keycode = m_pages[i].keys[j];

            LedMatrix::Led led;
            led.keycode = keycode;
            led.green = keycode != keycodes::KEY_NONE ? 16 : 0;
            led.red = keycode != keycodes::KEY_NONE ? 16 : 0;
            led.blue = keycode != keycodes::KEY_NONE ? 16 : 0;
            LedMatrix::set_led(led);
            // XXX: send led

            if (keycode != keycodes::KEY_NONE)
                asm volatile("nop");

            if(keycode == keycodes::KEY_FN)
                is_fn = 1;
            if(fn_code == 0)
                fn_code = keycode;
        }
    }

    // XXX: check for FN first, then copy non-null keys to buffer

    if(is_fn) {
        // TODO: process fn_code to keycode here
        memset(m_pages[m_buffer.id].keys, 0, KeyMatrix::MAX_KEYS);
    } else {
        memcpy(&m_pages[m_buffer.id], &m_buffer, sizeof(KeyMatrix::Page));
    }
}
