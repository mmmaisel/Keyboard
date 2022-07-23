/**********************************************************************\
 * Keyboard
 *
 * UART class
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
#include "priority.h"
#include "module.h"
#include "dev/core.h"
#include "dev/dma.h"
#include "dev/rcc.h"

#include "modular_keyboard.h"

#include <cstring>

Uart Uart1(1, &keyboard);
Uart Uart2(2, &keyboard);
Uart Uart6(6, &keyboard);

Uart::Uart(BYTE num, UartHandler* handler) :
    m_tx(0),
    m_rx(0),
    m_handler(handler)
{
    using namespace dev;
    using namespace dev::rcc;

    memset(m_tx_buffer, 0, BUFFER_SIZE);

    if(num == 1) {
        m_uart = USART1;
        m_dma = DMA2;
        m_rx_stream = 2;
        m_tx_stream = 7;

        // Enable used port clocks and configure pins PB6 and PB7.
        // This is identical for all modules.
        using namespace pinout::right;
        RCC->AHB1ENR |= GPIOBEN | DMA2EN;
        RCC->APB2ENR |= USART1EN;
        GPIOB->MODER |= MODE_UART1_TX | MODE_UART1_RX;
        GPIOB->AFRL |= AFRL_UART1_TX | AFRL_UART1_RX;

        {
            using namespace dev::dma;
            m_dma->STREAM[m_rx_stream].PAR = &m_uart->DR;
            m_dma->STREAM[m_rx_stream].CR = (4 << CHSEL_POS) | (1 << PL_POS) |
                (0 << MSIZE_POS) | (0 << PSIZE_POS) | MINC | DIR_P2M | TCIE;
            m_dma->STREAM[m_tx_stream].PAR = &m_uart->DR;
            m_dma->STREAM[m_tx_stream].CR = (4 << CHSEL_POS) | (1 << PL_POS) |
                (0 << MSIZE_POS) | (0 << PSIZE_POS) | MINC | DIR_M2P | TCIE;
            NVIC->enable_isr(isrnum::DMA2S2);
            NVIC->enable_isr(isrnum::DMA2S7);
            NVIC->set_priority(isrnum::DMA2S2, priority::UART);
            NVIC->set_priority(isrnum::DMA2S7, priority::UART);
        }
        {
            using namespace dev::usart;
            m_uart->CR1 &= CR1_MASK;
            m_uart->CR2 &= CR2_MASK;
            m_uart->CR3 &= CR3_MASK;
            m_uart->BRR = (m_uart->BRR & BRR_MASK) | (6 << DIV_MANT_POS) |
                (8 << DIV_FRAC_POS); // 460800 @ uart6
            m_uart->CR3 |= DMAT;
            m_uart->CR1 |= UE | TE | RE | RXNEIE;
            NVIC->enable_isr(isrnum::USART1);
            NVIC->set_priority(isrnum::USART1, priority::UART);
        }
    } else if(num == 2) {
        // UART2 only exists on right module.
        if(Module::get_id() != Module::RIGHT)
            return;

        m_uart = USART2;
        m_dma = DMA1;
        m_rx_stream = 5;
        m_tx_stream = 6;

        // Enable used port clocks and configure pins PA2 and PA3.
        using namespace pinout::right;
        RCC->AHB1ENR |= GPIOAEN | DMA1EN;
        RCC->APB1ENR |= USART2EN;
        GPIOA->MODER |= MODE_UART2_TX | MODE_UART2_RX;
        GPIOA->AFRL |= AFRL_UART2_TX | AFRL_UART2_RX;

        {
            using namespace dev::dma;
            m_dma->STREAM[m_rx_stream].PAR = &m_uart->DR;
            m_dma->STREAM[m_rx_stream].CR = (4 << CHSEL_POS) | (1 << PL_POS) |
                (0 << MSIZE_POS) | (0 << PSIZE_POS) | MINC | DIR_P2M | TCIE;
            m_dma->STREAM[m_tx_stream].PAR = &m_uart->DR;
            m_dma->STREAM[m_tx_stream].CR = (4 << CHSEL_POS) | (1 << PL_POS) |
                (0 << MSIZE_POS) | (0 << PSIZE_POS) | MINC | DIR_M2P | TCIE;
            NVIC->enable_isr(isrnum::DMA1S5);
            NVIC->enable_isr(isrnum::DMA1S6);
            NVIC->set_priority(isrnum::DMA1S5, priority::UART);
            NVIC->set_priority(isrnum::DMA1S6, priority::UART);
        }
        {
            using namespace dev::usart;
            m_uart->CR1 &= CR1_MASK;
            m_uart->CR2 &= CR2_MASK;
            m_uart->CR3 &= CR3_MASK;
            m_uart->BRR = (m_uart->BRR & BRR_MASK) | (6 << DIV_MANT_POS) |
                (8 << DIV_FRAC_POS); // 460800 @ uart6
            m_uart->CR3 |= DMAT;
            m_uart->CR1 |= UE | TE | RE | RXNEIE;
            NVIC->enable_isr(isrnum::USART2);
            NVIC->set_priority(isrnum::USART2, priority::UART);
        }
    } else if(num == 6) {
        // UART6 shared pins with LED matrix on left module.
        if(Module::get_id() == Module::LEFT)
            return;

        m_uart = USART6;
        m_dma = DMA2;
        m_rx_stream = 1;
        m_tx_stream = 6;

        // Enable used GPIO port clocks and configure pins.
        if(Module::get_id() == Module::RIGHT) {
            using namespace pinout::right;
            RCC->AHB1ENR |= GPIOCEN | DMA2EN;
            RCC->APB2ENR |= USART6EN;
            GPIOC->MODER |= MODE_UART6_TX | MODE_UART6_RX;
            GPIOC->AFRL |= AFRL_UART6_TX | AFRL_UART6_RX;
        } else {
            // This is identical on left, num and nav module.
            using namespace pinout::left;
            RCC->AHB1ENR |= GPIOAEN | DMA2EN;
            RCC->APB2ENR |= USART6EN;
            GPIOA->MODER |= MODE_UART6_TX | MODE_UART6_RX;
            GPIOA->AFRL |= AFRH_UART6_TX | AFRH_UART6_RX;
        }

        {
            using namespace dev::dma;
            m_dma->STREAM[m_rx_stream].PAR = &m_uart->DR;
            m_dma->STREAM[m_rx_stream].CR = (5 << CHSEL_POS) | (1 << PL_POS) |
                (0 << MSIZE_POS) | (0 << PSIZE_POS) | MINC | DIR_P2M | TCIE;
            m_dma->STREAM[m_tx_stream].PAR = &m_uart->DR;
            m_dma->STREAM[m_tx_stream].CR = (5 << CHSEL_POS) | (1 << PL_POS) |
                (0 << MSIZE_POS) | (0 << PSIZE_POS) | MINC | DIR_M2P | TCIE;
            NVIC->enable_isr(isrnum::DMA2S1);
            NVIC->enable_isr(isrnum::DMA2S6);
            NVIC->set_priority(isrnum::DMA2S1, priority::UART);
            NVIC->set_priority(isrnum::DMA2S6, priority::UART);
        }
        {
            using namespace dev::usart;
            m_uart->CR1 &= CR1_MASK;
            m_uart->CR2 &= CR2_MASK;
            m_uart->CR3 &= CR3_MASK;
            m_uart->BRR = (m_uart->BRR & BRR_MASK) | (6 << DIV_MANT_POS) |
                (8 << DIV_FRAC_POS); // 460800 @ uart6
            m_uart->CR3 |= DMAT;
            m_uart->CR1 |= UE | TE | RE | RXNEIE;
            NVIC->enable_isr(isrnum::USART6);
            NVIC->set_priority(isrnum::USART6, priority::UART);
        }
    } else {
        asm volatile(" svc 1");
    }
}

void Uart::write(BYTE data) {
    write(&data, 1);
}

void Uart::write(const BYTE* buffer, BYTE length) {
    using namespace dev;
    using namespace dev::dma;

    while(m_tx);
    m_tx = 1;
    if(length > BUFFER_SIZE)
        length = BUFFER_SIZE;
    memcpy(m_tx_buffer, buffer, length);

    m_dma->STREAM[m_tx_stream].M0AR = reinterpret_cast<WORD*>(m_tx_buffer);
    m_dma->STREAM[m_tx_stream].NDTR = length;
    m_dma->STREAM[m_tx_stream].CR |= EN;
}

void Uart::read(BYTE* buffer, BYTE length) {
    using namespace dev;
    using namespace dev::usart;

    m_rx = 1;
    m_uart->CR1 &= ~RXNEIE;
    m_uart->CR3 |= DMAR;
    m_dma->STREAM[m_rx_stream].M0AR = reinterpret_cast<WORD*>(buffer);
    m_dma->STREAM[m_rx_stream].NDTR = length;
    m_dma->STREAM[m_rx_stream].CR |= dma::EN;

    while(m_rx);
    m_uart->CR3 &= ~DMAR;
    m_uart->CR1 |= RXNEIE;
}

void Uart::ISR() {
    using namespace dev::usart;
    BYTE data = m_uart->DR;
    m_uart->SR = RXNE;
    m_handler->OnReceive(this, data);
}

void Uart::DMA_TX_ISR() {
    using namespace dev;
    using namespace dev::dma;
    m_dma->clear_isr(m_tx_stream);
    m_tx = 0;
}

void Uart::DMA_RX_ISR() {
    using namespace dev;
    using namespace dev::dma;
    m_dma->clear_isr(m_rx_stream);
    m_rx = 0;
}

extern "C" void uart1_vector() {
    Uart1.ISR();
}

extern "C" void uart2_vector(){
    Uart2.ISR();
}

extern "C" void uart6_vector(){
    Uart6.ISR();
}

extern "C" void dma1s5_vector(){
    Uart2.DMA_RX_ISR();
}

extern "C" void dma1s6_vector(){
    Uart2.DMA_TX_ISR();
}

extern "C" void dma2s1_vector(){
    Uart6.DMA_RX_ISR();
}

extern "C" void dma2s2_vector(){
    Uart1.DMA_RX_ISR();
}

extern "C" void dma2s6_vector(){
    Uart6.DMA_TX_ISR();
}

extern "C" void dma2s7_vector(){
    Uart1.DMA_TX_ISR();
}
