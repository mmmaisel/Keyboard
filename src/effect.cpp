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

#include <math.h>

EffectNone effect_none;
EffectBacklight effect_backlight;
EffectFlash effect_flash;
EffectRunning effect_running;
EffectRainbow effect_rainbow;

BYTE EffectController::_page = 0;
BYTE EffectController::_led_count = 0;
Effect* EffectController::_effect = &effect_none;
DWORD EffectController::_keys = 0;

void Effect::activate() {}

void EffectNone::run(BYTE led_count, DWORD new_keys, DWORD old_keys) {}

void EffectBacklight::activate() {
    _changed = 1;
}

void EffectBacklight::run(BYTE led_count, DWORD new_keys, DWORD old_keys) {
    if(!_changed)
        return;

    _changed = 0;
    for(BYTE i = 0; i < led_count; ++i) {
        LedMatrix::set_led(
            i+1, Color { .red = 16, .green = 16, .blue = 16 }
        );
    }
}

void EffectFlash::run(BYTE led_count, DWORD new_keys, DWORD old_keys) {
    for(BYTE i = 0; i < led_count; ++i) {
        BYTE changed = !!((new_keys & (1ull << i)) ^ (old_keys & (1ull << i)));
        if(changed) {
            if(new_keys & (1ull << i)) {
                LedMatrix::set_led(
                    i+1, Color { .red = 16, .green = 16, .blue = 16 }
                );
            } else {
                LedMatrix::set_led(
                    i+1, Color { .red = 0, .green = 0, .blue = 0 }
                );
            }
        }
    }
}

void EffectRunning::run(BYTE led_count, DWORD new_keys, DWORD old_keys) {
    if(++_cnt >= 60) {
        _cnt = 0;

        Color color = {.red = 0, .green = 0, .blue = 0};
        LedMatrix::set_led(1 + _idx / 3, color);
        if(++_idx >= led_count*3) {
            _idx = 0;
        }
        switch(_idx % 3) {
            case 0: color.red = 16; break;
            case 1: color.green = 16; break;
            case 2: color.blue = 16; break;
        }
        LedMatrix::set_led(1 + _idx / 3, color);
    }
}

void EffectRainbow::run(BYTE led_count, DWORD new_keys, DWORD old_keys) {
    Color color;
    TickType_t current_ticks = xTaskGetTickCount();
    _phase = fmod((_phase + (current_ticks - _ticks) / 100.0), float(led_count));
    _ticks = current_ticks;

    for(BYTE i = 0; i < led_count; ++i) {
        float hi = fmod((float(i + led_count) + _phase), led_count)
            / float(led_count) * 6.0f;

        switch(BYTE(hi)) {
            case 0:
                color.red = 16;
                color.green = 16 * (1 - fabs(fmod(hi, 2.0f) - 1));
                color.blue = 0;
                break;
            case 1:
                color.red = 16 * (1 - fabs(fmod(hi, 2.0f) - 1));
                color.green = 16;
                color.blue = 0;
                break;
            case 2:
                color.red = 0;
                color.green = 16;
                color.blue = 16 * (1 - fabs(fmod(hi, 2.0f) - 1));
                break;
            case 3:
                color.red = 0;
                color.green = 16 * (1 - fabs(fmod(hi, 2.0f) - 1));
                color.blue = 16;
                break;
            case 4:
                color.red = 16 * (1 - fabs(fmod(hi, 2.0f) - 1));
                color.green = 0;
                color.blue = 16;
                break;
            case 5:
                color.red = 16;
                color.green = 0;
                color.blue = 16 * (1 - fabs(fmod(hi, 2.0f) - 1));
                break;
        }

        LedMatrix::set_led(i+1, color);
    }
}

void EffectController::initialize(BYTE page, const LedMatrixConfig* config) {
    _page = page;
    _led_count = config->led_count;
}

void EffectController::set_effect(Effect* effect) {
    if(_effect == effect)
        return;

    _effect = effect;
    _keys = 0;
    _effect->activate();
    LedMatrix::clear();
}

void EffectController::on_keys(BYTE page, DWORD keys) {
    if(page != _page)
        return;

    _effect->run(_led_count, keys, _keys);
    _keys = keys;
}

Effect* EffectController::effect_by_id(EffectId id) {
    switch(id) {
        case EFFECT_NONE:
            return &effect_none;
        case EFFECT_BACKLIGHT:
            return &effect_backlight;
        case EFFECT_FLASH:
            return &effect_flash;
        case EFFECT_RUNNING:
            return &effect_running;
        case EFFECT_RAINBOW:
            return &effect_rainbow;
        default:
            return &effect_none;
    }
}
