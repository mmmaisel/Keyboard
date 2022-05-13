/**********************************************************************\
 * Keyboard
 *
 * STM32F401 GPIO register map
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
    struct GpioStruct {
        WORD MODER;
        WORD OTYPER;
        WORD OSPEEDR;
        WORD PUPDR;
        WORD IDR;
        WORD ODR;
        WORD BSRR;
        WORD LCKR;
        WORD AFRL;
        WORD AFRH;

        inline void set_odr(HWORD val) volatile {
            BSRR = WORD(val);
        }

        inline void clear_odr(HWORD val) volatile {
            BSRR = WORD(val) << 16;
        }
    };

    volatile GpioStruct* const GPIOA = (volatile GpioStruct*)0x40020000;
    volatile GpioStruct* const GPIOB = (volatile GpioStruct*)0x40020400;
    volatile GpioStruct* const GPIOC = (volatile GpioStruct*)0x40020800;
    volatile GpioStruct* const GPIOD = (volatile GpioStruct*)0x40020C00;
    volatile GpioStruct* const GPIOE = (volatile GpioStruct*)0x40021000;

    namespace gpio {
        // MODE
        enum : WORD {
            INPUT = 0,
            OUTPUT = 1,
            ALTERNATE = 2,
            ANALOG = 3
        };

        // PUPD
        enum : WORD {
            NOPULL = 0,
            PULLUP = 1,
            PULLDOWN = 2
        };
    }
}
