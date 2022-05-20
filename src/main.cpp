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
#include <string.h>
#include <stdlib.h>

#include "module.h"
#include "led_matrix.h"
#include "key_matrix.h"
#include "system.h"
#include "usb_phy.h"

#include "uart.h"

/// C++ main function, program starts here.
void main() __attribute__((noreturn));
void main() {
    LedMatrix::initialize();
    KeyMatrix::initialize();
    if(Module::get_id() == Module::RIGHT)
        USBPhy::Initialize();

    for(;;);
}
