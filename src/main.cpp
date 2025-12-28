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

#include "effect.h"
#include "key_matrix.h"
#include "led_matrix.h"
#include "module.h"
#include "pinout.h"
#include "priority.h"
#include "uart_receiver.h"
#include "usb_phy.h"
#include "wiring.h"

const int STACK_SIZE_KEYBOARD = 0x100;
const int STACK_SIZE_UARTRX = 0x100;

// Task memory
StaticTask_t task_mem_keyboard;
StackType_t  task_stack_keyboard[STACK_SIZE_KEYBOARD];
StaticTask_t task_mem_uartrx1;
StackType_t  task_stack_uartrx1[STACK_SIZE_UARTRX];

// XXX: led control usb protocol

/// C++ main function, program starts here.
/// Please note that the real program starts in startup.cpp
[[noreturn]] int main() {
    Wiring wiring(Module::get_id());

    EventDispatcher::initialize();
    EffectController::initialize(wiring.key_config->page, wiring.led_config);

    EffectController::set_effect(&effect_rainbow);
    //EffectController::set_effect(&effect_running);
    //EffectController::set_effect(&effect_flash);

    wiring.led_matrix_hw_init();
    LedMatrix::initialize(wiring.led_config);

    wiring.key_matrix_hw_init();
    KeyMatrix::initialize(wiring.key_config);

    if(Module::get_id() == Module::RIGHT) {
        USBPhy::Initialize();

        using namespace pinout::right;
        dev::GPIOB->MODER |= MODE_PWREN0 | MODE_PWREN1 | MODE_PWREN2;
        dev::GPIOB->PUPDR |= PUPD_PWREN0 | PUPD_PWREN1 | PUPD_PWREN2;
        dev::GPIOB->set_odr(PWREN0 | PWREN1 | PWREN2);
    }

    xTaskCreateStatic(
        &EventSink::task,
        "KEYBOARD",
        STACK_SIZE_KEYBOARD,
        wiring.keyboard,
        priority::KEYBOARD,
        task_stack_keyboard,
        &task_mem_keyboard
    );
    xTaskCreateStatic(
        &UartReceiver::task_trampoline,
        "UARTRX1",
        STACK_SIZE_UARTRX,
        &uart1_receiver,
        priority::UARTRX,
        task_stack_uartrx1,
        &task_mem_uartrx1
    );

    // Start scheduler
    vTaskStartScheduler();

    // Trap program here just in case that scheduler is stopped.
    for(;;);
}
