/**********************************************************************\
 * Keyboard
 *
 * System devices and error handlers
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
#include <stdint.h>

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#include "FreeRTOS/queue.h"

#include "dev/gpio.h"
#include "dev/rcc.h"
#include "pinout.h"

extern void* __heap_start__;

void sleep_ms(ULONG ms) {
    volatile ULONG cycles = 48000 * ms / 4; // Sysclock: 48 MHz
    asm (
        "1:\n"
        "  subs %0, %0, #1\n" // 1 cycle
        "  nop\n"             // 1 cycle
        "  bne 1b\n"          // 1 + P cycles (here: pipeline == 1)
        : : "r"(cycles) :
    );
}

/// @internal
/// A function which is called after main() and handles something
/// with destruction of global objects.
///
/// As main() never exits here this function is a stub.
/// cppcheck-suppress[unusedFunction]
extern "C" void __dso_handle() {
}

/// @internal
/// A deprecated function which is called after main() and handles
/// destruction of modules.
///
/// As main() never exits here this function is a stub.
/// cppcheck-suppress[unusedFunction]
extern "C" void _fini() {
}

/// @internal
/// Increases heap size, used by malloc / new.
/// Since there is no dynamic memory in this program this function is a stub.
/// cppcheck-suppress[unusedFunction]
extern "C" void* _sbrk(int incr) {
    return __heap_start__;
}

/**********************************************************************\
 * FreeRTOS
\**********************************************************************/

/// @internal
/// FreeRTOS assertion failed hook
extern "C" void vAssertCalled(const char* file, const char* line)
{
    for(;;);
}

StaticTask_t xIdleTaskTCB;
StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

extern "C" void vApplicationGetIdleTaskMemory
(
    StaticTask_t **ppxIdleTaskTCBBuffer,
    StackType_t **ppxIdleTaskStackBuffer,
    uint32_t *pulIdleTaskStackSize
)
{
    // Pass out a pointer to the StaticTask_t structure in
    // which the Idle task's state will be stored.
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    // Pass out the array that will be used as the Idle task's stack.
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    // Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    // Note that, as the array is necessarily of type StackType_t,
    // configMINIMAL_STACK_SIZE is specified in words, not bytes.
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/**********************************************************************\
 * Fatal software errors
\**********************************************************************/

void error_handler() {
    // TODO: signal error somehow
    for(;;);
}

/// @internal
/// Pure virtual function call error hook
/// cppcheck-suppress[unusedFunction]
extern "C" void __cxa_pure_virtual() {
    error_handler();
}

/**********************************************************************\
 * Error interrupts
\**********************************************************************/

extern "C" void fault_vector() __attribute__((naked));
extern "C" void fault_vector() {
    error_handler();
}

extern "C" void nmi_vector() __attribute__((naked));
extern "C" void nmi_vector() {
    error_handler();
}

extern "C" void default_vector() __attribute__((naked));
extern "C" void default_vector() {
    error_handler();
}

