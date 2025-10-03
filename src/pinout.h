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

#include "types.h"
#include "dev/gpio.h"

namespace pinout {
    using namespace dev::gpio;

    namespace right {
        enum : WORD {
            // PORTB
            PWREN0 = (1 << 0),
            PWREN1 = (1 << 1),
            PWREN2 = (1 << 2),
            LVEN = (1 << 5),

            KR1 = (1 << 8),
            KR2 = (1 << 9),
            KR3 = (1 << 10),
            KR4 = (1 << 12),
            KR5 = (1 << 13),
            KR6 = (1 << 14),
            KR7 = (1 << 15),

            // PORTC
            KC1 = (1 << 0),
            KC2 = (1 << 1),
            KC3 = (1 << 2),
            KC4 = (1 << 3),
            KC5 = (1 << 4),
            KC6 = (1 << 5),
            NODEID0 = (1 << 12),
            NODEID1 = (1 << 13),

            // PORTD
            LC1 = (1 << 0),
            LC2 = (1 << 1),
            LC3 = (1 << 2),
            LC4 = (1 << 3),
            LC5 = (1 << 4),
            LC6 = (1 << 5),
            LC7 = (1 << 6),
            LC8 = (1 << 7),
            LC9 = (1 << 8),

            // PORTE
            LR1 = (1 << 0),
            LR2 = (1 << 1),
            LR3 = (1 << 2),
            LR4 = (1 << 3),
            LR5 = (1 << 4),
            LR6 = (1 << 5),
            LR7 = (1 << 6),
            LR8 = (1 << 7),
            LR9 = (1 << 8),
            LR10 = (1 << 9),
            LR11 = (1 << 10),
            LR12 = (1 << 11),
            LR13 = (1 << 12),
            LR14 = (1 << 13),
            LR15 = (1 << 14),
        };

        enum : WORD {
            // PORTA
            MODE_UART2_TX = (ALTERNATE << (2*2)),
            MODE_UART2_RX = (ALTERNATE << (2*3)),
            MODE_VBUS = (ALTERNATE << (2*9)),
            MODE_DM = (ALTERNATE << (2*11)),
            MODE_DP = (ALTERNATE << (2*12)),

            // PORTB
            MODE_PWREN0 = (OUTPUT << (2*0)),
            MODE_PWREN1 = (OUTPUT << (2*1)),
            MODE_PWREN2 = (OUTPUT << (2*2)),
            MODE_LVEN = (OUTPUT << (2*5)),
            MODE_UART1_TX = (ALTERNATE << (2*6)),
            MODE_UART1_RX = (ALTERNATE << (2*7)),
            MODE_KR1 = (INPUT << (2*8)),
            MODE_KR2 = (INPUT << (2*9)),
            MODE_KR3 = (INPUT << (2*10)),
            MODE_KR4 = (INPUT << (2*12)),
            MODE_KR5 = (INPUT << (2*13)),
            MODE_KR6 = (INPUT << (2*14)),
            MODE_KR7 = (INPUT << (2*15)),

            // PORTC
            MODE_KC1 = (OUTPUT << (2*0)),
            MODE_KC2 = (OUTPUT << (2*1)),
            MODE_KC3 = (OUTPUT << (2*2)),
            MODE_KC4 = (OUTPUT << (2*3)),
            MODE_KC5 = (OUTPUT << (2*4)),
            MODE_KC6 = (OUTPUT << (2*5)),
            MODE_UART6_TX = (ALTERNATE << (2*6)),
            MODE_UART6_RX = (ALTERNATE << (2*7)),
            MODE_NODEID0 = (INPUT << (2*12)),
            MODE_NODEID1 = (INPUT << (2*13)),

