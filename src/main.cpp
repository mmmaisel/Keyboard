/**********************************************************************\
 * Keyboard
 *
 * C++ main function
 *  - please note that the real program starts in startup.cpp
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

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"

#include "module.h"
#include "led_matrix.h"
#include "key_matrix.h"
#include "system.h"
#include "usb_phy.h"
#include "uart.h"

#include "modular_keyboard.h"

#include "pinout.h"
#include "priority.h"
#include "dev/gpio.h"

const int STACK_SIZE_KEYBOARD = 0x100;

// Task memory
StaticTask_t taskMem_KEYBOARD;
StackType_t  taskStack_KEYBOARD[STACK_SIZE_KEYBOARD];

// XXX: led control usb protocol
// XXX: control LEDs via keys + FN

/// C++ main function, program starts here.
[[noreturn]] void main() {
    LedMatrix::initialize();
    KeyMatrix::initialize();
    if(Module::get_id() == Module::RIGHT)
        USBPhy::Initialize();

    using namespace pinout::right;

    dev::GPIOB->MODER |= MODE_PWREN0 | MODE_PWREN1 | MODE_PWREN2;
    dev::GPIOB->PUPDR |= PUPD_PWREN0 | PUPD_PWREN1 | PUPD_PWREN2;
    dev::GPIOB->set_odr(PWREN0 | PWREN1 | PWREN2);
//        dev::GPIOB->set_odr(PWREN0 | PWREN1 | PWREN2);
//        dev::GPIOB->clear_odr(PWREN0 | PWREN1 | PWREN2);

    xTaskCreateStatic(&ModularKeyboard::task, "KEYBOARD", STACK_SIZE_KEYBOARD, 0,
        priority::KEYBOARD, taskStack_KEYBOARD, &taskMem_KEYBOARD);

    // Start scheduler
    vTaskStartScheduler();

    // Trap program here just in case that scheduler is stopped.
    for(;;);
}
