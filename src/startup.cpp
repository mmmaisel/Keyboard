/**********************************************************************\
 * Keyboard
 *
 * Startup code, reset/fail interrupts
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
/*
*
* Copyright (C) Patryk Jaworski <regalis@regalis.com.pl>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/
#include <stdint.h>
#include <string.h>

#include "dev/core.h"
#include "dev/flash.h"
#include "dev/pwr.h"
#include "dev/rcc.h"
#include "module.h"

extern void main();

extern uint8_t __data_start__;
extern uint8_t __data_load__;
extern uint8_t __data_end__;

extern uint8_t __bss_start__;
extern uint8_t __bss_end__;

typedef void (*ctorPtr)();

extern ctorPtr __init_array_start__;
extern ctorPtr __init_array_end__;

using namespace dev;
using namespace dev::core;
using namespace dev::flash;
using namespace dev::pwr;
using namespace dev::rcc;

extern "C" {
    /// @internal
    /// The reset vector. Here does all start after the
    /// CPU is switched on. It sets the clock and port config,
    /// initializes all global variables and classes,
    /// and finally calls main.
    void reset_vector() {
        // Enable Power Control clock
        RCC->APB1ENR |= PWREN;
        // Regulator voltage scaling output selection: Scale 2
        // Configure low power in stop mode.
        PWR->CR |= VOS_SCALE2 | MRLVDS | LPLVDS | FPDS | LPDS;

        // Wait until HSI ready (high speed internal oscillator)
        while((RCC->CR & HSIRDY) == 0);

        // Switch to HSI
        RCC->CFGR = (RCC->CFGR & ~SW_MASK) | SW_HSI;

        // Disable main PLL
        RCC->CR &= ~PLLON;
        // Enable crystal
        RCC->CR |= HSEON;

        // Wait until PLL ready (disabled)
        while((RCC->CR & PLLRDY) != 0);

        // Configure PLL
        // f_in  = HSE (8 MHz)
        // f_vco = f_in * N/M
        // f_out = f_vco / P
        // f_usb_sd = f_vco / Q
        //
        // N = 216, M = 6, P = 6, Q = 6
        // f_vco = 288MHz, f_out = 48 MHz, f_usb_sd = 48 MHz
        // f_vco range: [192; 432] MHz
        RCC->PLLCFGR &=
            ~(PLLM_MASK | PLLN_MASK | PLLP_MASK | PLLQ_MASK | PLLSRC_HSE);
        RCC->PLLCFGR |= PLLSRC_HSE |
            (6 << PLLQ_POS) | PLLP1 | (216 << PLLN_POS) | (6 << PLLM_POS);

        // PLL On
        RCC->CR |= PLLON;
        // Wait until PLL is locked
        while((RCC->CR & PLLRDY) == 0);

        // FLASH configuration block
        FLASH->ACR = (FLASH->ACR & ~LATENCY_MASK)
            | ICEN | DCEN | PRFTEN | (1 << LATENCY_POS);

        // Check flash latency
        if((FLASH->ACR & LATENCY_MASK) != (1 << LATENCY_POS)) {
            for(;;);
        }

        // Set clock source to PLL
        RCC->CFGR |=  SW_PLL;
        // Check clock source
        while((RCC->CFGR & SWS_PLL) != SWS_PLL);

        // Set HCLK (AHB1) prescaler (DIV1)
        RCC->CFGR = (RCC->CFGR & ~HPRE_MASK) | (0 << HPRE_POS);

        // Set APB1 Low speed prescaler (APB1) DIV4
        RCC->CFGR = (RCC->CFGR & ~PPRE1_MASK) | PPRE1_DIV4;

        // SET APB2 High speed prescaler (APB2) DIV1
        RCC->CFGR = (RCC->CFGR & ~PPRE2_MASK) | PPRE2_DIV1;

        // Init FPU
        SCB->CPACR = CP10_ACC_FULL | CP11_ACC_FULL;
        // Init deepsleep
        SCB->SCR |= SLEEPDEEP;

        // Put initial values into initialized global/static variables
        // cppcheck-suppress[comparePointers]
        memcpy(&__data_start__, &__data_load__, &__data_end__ - &__data_start__);

        // Clear bss
        // cppcheck-suppress[comparePointers]
        memset(&__bss_start__, 0, &__bss_end__ - &__bss_start__);

        // Detect module
        Module::detect();

        // Call global constructors
        // cppcheck-suppress[comparePointers]
        for(ctorPtr* ctor = &__init_array_start__; ctor < &__init_array_end__; ++ctor)
            (*ctor)();

        // Call main
        main();
    }
}
