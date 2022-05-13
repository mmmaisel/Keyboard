/**********************************************************************\
 * Keyboard
 *
 * debug write only UART class
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
#include "uart.h"

#include "pinout.h"
#include "dev/rcc.h"

SimpleUart UART6(dev::USART6);

SimpleUart::SimpleUart(volatile dev::UsartStruct* uart)
    : m_uart(uart)
{
    using namespace dev;
    using namespace dev::rcc;
    using namespace dev::usart;
    using namespace pinout;

    // Enable used GPIO port clocks
    // XXX: evaluate which uart to use
    // XXX: this is for UART6
    RCC->AHB1ENR |= GPIOCEN;
    RCC->APB2ENR |= USART6EN;

    // Configure Pins PA2, PA3
    GPIOC->MODER |= MODE_UART6_TX | MODE_UART6_RX;
    GPIOC->AFRL |= AFRL_UART6_TX | AFRL_UART6_RX;

    m_uart->CR1 &= CR1_MASK;
    m_uart->CR2 &= CR2_MASK;
    m_uart->CR3 &= CR3_MASK;
    m_uart->BRR = (m_uart->BRR & BRR_MASK) | (6 << DIV_MANT_POS) | (8 << DIV_FRAC_POS); // 460800 @ uart6
    m_uart->CR1 |= UE | TE; // | RE;
}
