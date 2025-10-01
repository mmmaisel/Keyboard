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

class Effect {
    public:
        virtual void run() = 0;
};

class EffectNone : public Effect {
    public:
        void run();
};

class EffectFlash : public Effect {
    public:
        void run();
};

class EffectController {
    // Static class
    EffectController() = delete;
    EffectController(const EffectController&) = delete;
    EffectController(EffectController&&) = delete;
    ~EffectController() = delete;

    public:
        static inline void set_effect(Effect* effect) { _effect = effect; }
        static void on_change(DWORD keys);

    private:
        static Effect* _effect;
        static DWORD _keys;
};

extern EffectNone effect_none;
extern EffectFlash effect_flash;
