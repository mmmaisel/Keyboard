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

#include "event.h"
#include "led_matrix.h"

class Effect {
    Effect(const Effect&) = delete;
    Effect(Effect&&) = delete;

    public:
        virtual void activate();
        virtual void run(BYTE led_count, DWORD new_keys, DWORD old_keys) = 0;

    protected:
        Effect() = default;
};

class EffectNone : public Effect {
    public:
        virtual void run(
            BYTE led_count, DWORD new_keys, DWORD old_keys) override;
};

class EffectBacklight : public Effect {
    public:
        virtual void activate() override;
        virtual void run(
            BYTE led_count, DWORD new_keys, DWORD old_keys) override;

    private:
        static const BYTE STATE_COUNT = 9;

        BYTE _changed = 1;
        BYTE _state = STATE_COUNT-1;
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
        TickType_t _ticks;
        float _phase;
};

class EffectController {
    // Static class
    EffectController() = delete;
    EffectController(const EffectController&) = delete;
    EffectController(EffectController&&) = delete;
    ~EffectController() = delete;

    public:
        static void initialize(BYTE page, const LedMatrixConfig* config);
        static void set_effect(Effect* effect);
        static void on_keys(BYTE page, DWORD keys);
        static Effect* effect_by_id(EffectId id);

    private:
        static BYTE _page;
        static BYTE _led_count;
        static Effect* _effect;
        static DWORD _keys;
};

extern EffectNone effect_none;
extern EffectBacklight effect_backlight;
extern EffectFlash effect_flash;
extern EffectRunning effect_running;
extern EffectRainbow effect_rainbow;
