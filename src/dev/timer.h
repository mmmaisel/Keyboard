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
    struct TimerStruct {
        HWORD CR1;
        HWORD : 16;
        HWORD CR2;
        HWORD : 16;
        HWORD SMCR;
        HWORD : 16;
        HWORD DIER;
        HWORD : 16;
        HWORD SR;
        HWORD : 16;
        HWORD EGR;
        HWORD : 16;
        HWORD CCMR1;
        HWORD : 16;
        HWORD CCMR2;
        HWORD : 16;
        HWORD CCER;
        HWORD : 16;
        WORD CNT;
        HWORD PSC;
        HWORD : 16;
        WORD ARR;
        WORD : 32;
        WORD CCR1;
        WORD CCR2;
        WORD CCR3;
        WORD CCR4;
        WORD : 32;
        HWORD DCR;
        HWORD : 16;
        HWORD DMAR;
        HWORD : 16;
        HWORD OR;
    };

    volatile TimerStruct* const TIM2 = (volatile TimerStruct*)0x40000000;
    volatile TimerStruct* const TIM3 = (volatile TimerStruct*)0x40000400;
    volatile TimerStruct* const TIM4 = (volatile TimerStruct*)0x40000800;
    volatile TimerStruct* const TIM5 = (volatile TimerStruct*)0x40000C00;

    namespace timer {
        // CR1
        enum : WORD {
            CKD_MASK = 0x0300,
            CKD_POS = 8,
            ARPE = (1 << 7),
            CMS_MASK = 0x0060,
            CMS_POS = 5,
            DIR = (1 << 4),
            OPM = (1 << 3),
            URS = (1 << 2),
            UDIS = (1 << 1),
            CEN = (1 << 0)
        };

        // CR2
        enum : WORD {
            TI1S = (1 << 7),
            MMS_MASK = 0x0070,
            MMS_POS = 4,
            CCDS = (1 << 3)
        };

        // SMCR
        enum : WORD {
            ETP = (1 << 15),
            ECE = (1 << 14),
            ETPS_MASK = 0x3000,
            ETPS_POS = 12,
            ETF_MASK = 0x0F00,
            ETF_POS = 8,
            MSM = (1 << 7),
            TS_MASK = 0x0070,
            TS_POS = 4,
            SMS_MASK = 0x0007,
            SMS_POS = 0
        };

        // DIER
        enum : WORD {
            TDE = (1 << 14),
            CC4DE = (1 << 12),
            CC3DE = (1 << 11),
            CC2DE = (1 << 10),
            CC1DE = (1 << 9),
            UDE = (1 << 8),
            TIE = (1 << 6),
            CC4IE = (1 << 4),
            CC3IE = (1 << 3),
            CC2IE = (1 << 2),
            CC1IE = (1 << 1),
            UIE = (1 << 0)
        };

        // SR
        enum : WORD {
            CC4OF = (1 << 12),
            CC3OF = (1 << 11),
            CC2OF = (1 << 10),
            CC1OF = (1 << 9),
            TIF = (1 << 6),
            CC4IF = (1 << 4),
            CC3IF = (1 << 3),
            CC2IF = (1 << 2),
            CC1IF = (1 << 1),
            UIF = (1 << 0)
        };

        // EGR
        enum : WORD {
            TG = (1 << 6),
            CC4G = (1 << 4),
            CC3G = (1 << 3),
            CC2G = (1 << 2),
            CC1G = (1 << 1),
            UG = (1 << 0)
        };

        // CCMR1
        enum : WORD {
            OC2CE = (1 << 15),
            OC2M_MASK = 0x7000,
            OC2M_POS = 12,
            OC2PE = (1 << 11),
            OC2FE = (1 << 10),
            CC2S_MASK = 0x0300,
            CC2S_POS = 8,
            OC1CE = (1 << 7),
            OC1M_MASK = 0x0070,
            OC1M_POS = 4,
            OC1PE = (1 << 3),
            PC1FE = (1 << 2),
            CC1S_MASK = 0x0003,
            CC1S_POS = 0
        };

        // CCMR2
        enum : WORD {
            OC4CE = (1 << 15),
            OC4M_MASK = 0x7000,
            OC4M_POS = 12,
            OC4PE = (1 << 11),
            OC4FE = (1 << 10),
            CC4S_MASK = 0x0300,
            CC4S_POS = 8,
            OC3CE = (1 << 7),
            OC3M_MASK = 0x0070,
            OC3M_POS = 4,
            OC3PE = (1 << 3),
            PC3FE = (1 << 2),
            CC3S_MASK = 0x0003,
            CC3S_POS = 0
        };

        // CCER
        enum : WORD {
            CC4NP = (1 << 15),
            CC4P = (1 << 13),
            CC4E = (1 << 12),
            CC3NP = (1 << 11),
            CC3P = (1 << 9),
            CC3E = (1 << 8),
            CC2NP = (1 << 7),
            CC2P = (1 << 5),
            CC2E = (1 << 4),
            CC1NP = (1 << 3),
            CC1P = (1 << 1),
            CC1E = (1 << 0)
        };

        // DCR
        enum : WORD {
            DBL_MASK = 0x1F00,
            DBL_POS = 8,
            DBA_MASK = 0x001F,
            DBA_POS = 0
        };

        // OR
        enum : WORD {
            ITR1_RMP_MASK = 0x0C00,
            ITR1_RMP_POS = 10,
            TI4_RMP_MASK = 0x00C0,
            TI4_RMP_POS = 6
        };
    }
}
