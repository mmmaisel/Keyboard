/**********************************************************************\
 * Keyboard
 *
 * STM32F401 RCC register map
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
    struct RccStruct {
        WORD CR;
        WORD PLLCFGR;
        WORD CFGR;
        WORD CIR;
        WORD AHB1RSTR;
        WORD AHB2RSTR;
        DWORD : 64;
        WORD APB1RSTR;
        WORD APB2RSTR;
        DWORD : 64;
        WORD AHB1ENR;
        WORD AHB2ENR;
        DWORD : 64;
        WORD APB1ENR;
        WORD APB2ENR;
        DWORD : 64;
        WORD AHB1LPENR;
        WORD AHB2LPENR;
        DWORD : 64;
        WORD APB1LPENR;
        WORD APB2LPENR;
        DWORD : 64;
        WORD BDCR;
        WORD CSR;
        DWORD : 64;
        WORD SSCGR;
        WORD PLLI2SCFGR;
        WORD : 32;
        WORD DCKCFGR;
    };

    volatile RccStruct* const RCC = (volatile RccStruct*)0x40023800;

    namespace rcc {
        // CR
        enum : WORD {
            PLLI2CRDY = (1 << 27),
            PLLI2CON = (1 << 26),
            PLLRDY = (1 << 25),
            PLLON = (1 << 24),
            CSSON = (1 << 19),
            HSEBYP = (1 << 18),
            HSERDY = (1 << 17),
            HSEON = (1 << 16),
            HSICAL_MASK = 0x0000FF00,
            HSICAL_POS = 8,
            HSITRIM_MASK = 0x000000F80,
            HSITRIM_POS = 3,
            HSIRDY = (1 << 1),
            HSION = (1 << 0)
        };

        // PLLCFGR
        enum : WORD {
            PLLQ_MASK = 0x0F000000,
            PLLQ_POS = 24,
            PLLSRC_HSE = (1 << 22),
            PLLP_MASK = 0x00030000,
            PLLP1 = (1 << 17),
            PLLP0 = (1 << 16),
            PLLN_MASK = 0x00007FC0,
            PLLN_POS = 6,
            PLLM_MASK = 0x0000003F,
            PLLM_POS = 0
        };

        // CFGR
        enum : WORD {
            MCO2_MASK = 0xC0000000,
            MCO2_SYSCLK = (0 << 30),
            MCO2_PLLI2S = (1 << 30),
            MCO2_HSE = (2U << 30),
            MCO2_PLL = (3U << 30),
            MCO2PRE_MASK = 0x38000000,
            MCO2PRE_POS = 27,
            MCO1PRE_MASK = 0x07000000,
            MCO1PRE_POS = 24,
            I2SSRC = (1 << 23),
            MCO1_MASK = 0x00600000,
            MCO1_HSI = (0 << 21),
            MCO1_LSE = (1 << 21),
            MCO1_HSE = (2 << 21),
            MCO1_PLL = (3 << 21),
            RTCPRE_MASK = 0x001F0000,
            RTCPRE_POS = 16,
            PPRE2_MASK = 0x0000E000,
            PPRE2_DIV1 = (0 << 10),
            PPRE2_DIV2 = (4 << 10),
            PPRE2_DIV4 = (5 << 10),
            PPRE2_DIV8 = (6 << 10),
            PPRE2_DIV16 = (7 << 10),
            PPRE1_MASK = 0x00001C00,
            PPRE1_DIV1 = (0 << 10),
            PPRE1_DIV2 = (4 << 10),
            PPRE1_DIV4 = (5 << 10),
            PPRE1_DIV8 = (6 << 10),
            PPRE1_DIV16 = (7 << 10),
            HPRE_MASK = 0x000000F0,
            HPRE_POS = 4,
            SWS_MASK = 0x0000000C,
            SWS_HSI = (0 << 2),
            SWS_HSE = (1 << 2),
            SWS_PLL = (2 << 2),
            SW_MASK = 0x00000003,
            SW_HSI = (0 << 0),
            SW_HSE = (1 << 0),
            SW_PLL = (2 << 0)
        };

        // AHB1ENR
        enum : WORD {
            DMA2EN = (1 << 22),
            DMA1EN = (1 << 21),
            CRCEN = (1 << 12),
            GPIOHEN = (1 << 7),
            GPIOEEN = (1 << 4),
            GPIODEN = (1 << 3),
            GPIOCEN = (1 << 2),
            GPIOBEN = (1 << 1),
            GPIOAEN = (1 << 0)
        };

        // APB1ENR
        enum : WORD {
            PWREN = (1 << 28),
            I2C3EN = (1 << 23),
            I2C2EN = (1 << 22),
            I2C1EN = (1 << 21),
            USART2EN = (1 << 17),
            SPI3EN = (1 << 15),
            SPI2EN = (1 << 14),
            WWDGEN = (1 << 11),
            TIM5EN = (1 << 3),
            TIM4EN = (1 << 2),
            TIM3EN = (1 << 1),
            TIM2EN = (1 << 0)
        };

        // APB2ENR
        enum : WORD {
            TIM11EN = (1 << 18),
            TIM10EN = (1 << 17),
            TIM9EN = (1 << 16),
            SYSCFGEN = (1 << 14),
            SPI4EN = (1 << 13),
            SPI1EN = (1 << 12),
            SDIOEN = (1 << 11),
            ADC1EN = (1 << 8),
            USART6EN = (1 << 5),
            USART1EN = (1 << 4),
            TIM1EN = (1 << 0)
        };

        // AHB2ENR
        enum : WORD {
            USBEN = (1 << 7)
        };
    }
}

