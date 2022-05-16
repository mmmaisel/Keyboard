/**********************************************************************\
 * Keyboard
 *
 * ARM M4 external interrupt register map
 **********************************************************************
 * Copyright (C) 2021-2022 - Max Maisel
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

#include "../types.h"

namespace dev {
    struct ExtiStruct {
        WORD IMR;
        WORD EMR;
        WORD RTSR;
        WORD FTSR;
        WORD SWIER;
        WORD PR;
    };

    volatile ExtiStruct* const EXTI = (volatile ExtiStruct*)0x40013C00;

    namespace exti {
        enum : WORD {
            EXTINUM_WAKEUP_USB = 18
        };
    }
}
