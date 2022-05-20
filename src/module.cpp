/**********************************************************************\
 * Keyboard
 *
 * Board module detection
 **********************************************************************
 * Copyright (C) 2022 - Max Maisel
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
#include "module.h"
#include "pinout.h"

#include "dev/gpio.h"
#include "dev/rcc.h"

BYTE Module::m_module = 0;

void Module::detect() {
    using namespace pinout;

    dev::RCC->AHB1ENR |= dev::rcc::GPIOCEN;
    WORD nodeid = dev::GPIOC->IDR & (NODEID0 | NODEID1);
    if(nodeid == (NODEID0 | NODEID1))
        m_module = NAV;
    else if(nodeid == NODEID0)
        m_module = NUM;
    else if(nodeid == NODEID1)
        m_module = LEFT;
    else
        m_module = RIGHT;
}
