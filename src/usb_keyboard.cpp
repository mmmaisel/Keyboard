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
#include "usb_keyboard.h"

#include "event.h"
#include "hid_keyboard_endpoint.h"
#include "key_layout.h"

#include <cstring>

UsbKeyboard usb_keyboard;

UsbKeyboard::UsbKeyboard() {
    _queue = xQueueCreateStatic(1, sizeof(HidKeyboardReport),
        reinterpret_cast<BYTE*>(&_queue_items), &_queue_mem);
#ifdef DEBUG
    vQueueAddToRegistry(_queue, "HidKbd");
#endif
}

// TODO: USB needs mutex with any interrupt or xQueueOverwrite

void UsbKeyboard::on_event(Event* event) {
    if(event->type == EVENT_KEYS) {
        _pages[event->keys.page] = event->keys.state;
    }

    HidKeyboardReport report;
    // TODO: populate report

    xQueueOverwrite(_queue, &report);
    //ep1.send_report(&report);
}

BYTE UsbKeyboard::get_report_from_isr(
    HidKeyboardReport* report, BaseType_t* task_woken
) {
    return xQueueReceiveFromISR(_queue, report, task_woken);
}

//void ModularKeyboard::get_keys(BYTE* buffer) {
//    BYTE pos = 0;
    //BYTE is_fn = 0;
    //BYTE fn_code = 0;

    /*for(BYTE i = 0; i < PAGE_COUNT; ++i) {
        for(BYTE j = 0; j < KeyMatrix::MAX_KEYS; ++j) {
            BYTE keycode = m_pages[i].keys[j];

            if(keycode == keycodes::KEY_FN)
    is_fn = 1;
            if(fn_code == 0)
                fn_code = keycode;
        }
    }

    // TODO: process fn_code to keycode here
    if(is_fn) {
        buffer[0] = keycodes::KEY_NONE;
        return;
    }

    for(BYTE i = 0; i < PAGE_COUNT; ++i) {
        for(BYTE j = 0; j < KeyMatrix::MAX_KEYS; ++j) {
            BYTE keycode = m_pages[i].keys[j];
            if(keycode == keycodes::KEY_NONE)
                continue;

            buffer[pos] = keycode;
            if(++pos == BUFFER_SIZE-1) {
                buffer[pos] = keycodes::KEY_NONE;
                return;
            }
        }
    }*/
//    buffer[pos] = keycodes::KEY_NONE;
//}
