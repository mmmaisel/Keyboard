/******************************************************************************\
    Split Keyboard
    Copyright (C) 2019-2025 - Max Maisel

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
#include "effect.h"

#include "event.h"
#include "key_matrix.h"

EffectNone effect_none;
EffectFlash effect_flash;
EffectRunning effect_running;

Effect* EffectController::_effect = &effect_none;
DWORD EffectController::_keys = 0;

void EffectNone::run(DWORD new_keys, DWORD old_keys) {}

void EffectFlash::run(DWORD new_keys, DWORD old_keys) {
    for(BYTE i = 0; i < KeyMatrixConfig::MAX_KEYS; ++i) {
        BYTE changed = !!((new_keys & (1ull << i)) ^ (old_keys & (1ull << i)));
        if(changed) {
            if(new_keys & (1ull << i)) {
                EventDispatcher::set_led(
                    i, Color { .red = 16, .green = 16, .blue = 16 }
                );
            } else {
                EventDispatcher::set_led(
                    i, Color { .red = 0, .green = 0, .blue = 0 }
                );
            }
        }
    }
}

void EffectRunning::run(DWORD new_keys, DWORD old_keys) {
    if(++_cnt >= 100) {
        _cnt = 0;

        Color color = {.red = 0, .green = 0, .blue = 0};
        EventDispatcher::set_led(_idx / 3, color);
        if(++_idx >= 120) {
            _idx = 0;
        }
        switch(_idx % 3) {
            case 0: color.red = 16; break;
            case 1: color.green = 16; break;
            case 2: color.blue = 16; break;
        }
        EventDispatcher::set_led(_idx / 3, color);
    }
}

void EffectController::on_keys(DWORD keys) {
    _effect->run(keys, _keys);
    _keys = keys;
}
