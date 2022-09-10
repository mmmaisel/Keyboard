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
#pragma once

#include "types.h"

class EffectController {
    // Static class
    EffectController() = delete;
    EffectController(const EffectController&) = delete;
    EffectController(EffectController&&) = delete;
    ~EffectController() = delete;

    public:
        enum Effect {
            NONE,
            FLASH
        };

        static void set_effect(Effect effect);
        static void on_update_page(const BYTE* old_page, const BYTE* new_page);

    private:
        static Effect m_active_effect;
};
