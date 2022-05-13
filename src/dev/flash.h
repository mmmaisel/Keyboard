/**********************************************************************\
 * Keyboard
 *
 * STM32F401 flash register map
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
    struct FlashStruct {
        WORD ACR;
        WORD KEYR;
        WORD OPTKEYR;
        WORD SR;
        WORD CR;
        WORD OPTCR;
    };

    volatile FlashStruct* const FLASH = (volatile FlashStruct*)0x40023C00;

    namespace flash {
        // ACR
        enum : WORD {
            DCRST = (1 << 12),
            ICRST = (1 << 1),
            DCEN = (1 << 10),
            ICEN = (1 << 9),
            PRFTEN = (1 << 8),
            LATENCY_MASK = 0x0000000F,
            LATENCY_POS = 0
        };
    }
}
