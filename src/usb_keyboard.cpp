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

#include "effect.h"
#include "event.h"
#include "hid_keyboard_endpoint.h"
#include "key_matrix.h"
#include "key_layout.h"
#include "uart.h"
#include "uart_serde.h"

#include <cstring>

UsbKeyboard usb_keyboard;

UsbKeyboard::UsbKeyboard() :
    _pages{},
    _last_cmd(EFFECT_NONE),
    _cmd_ctr(0),
    _idle_ctr(0)
{
    _queue = xQueueCreateStatic(1, sizeof(HidKeyboardReport),
        reinterpret_cast<BYTE*>(&_queue_items), &_queue_mem);
#ifdef DEBUG
    vQueueAddToRegistry(_queue, "HidKbd");
#endif
}

void UsbKeyboard::on_event(Event* event) {
    if(event->type != EVENT_KEYS)
        return;

    _pages[event->keys.page-1] = event->keys.state;

    BYTE send_report = 1;
    HidKeyboardReport report;
    if(fill_report(&report)) {
        send_report = handle_fn(&report);
    }

    if(send_report) {
        xQueueOverwrite(_queue, &report);
        ep1.send_report(&report);
    }
}

void UsbKeyboard::set_led(BYTE keycode, Color color) {
    BYTE num = 0;
    // TODO: support other pages
    BYTE page = 0;
    for(BYTE i = 0; i < KeyMatrixConfig::MAX_KEYS; ++i) {
        if(keycode == KEY_LAYOUT[page][i]) {
            num = i+1;
            break;
        }
    }

    LedMatrix::set_led(num, color);
}

BYTE UsbKeyboard::get_report_from_isr(
    HidKeyboardReport* report, BaseType_t* task_woken
) {
    return xQueueReceiveFromISR(_queue, report, task_woken);
}

BYTE UsbKeyboard::fill_report(HidKeyboardReport* report) {
    using namespace keycodes;

    BYTE pos = 0;
    BYTE found_fn = 0;
    report->buffer[0] = 0;
    report->buffer[1] = 0;

    for(BYTE i = 0; i < PAGE_COUNT; ++i) {
        for(BYTE j = 0; j < KeyMatrixConfig::MAX_KEYS; ++j) {
            if(_pages[i] & (1ull << j)) {
                BYTE keycode = KEY_LAYOUT[i][j];
                switch(keycode) {
                    case KEY_FN:
                        found_fn = 1;
                        break;
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

    return found_fn;
}

BYTE UsbKeyboard::handle_fn(HidKeyboardReport* report) {
    using namespace keycodes;

    switch(report->keys[0]) {
        case KEY_F12:
            replace_keys(report, KEY_MUTE);
            return 1;
        case KEY_DELETE:
            switch_effect(EFFECT_BACKLIGHT);
            return 0;
        case KEY_END:
            switch_effect(EFFECT_FLASH);
            return 0;
        case KEY_PAGEDOWN:
            switch_effect(EFFECT_RAINBOW);
            return 0;
        default:
            if(_last_cmd != EFFECT_NONE) {
                if(++_idle_ctr > IDLE_DURATION) {
                    _idle_ctr = 0;
                    _last_cmd = EFFECT_NONE;
                }
            }
    }

    return 1;
}

void UsbKeyboard::replace_keys(HidKeyboardReport* report, BYTE new_key) {
    report->keys[0] = new_key;
    report->keys[1] = 0;
    report->keys[2] = 0;
    report->keys[3] = 0;
    report->keys[4] = 0;
    report->keys[5] = 0;
}

void UsbKeyboard::switch_effect(EffectId id) {
    if(_last_cmd != id) {
        if(++_cmd_ctr > UartMessage::CTR_MAX)
            _cmd_ctr = 0;
        _last_cmd = id;
        EffectController::set_effect(EffectController::effect_by_id(id));
    }

    UartMessage msg = UartMessage::serialize_effect(_cmd_ctr, id);
    uart1.write(reinterpret_cast<BYTE*>(&msg), UartMessage::EFF_MSG_LEN);
    uart2.write(reinterpret_cast<BYTE*>(&msg), UartMessage::EFF_MSG_LEN);
    uart6.write(reinterpret_cast<BYTE*>(&msg), UartMessage::EFF_MSG_LEN);
}
