/******************************************************************************\
    Split Keyboard
    Copyright (C) 2021-2025 - Max Maisel

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

#include "../types.h"

namespace dev {
    struct PwrStruct {
        WORD CR;
        WORD CSR;
    };

    volatile PwrStruct* const PWR = (volatile PwrStruct*)0x40007000;

    namespace pwr {
        // CR
        enum : WORD {
            VOS_MASK = 0x0000C000,
            VOS_SCALE3 = (1 << 14),
            VOS_SCALE2 = (2 << 14),
            ADCDC1 = (1 << 13),
            MRLVDS = (1 << 11),
            LPLVDS = (1 << 10),
            FPDS = (1 << 9),
            DBP = (1 << 8),
            PLS_MASK = 0x000000E0,
            PVDE = (1 << 4),
            CSBF = (1 << 3),
            CWUF = (1 << 2),
            PDDS = (1 << 1),
            LPDS = (1 << 0)
        };

        // CSR
        enum : WORD {
            VOSRDY = (1 << 14),
            BRE = (1 << 9),
            EWUP = (1 << 8),
            BRR = (1 << 3),
            PVDO = (1 << 2),
            SBF = (1 << 1),
            WUF = (1 << 0)
        };
    }
}
