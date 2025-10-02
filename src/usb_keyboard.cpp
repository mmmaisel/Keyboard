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
#include "key_matrix.h"
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
        _pages[event->keys.page-1] = event->keys.state;
    }

    HidKeyboardReport report;
    fill_report(&report);

    xQueueOverwrite(_queue, &report);
    ep1.send_report(&report);
}

BYTE UsbKeyboard::get_report_from_isr(
    HidKeyboardReport* report, BaseType_t* task_woken
) {
    return xQueueReceiveFromISR(_queue, report, task_woken);
}

void UsbKeyboard::fill_report(HidKeyboardReport* report) {
    using namespace keycodes;

    BYTE pos = 0;
    report->buffer[0] = 0;
    report->buffer[1] = 0;

    // TODO: handle fn key combos

    for(BYTE i = 0; i < PAGE_COUNT; ++i) {
        for(BYTE j = 0; j < KeyMatrixConfig::MAX_KEYS; ++j) {
            if(_pages[i] & (1ull << j)) {
                BYTE keycode = KEY_LAYOUT[i][j];
                switch(keycode) {
                    case KEY_LEFTCTRL:
                        report->mods[0] |= MOD_LCTRL;
                        break;
                    case KEY_LEFTSHIFT:
                        report->mods[0] |= MOD_LSHIFT;
                        break;
                    case KEY_LEFTALT:
                        report->mods[0] |= MOD_LALT;
                        break;
                    case KEY_LEFTSUPER:
                        report->mods[0] |= MOD_LSUPER;
                        break;
                    case KEY_RIGHTCTRL:
                        report->mods[0] |= MOD_RCTRL;
                        break;
                    case KEY_RIGHTSHIFT:
                        report->mods[0] |= MOD_RSHIFT;
                        break;
                    case KEY_RIGHTALT:
                        report->mods[0] |= MOD_RALT;
                        break;
                    case KEY_RIGHTSUPER:
                        report->mods[0] |= MOD_RSUPER;
                        break;
                    default:
                        if(pos < 6)
                            report->keys[pos++] = keycode;
                        break;
                }
            }
        }
    }
}