            // PORTD
            MODE_LC1 = (OUTPUT << (2*0)),
            MODE_LC2 = (OUTPUT << (2*1)),
            MODE_LC3 = (OUTPUT << (2*2)),
            MODE_LC4 = (OUTPUT << (2*3)),
            MODE_LC5 = (OUTPUT << (2*4)),
            MODE_LC6 = (OUTPUT << (2*5)),
            MODE_LC7 = (OUTPUT << (2*6)),
            MODE_LC8 = (OUTPUT << (2*7)),
            MODE_LC9 = (OUTPUT << (2*8)),

            // PORTE
            MODE_LR1 = (OUTPUT << (2*0)),
            MODE_LR2 = (OUTPUT << (2*1)),
            MODE_LR3 = (OUTPUT << (2*2)),
            MODE_LR4 = (OUTPUT << (2*3)),
            MODE_LR5 = (OUTPUT << (2*4)),
            MODE_LR6 = (OUTPUT << (2*5)),
            MODE_LR7 = (OUTPUT << (2*6)),
            MODE_LR8 = (OUTPUT << (2*7)),
            MODE_LR9 = (OUTPUT << (2*8)),
            MODE_LR10 = (OUTPUT << (2*9)),
            MODE_LR11 = (OUTPUT << (2*10)),
            MODE_LR12 = (OUTPUT << (2*11)),
            MODE_LR13 = (OUTPUT << (2*12)),
            MODE_LR14 = (OUTPUT << (2*13)),
            MODE_LR15 = (OUTPUT << (2*14)),
        };

        enum : WORD {
            // PORTA
            AFRL_UART2_TX = (7 << (4*2)),
            AFRL_UART2_RX = (7 << (4*3)),
            AFRH_VBUS = (10 << (4*(9-8))),
            AFRH_DM = (10 << (4*(11-8))),
            AFRH_DP = (10 << (4*(12-8))),

            // PORTB
            AFRL_UART1_TX = (7 << (4*6)),
            AFRL_UART1_RX = (7 << (4*7)),

            // PORTC
            AFRL_UART6_TX = (8 << (4*6)),
            AFRL_UART6_RX = (8u << (4*7)),
        };

        enum : WORD {
            // PORTB
            PUPD_PWREN0 = (PULLDOWN << (2*0)),
            PUPD_PWREN1 = (PULLDOWN << (2*1)),
            PUPD_PWREN2 = (PULLDOWN << (2*2)),
            PUPD_LVEN = (PULLUP << (2*5)),
            PUPD_KR1 = (PULLDOWN << (2*8)),
            PUPD_KR2 = (PULLDOWN << (2*9)),
            PUPD_KR3 = (PULLDOWN << (2*10)),
            PUPD_KR4 = (PULLDOWN << (2*12)),
            PUPD_KR5 = (PULLDOWN << (2*13)),
            PUPD_KR6 = (PULLDOWN << (2*14)),
            PUPD_KR7 = unsigned((PULLDOWN << (2*15))),
        };
    }

    namespace left {
        enum : WORD {
            // PORTA
            LC1 = (1 << 0),
            LC2 = (1 << 1),
            LC3 = (1 << 2),
            LC4 = (1 << 3),
            LC5 = (1 << 4),
            LC6 = (1 << 5),
            LC7 = (1 << 6),
            LC8 = (1 << 7),
            LC9 = (1 << 8),
            LC10 = (1 << 9),
            LR1 = (1 << 10),
            LR2 = (1 << 11),
            LR3 = (1 << 12),

            // PORTB
            LR4 = (1 << 0),
            LR5 = (1 << 1),
            LR6 = (1 << 2),
            LVEN = (1 << 5),
            LR7 = (1 << 8),
            LR8 = (1 << 9),
            LR9 = (1 << 10),
            LR10 = (1 << 12),
            LR11 = (1 << 13),
            LR12 = (1 << 14),
            KR7 = (1 << 15),

