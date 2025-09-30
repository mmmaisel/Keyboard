/******************************************************************************\
    Split Keyboard
    Copyright (C) 2019-2025 - Max Maisel

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

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"

#include "dev/gpio.h"
#include "key_matrix.h"
#include "led_matrix.h"
#include "modular_keyboard.h"
#include "module.h"
#include "pinout.h"
#include "priority.h"
#include "system.h"
#include "uart.h"
#include "uart_protocol.h"
#include "usb_phy.h"
#include "wiring.h"

const int STACK_SIZE_KEYBOARD = 0x100;
const int STACK_SIZE_UARTPROTO = 0x100;

// Task memory
StaticTask_t taskMem_KEYBOARD;
StackType_t  taskStack_KEYBOARD[STACK_SIZE_KEYBOARD];
StaticTask_t taskMem_UARTPROTO;
StackType_t  taskStack_UARTPROTO[STACK_SIZE_UARTPROTO];

// XXX: led control usb protocol
// XXX: control LEDs via keys + FN

/// C++ main function, program starts here.
/// Please note that the real program starts in startup.cpp
[[noreturn]] void main() {
    Wiring wiring(Module::get_id());

    ModularKeyboard::initialize();
    UartProtocol::initialize();
    LedMatrix::initialize();

    wiring.key_matrix_hw_init();
    KeyMatrix::initialize(wiring.key_config, ModularKeyboard::get_queue());

    if(Module::get_id() == Module::RIGHT) {
        USBPhy::Initialize();

        using namespace pinout::right;
        dev::GPIOB->MODER |= MODE_PWREN0 | MODE_PWREN1 | MODE_PWREN2;
        dev::GPIOB->PUPDR |= PUPD_PWREN0 | PUPD_PWREN1 | PUPD_PWREN2;
        dev::GPIOB->set_odr(PWREN0 | PWREN1 | PWREN2);
    }

    xTaskCreateStatic(&ModularKeyboard::task, "KEYBOARD", STACK_SIZE_KEYBOARD, 0,
        priority::KEYBOARD, taskStack_KEYBOARD, &taskMem_KEYBOARD);
    xTaskCreateStatic(&UartProtocol::task, "UARTPROTO", STACK_SIZE_UARTPROTO, 0,
        priority::UARTPROTO, taskStack_UARTPROTO, &taskMem_UARTPROTO);

    // Start scheduler
    vTaskStartScheduler();

    // Trap program here just in case that scheduler is stopped.
    for(;;);
}
