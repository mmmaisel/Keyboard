/******************************************************************************\
    Split Keyboard
    Copyright (C) 2022-2025 - Max Maisel

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
    struct UsartStruct {
        WORD SR;
        WORD DR;
        WORD BRR;
        WORD CR1;
        WORD CR2;
        WORD CR3;
        WORD GTPR;
    };

    volatile UsartStruct* const USART1 = (volatile UsartStruct*)0x40011000;
    volatile UsartStruct* const USART2 = (volatile UsartStruct*)0x40004400;
    volatile UsartStruct* const USART6 = (volatile UsartStruct*)0x40011400;

    namespace usart {
        // SR
        enum : WORD {
            CTS = (1 << 9),
            LBD = (1 << 8),
            TXE = (1 << 7),
            TC = (1 << 6),
            RXNE = (1 << 5),
            IDLE = (1 << 4),
            ORE = (1 << 3),
            NF = (1 << 2),
            FE = (1 << 1),
            PE = (1 << 0)
        };

        // BRR
        enum : WORD {
            BRR_MASK = 0xFFFF0000,
            DIV_MANT_MASK = 0x0000FFF0,
            DIV_MANT_POS = 4,
            DIV_FRAC_MASK = 0x0000000F,
            DIV_FRAC_POS = 0
        };

        // CR1
        enum : WORD {
            CR1_MASK = 0xFFFF4000,
            OVER8 = (1 << 15),
            UE = (1 << 13),
            M = (1 << 12),
            WAKE = (1 << 11),
            PCE = (1 << 10),
            PS = (1 << 9),
            PEIE = (1 << 8),
            TXEIE = (1 << 7),
            TCIE = (1 << 6),
            RXNEIE = (1 << 5),
            IDLEIE = (1 << 4),
            TE = (1 << 3),
            RE = (1 << 2),
            RWU = (1 << 1),
            SBK = (1 << 0)
        };

        // CR2
        enum : WORD {
            CR2_MASK = 0xFFFF8090,
            LINEN = (1 << 14),
            STOP_MASK = 0x00003000,
            STOP_POS = 12,
            CLKEN = (1 << 11),
            CPOL = (1 << 10),
            CPHA = (1 << 9),
            LBCL = (1 << 8),
            LBDIE = (1 << 6),
            LBDL = (1 << 5),
            ADDR_MASK = 0x0000000F,
            ADDR_POS = 0
        };

        // CR3
        enum : WORD {
            CR3_MASK = 0xFFFFF000,
            ONEBIT = (1 << 11),
            CTSIE = (1 << 10),
            CTSE = (1 << 9),
            RTSE = (1 << 8),
            DMAT = (1 << 7),
            DMAR = (1 << 6),
            SCEN = (1 << 5),
            NACK = (1 << 4),
            HDSEL = (1 << 3),
            IRLP = (1 << 2),
            IREN = (1 << 1),
            EIE = (1 << 0)
        };

        // GTPR
        enum : WORD {
            GT_MASK = 0x0000FF00,
            GT_POS = 8,
            PSC_MASK = 0x000000FF,
            PSC_POS = 0
        };
    }
}