            // PORTC
            KR1 = (1 << 0),
            KR2 = (1 << 1),
            KR3 = (1 << 2),
            KR4 = (1 << 3),
            KR5 = (1 << 4),
            KR6 = (1 << 5),
            KC1 = (1 << 6),
            KC2 = (1 << 7),
            KC3 = (1 << 8),
            KC4 = (1 << 9),
            KC5 = (1 << 10),
            KC6 = (1 << 11),
            NODEID0 = (1 << 12),
            NODEID1 = (1 << 13)
        };

        enum : WORD {
            // PORTA
            MODE_LC1 = (OUTPUT << (2*0)),
            MODE_LC2 = (OUTPUT << (2*1)),
            MODE_LC3 = (OUTPUT << (2*2)),
            MODE_LC4 = (OUTPUT << (2*3)),
            MODE_LC5 = (OUTPUT << (2*4)),
            MODE_LC6 = (OUTPUT << (2*5)),
            MODE_LC7 = (OUTPUT << (2*6)),
            MODE_LC8 = (OUTPUT << (2*7)),
            MODE_LC9 = (OUTPUT << (2*8)),
            MODE_LC10 = (OUTPUT << (2*9)),
            MODE_LR1 = (OUTPUT << (2*10)),
            MODE_LR2 = (OUTPUT << (2*11)),
            MODE_LR3 = (OUTPUT << (2*12)),
            // Dual-use pins for debugging.
            MODE_UART6_TX = (ALTERNATE << (2*11)),
            MODE_UART6_RX = (ALTERNATE << (2*12)),

            // PORTB
            MODE_LR4 = (OUTPUT << (2*0)),
            MODE_LR5 = (OUTPUT << (2*1)),
            MODE_LR6 = (OUTPUT << (2*2)),
            MODE_LVEN = (OUTPUT << (2*5)),
            MODE_UART1_TX = (ALTERNATE << (2*6)),
            MODE_UART1_RX = (ALTERNATE << (2*7)),
            MODE_LR7 = (OUTPUT << (2*8)),
            MODE_LR8 = (OUTPUT << (2*9)),
            MODE_LR9 = (OUTPUT << (2*10)),
            MODE_LR10 = (OUTPUT << (2*12)),
            MODE_LR11 = (OUTPUT << (2*13)),
            MODE_LR12 = (OUTPUT << (2*14)),
            MODE_KR7 = (INPUT << (2*15)),

            // PORTC
            MODE_KR1 = (INPUT << (2*0)),
            MODE_KR2 = (INPUT << (2*1)),
            MODE_KR3 = (INPUT << (2*2)),
            MODE_KR4 = (INPUT << (2*3)),
            MODE_KR5 = (INPUT << (2*4)),
            MODE_KR6 = (INPUT << (2*5)),
            MODE_KC1 = (OUTPUT << (2*6)),
            MODE_KC2 = (OUTPUT << (2*7)),
            MODE_KC3 = (OUTPUT << (2*8)),
            MODE_KC4 = (OUTPUT << (2*9)),
            MODE_KC5 = (OUTPUT << (2*10)),
            MODE_KC6 = (OUTPUT << (2*11)),
            MODE_NODEID0 = (INPUT << (2*12)),
            MODE_NODEID1 = (INPUT << (2*13))
        };

        enum : WORD {
            // PORTA
            AFRH_UART6_TX = (8 << (4*3)),
            AFRH_UART6_RX = (8 << (4*4)),

            // PORTB
            AFRL_UART1_TX = (7 << (4*6)),
            AFRL_UART1_RX = (7 << (4*7)),
        };

        enum : WORD {
            // PORTB
            PUPD_LVEN = (PULLUP << (2*5)),
            PUPD_KR7 = unsigned((PULLDOWN << (2*15))),

            // PORTC
            PUPD_KR1 = (PULLDOWN << (2*0)),
            PUPD_KR2 = (PULLDOWN << (2*1)),
            PUPD_KR3 = (PULLDOWN << (2*2)),
            PUPD_KR4 = (PULLDOWN << (2*3)),
            PUPD_KR5 = (PULLDOWN << (2*4)),
            PUPD_KR6 = (PULLDOWN << (2*5)),
        };
    }

