/**********************************************************************\
 * Keyboard
 *
 * Light effect controller
 **********************************************************************
 * Copyright (C) 2019-2022 - Max Maisel
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
#include "effect_controller.h"
#include "key_matrix.h"
#include "key_layout.h"
#include "led_matrix.h"
#include "modular_keyboard.h"

EffectController::Effect EffectController::m_active_effect = FLASH;

void EffectController::set_effect(Effect effect) {
    m_active_effect = effect;
}

void EffectController::on_update_page(const BYTE* old_page, const BYTE* new_page) {
    if(m_active_effect == FLASH) {
        for(BYTE i = 0; i < KeyMatrix::MAX_KEYS; ++i) {
            BYTE keycode = old_page[i];
            if(keycode == 0)
                break;

            LedMatrix::Led led;
            led.keycode = keycode;
            led.green = 0;
            led.red = 0;
            led.blue = 0;
            // XXX: combine LED messages
            ModularKeyboard::set_led(led);
        }

        for(BYTE i = 0; i < KeyMatrix::MAX_KEYS; ++i) {
            BYTE keycode = new_page[i];
            if(keycode == 0)
                break;

            LedMatrix::Led led;
            led.keycode = keycode;
            led.green = keycode != keycodes::KEY_NONE ? 16 : 0;
            led.red = keycode != keycodes::KEY_NONE ? 16 : 0;
            led.blue = keycode != keycodes::KEY_NONE ? 16 : 0;
            // XXX: combine LED messages
            ModularKeyboard::set_led(led);
        }
    }
}
