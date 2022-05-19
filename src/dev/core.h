/**********************************************************************\
 * Keyboard
 *
 * ARM M4 core register map
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
    struct ScbStruct {
        WORD CPUID;
        WORD ICSR;
        WORD VTOR;
        WORD AIRCR;
        WORD SCR;
        WORD CCR;
        BYTE SHP[12];
        WORD SHCSR;
        WORD CFSR;
        WORD HFSR;
        WORD DFSR;
        WORD MMFAR;
        WORD BFAR;
        WORD AFSR;
        WORD PFR[2];
        WORD DFR;
        WORD ADR;
        WORD MMFR[4];
        WORD ISAR[5];
        WORD _reserved0[5];
        WORD CPACR;
    };

    volatile ScbStruct* const SCB = (volatile ScbStruct*)0xE000ED00;

    enum : WORD {
        CP10_ACC_FULL = 0x00300000,
        CP11_ACC_FULL = 0x00C00000
    };

    struct NvicStruct {
        WORD EN[8];
        WORD _unused0[24];
        WORD DIS[8];
        WORD _unused1[24];
        WORD PEND[8];
        WORD _unused2[24];
        WORD UNPEND[8];
        WORD _unused3[24];
        WORD ACTIVE[8];
        WORD _unused4[56];
        BYTE PRI[240];
        WORD _unused5[644];
        WORD SWTRIG;

        inline void enable_isr(BYTE num) volatile {
            EN[num / 32] = (1 << (num % 32));
        }

        inline void disable_isr(BYTE num) volatile {
            EN[num / 32] = (1 << (num % 32));
        }
    };

    volatile NvicStruct* const NVIC = (volatile NvicStruct*)0xE000E100;

    namespace isrnum {
        enum : WORD {
            DMA1S5 = 16,
            DMA1S6 = 17,
            TIM2 = 28,
            TIM3 = 29,
            USART1 = 37,
            USART2 = 38,
            USB_WAKEUP = 42,
            DMA2S1 = 57,
            DMA2S2 = 58,
            USB = 67,
            DMA2S6 = 69,
            DMA2S7 = 70,
            USART6 = 71,
        };
    }

    struct SystickStruct {
        WORD CTRL;
        WORD LOAD;
        WORD VAL;
        WORD CALIB;
    };

    volatile SystickStruct* const SYSTICK = (volatile SystickStruct*)0xE000E010;

    volatile WORD* const DBGMCU_CR = (volatile WORD*)0xE0042004;

    namespace core {
        enum : WORD {
            SEVEONPEND = (1 << 4),
            SLEEPDEEP = (1 << 2),
            SLEEPONEXIT = (1 << 1)
        };

        // SYSTICK CTRL
        enum : WORD {
            CNTFLAG = (1 << 16),
            CLKSRC = (1 << 2),
            TICKINT = (1 << 1),
            TICKEN = (1 << 0)
        };
    }

    namespace debug {
        enum : WORD {
            DBG_STBY = (1 << 2),
            DBG_STOP = (1 << 1),
            DBG_SLEEP = (1 << 0)
        };
    };
}