    namespace num {
        enum : WORD {
            // PORTA
            LR1 = (1 << 0),
            LR2 = (1 << 1),
            LR3 = (1 << 2),
            LR4 = (1 << 3),
            LR5 = (1 << 4),
            LR6 = (1 << 5),
            LR7 = (1 << 6),
            LR8 = (1 << 7),
            LR9 = (1 << 8),

            // PORTB
            LVEN = (1 << 5),
            LC1 = (1 << 8),
            LC2 = (1 << 9),
            LC3 = (1 << 10),
            LC4 = (1 << 12),
            LC5 = (1 << 13),
            LC6 = (1 << 14),

            // PORTC
            KR1 = (1 << 0),
            KR2 = (1 << 1),
            KR3 = (1 << 2),
            KR4 = (1 << 3),
            KR5 = (1 << 4),
            KC1 = (1 << 5),
            KC2 = (1 << 6),
            KC3 = (1 << 7),
            KC4 = (1 << 8),
            NODEID0 = (1 << 12),
            NODEID1 = (1 << 13)
        };

        enum : WORD {
            // PORTA
            MODE_LR1 = (OUTPUT << (2*0)),
            MODE_LR2 = (OUTPUT << (2*1)),
            MODE_LR3 = (OUTPUT << (2*2)),
            MODE_LR4 = (OUTPUT << (2*3)),
            MODE_LR5 = (OUTPUT << (2*4)),
            MODE_LR6 = (OUTPUT << (2*5)),
            MODE_LR7 = (OUTPUT << (2*6)),
            MODE_LR8 = (OUTPUT << (2*7)),
            MODE_LR9 = (OUTPUT << (2*8)),
            MODE_UART6_TX = (ALTERNATE << (2*11)),
            MODE_UART6_RX = (ALTERNATE << (2*12)),

            // PORTB
            MODE_LVEN = (OUTPUT << (2*5)),
            MODE_UART1_TX = (ALTERNATE << (2*6)),
            MODE_UART1_RX = (ALTERNATE << (2*7)),
            MODE_LC1 = (OUTPUT << (2*8)),
            MODE_LC2 = (OUTPUT << (2*9)),
            MODE_LC3 = (OUTPUT << (2*10)),
            MODE_LC4 = (OUTPUT << (2*12)),
            MODE_LC5 = (OUTPUT << (2*13)),
            MODE_LC6 = (OUTPUT << (2*14)),

            // PORTC
            MODE_KR1 = (INPUT << (2*0)),
            MODE_KR2 = (INPUT << (2*1)),
            MODE_KR3 = (INPUT << (2*2)),
            MODE_KR4 = (INPUT << (2*3)),
            MODE_KR5 = (INPUT << (2*4)),
            MODE_KC1 = (OUTPUT << (2*5)),
            MODE_KC2 = (OUTPUT << (2*6)),
            MODE_KC3 = (OUTPUT << (2*7)),
            MODE_KC4 = (OUTPUT << (2*8)),
            MODE_NODEID0 = (INPUT << (2*12)),
            MODE_NODEID1 = (INPUT << (2*13))
        };

        enum : WORD {
            // PORTA
            AFRH_UART6_TX = (8 << (4*3)),
            AFRH_UART6_RX = (8 << (4*4)),

            // PORTB
            AFRL_UART1_TX = (7 << (4*6)),
            AFRL_UART1_RX = (7 << (4*7)),
        };

        enum : WORD {
            // PORTB
            PUPD_LVEN = (PULLUP << (2*5)),

            // PORTC
            PUPD_KR1 = (PULLDOWN << (2*0)),
            PUPD_KR2 = (PULLDOWN << (2*1)),
            PUPD_KR3 = (PULLDOWN << (2*2)),
            PUPD_KR4 = (PULLDOWN << (2*3)),
            PUPD_KR5 = (PULLDOWN << (2*4)),
        };
    }

