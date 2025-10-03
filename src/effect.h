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
#pragma once

#include "types.h"

#include "led_matrix.h"

class Effect {
    public:
        virtual void run(BYTE led_count, DWORD new_keys, DWORD old_keys) = 0;
};

class EffectNone : public Effect {
    public:
        virtual void run(
            BYTE led_count, DWORD new_keys, DWORD old_keys) override;
};

class EffectFlash : public Effect {
    public:
        virtual void run(
            BYTE led_count, DWORD new_keys, DWORD old_keys) override;
};

class EffectRunning : public Effect {
    public:
        virtual void run(
            BYTE led_count, DWORD new_keys, DWORD old_keys) override;

    private:
        BYTE _idx;
        WORD _cnt;
};

class EffectRainbow : public Effect {
    public:
        virtual void run(
            BYTE led_count, DWORD new_keys, DWORD old_keys) override;

    private:
        BYTE _phase;
        BYTE _cnt;
};

class EffectController {
    // Static class
    EffectController() = delete;
    EffectController(const EffectController&) = delete;
    EffectController(EffectController&&) = delete;
    ~EffectController() = delete;

    public:
        static void initialize(const LedMatrixConfig* config);
        static inline void set_effect(Effect* effect) { _effect = effect; }
        static void on_keys(DWORD keys);

    private:
        static BYTE _led_count;
        static Effect* _effect;
        static DWORD _keys;
};

extern EffectNone effect_none;
extern EffectFlash effect_flash;
extern EffectRunning effect_running;
extern EffectRainbow effect_rainbow;
