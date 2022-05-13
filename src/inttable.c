/**********************************************************************\
 * Keyboard
 *
 * ARM interrupt table
 * Must use C compiler!
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
/// @file

#include <stdint.h>

extern void reset_vector();
extern void nmi_vector();
extern void fault_vector();
extern void default_vector();

extern void tim2_vector();
extern void tim3_vector();

extern uint8_t __stack_top__;

/// @internal
/// The interrupt table - must be at fixed address in ROM.
__attribute__ ((section(".inttable")))
__attribute__ ((aligned (1024)))
void (* const __inttable__[])() = {
    // Vector                         num  bit  name
    (void(*)())&__stack_top__,     // 0     -    Initial stack pointer (from linker script)
    reset_vector,                  // 1     -    Reset vector
    nmi_vector,                    // 2     -    NMI vector
    fault_vector,                  // 3     -    hard fault vector
    default_vector,                // 4     -    MPU fault vector
    default_vector,                // 5     -    bus fault vector
    default_vector,                // 6     -    usage fault vector
    0, 0, 0, 0,                    // -     -    Reserved
    default_vector,                // 11    -    SVCall
    default_vector,                // 12    -    Debug monitor vector
    0,                             // -     -    Reserved
    default_vector,                // 14    -    PendSV
    default_vector,                // 15    -    SysTick
    default_vector,                // 16    0    Window Watchdog
    default_vector,                // 17    1    PVD
    default_vector,                // 18    2    Tamper and Timestamps
    default_vector,                // 19    3    RTC Wakeup
    default_vector,                // 20    4    Flash
    default_vector,                // 21    5    RCC
    default_vector,                // 22    6    EXTI0
    default_vector,                // 23    7    EXTI1
    default_vector,                // 24    8    EXTI2
    default_vector,                // 25    9    EXTI3
    default_vector,                // 26    10   EXTI4
    default_vector,                // 27    11   DMA1 Stream 0
    default_vector,                // 28    12   DMA1 Stream 1
    default_vector,                // 29    13   DMA1 Stream 2
    default_vector,                // 30    14   DMA1 Stream 3
    default_vector,                // 31    15   DMA1 Stream 4
    default_vector,                // 32    16   DMA1 Stream 5
    default_vector,                // 33    17   DMA1 Stream 6
    default_vector,                // 34    18   ADCs
    0, 0, 0, 0,                    // -     -    Reserved
    default_vector,                // 39    23   EXTI9-5
    default_vector,                // 40    24   TIM1 Break and TIM9
    default_vector,                // 41    25   TIM1 Update and TIM10
    default_vector,                // 42    26   TIM1 Trigger and Commutation and TIM11
    default_vector,                // 43    27   TIM1 Capture Compare
    tim2_vector,                   // 44    28   TIM2
    tim3_vector,                   // 45    29   TIM3
    default_vector,                // 46    30   TIM4
    default_vector,                // 47    31   I2C1 Event
    default_vector,                // 48    32   I2C1 Error
    default_vector,                // 49    33   I2C2 Event
    default_vector,                // 50    34   I2C2 Error
    default_vector,                // 51    35   SPI1
    default_vector,                // 52    36   SPI2
    default_vector,                // 53    37   USART1
    default_vector,                // 54    38   USART2
    0,                             // -     -    Reserved
    default_vector,                // 56    40   EXTI15-10
    default_vector,                // 57    41   RTC Alarm
    default_vector,                // 58    42   USB OTG Wakeup
    0, 0, 0, 0,                    // -     -    Reserved
    default_vector,                // 63    47   DMA1 Stream7
    0,                             // -     -    Reserved
    default_vector,                // 65    49   SDIO
    default_vector,                // 66    50   TIM5
    default_vector,                // 67    51   SPI3
    0, 0, 0, 0,                    // -     -    Reserved
    default_vector,                // 72    56   DMA2 Stream 0
    default_vector,                // 73    57   DMA2 Stream 1
    default_vector,                // 74    58   DMA2 Stream 2
    default_vector,                // 75    59   DMA2 Stream 3
    default_vector,                // 76    60   DMA2 Stream 4
    0, 0, 0, 0, 0, 0,              // -     -    Reserved
    default_vector,                // 83    67   USB OTG FS
    default_vector,                // 84    68   DMA2 Stream 5
    default_vector,                // 85    69   DMA2 Stream 6
    default_vector,                // 86    70   DMA2 Stream 7
    default_vector,                // 87    71   USART6
    default_vector,                // 88    72   I2C3 Event
    default_vector,                // 89    73   I2C3 Error
    0, 0, 0, 0, 0, 0, 0,           // -     -    Reserved
    default_vector,                // 97    75   FPU
    0, 0,                          // -     -    Reserved
    default_vector                 // 100   78   SPI4
};