    namespace nav {
        enum : WORD {
            // PORTA
            LC1 = (1 << 0),
            LC2 = (1 << 1),
            LC3 = (1 << 2),
            LC4 = (1 << 3),
            LC5 = (1 << 4),

            // PORTB
            LR1 = (1 << 0),
            LR2 = (1 << 1),
            LR3 = (1 << 2),
            LVEN = (1 << 5),
            LR4 = (1 << 8),
            LR5 = (1 << 9),
            LR6 = (1 << 10),
            LR7 = (1 << 12),
            LR8 = (1 << 13),
            LR9 = (1 << 14),

            // PORTC
            KR1 = (1 << 0),
            KR2 = (1 << 1),
            KR3 = (1 << 2),
            KR4 = (1 << 3),
            KR5 = (1 << 4),
            KC1 = (1 << 5),
            KC2 = (1 << 6),
            KC3 = (1 << 7),
            NODEID0 = (1 << 12),
            NODEID1 = (1 << 13)
        };

        enum : WORD {
            // PORTA
            MODE_LC1 = (OUTPUT << (2*0)),
            MODE_LC2 = (OUTPUT << (2*1)),
            MODE_LC3 = (OUTPUT << (2*2)),
            MODE_LC4 = (OUTPUT << (2*3)),
            MODE_LC5 = (OUTPUT << (2*4)),
            MODE_UART6_TX = (ALTERNATE << (2*11)),
            MODE_UART6_RX = (ALTERNATE << (2*12)),

            // PORTB
            MODE_LR1 = (OUTPUT << (2*0)),
            MODE_LR2 = (OUTPUT << (2*1)),
            MODE_LR3 = (OUTPUT << (2*2)),
            MODE_LVEN = (OUTPUT << (2*5)),
            MODE_UART1_TX = (ALTERNATE << (2*6)),
            MODE_UART1_RX = (ALTERNATE << (2*7)),
            MODE_LR4 = (OUTPUT << (2*8)),
            MODE_LR5 = (OUTPUT << (2*9)),
            MODE_LR6 = (OUTPUT << (2*10)),
            MODE_LR7 = (OUTPUT << (2*12)),
            MODE_LR8 = (OUTPUT << (2*13)),
            MODE_LR9 = (OUTPUT << (2*14)),

            // PORTC
            MODE_KR1 = (INPUT << (2*0)),
            MODE_KR2 = (INPUT << (2*1)),
            MODE_KR3 = (INPUT << (2*2)),
            MODE_KR4 = (INPUT << (2*3)),
            MODE_KR5 = (INPUT << (2*4)),
            MODE_KC1 = (OUTPUT << (2*5)),
            MODE_KC2 = (OUTPUT << (2*6)),
            MODE_KC3 = (OUTPUT << (2*7)),
            MODE_NODEID0 = (INPUT << (2*12)),
            MODE_NODEID1 = (INPUT << (2*13))
        };

        enum : WORD {
            // PORTA
            AFRH_UART6_TX = (8 << (4*3)),
            AFRH_UART6_RX = (8 << (4*4)),

            // PORTB
            AFRL_UART1_TX = (7 << (4*6)),
            AFRL_UART1_RX = (7 << (4*7)),
        };

        enum : WORD {
            // PORTB
            PUPD_LVEN = (PULLUP << (2*5)),

            // PORTC
            PUPD_KR1 = (PULLDOWN << (2*0)),
            PUPD_KR2 = (PULLDOWN << (2*1)),
            PUPD_KR3 = (PULLDOWN << (2*2)),
            PUPD_KR4 = (PULLDOWN << (2*3)),
            PUPD_KR5 = (PULLDOWN << (2*4)),
        };
    }
}
