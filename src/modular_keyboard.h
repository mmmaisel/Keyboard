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
#pragma once

#include "types.h"
#include "key_matrix.h"
#include "led_matrix.h"

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/queue.h"

class ModularKeyboard {
    // Static class
    ModularKeyboard() = delete;
    ModularKeyboard(const ModularKeyboard&) = delete;
    ModularKeyboard(ModularKeyboard&&) = delete;
    ~ModularKeyboard() = delete;

    public:
        static const BYTE BUFFER_SIZE = 16;
        static const BYTE PAGE_COUNT = 4;

        static void initialize();
        [[noreturn]] static void task(void* pContext);

        static void send_page(KeyMatrix::Page* page);
        static void send_page_from_isr(KeyMatrix::Page* page);

        static void get_keys(BYTE* buffer);
        static void set_led(const LedMatrix::Led& led);

    private:
        static QueueHandle_t m_queue;
        static KeyMatrix::Page m_queue_item;
        static StaticQueue_t m_queue_mem;

        static KeyMatrix::Page m_buffer;
        static KeyMatrix::Page m_pages[PAGE_COUNT];

        static void update_keys();
        static void process_keys(BYTE* buffer);
};
