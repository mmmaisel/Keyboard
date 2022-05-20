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
#include "dev/core.h"
#include "dev/dma.h"
#include "dev/rcc.h"

#include <cstring>

Uart Uart1(1);
Uart Uart2(2);
Uart Uart6(6);

// XXX: only valid for master module
// XXX: may it work and initialize HW that is not routed to pins?
Uart::Uart(BYTE num, UartHandler* handler) :
    m_tx(0),
    m_rx(0),
    m_handler(handler)
{
    using namespace dev;
    using namespace dev::rcc;
    using namespace pinout;

    if(num == 1) {
        m_uart = USART1;
        m_dma = DMA2;
        m_rx_stream = 2;
        m_tx_stream = 7;

        // Enable used port clocks
        RCC->AHB1ENR |= GPIOBEN | DMA2EN;
        RCC->APB2ENR |= USART1EN;

        {
            using namespace dev::dma;
            m_dma->STREAM[m_rx_stream].PAR = (WORD)&m_uart->DR;
            m_dma->STREAM[m_rx_stream].CR = (4 << CHSEL_POS) | (1 << PL_POS) |
                (0 << MSIZE_POS) | (0 << PSIZE_POS) | MINC | DIR_P2M | TCIE;
            m_dma->STREAM[m_tx_stream].PAR = (WORD)&m_uart->DR;
            m_dma->STREAM[m_tx_stream].CR = (4 << CHSEL_POS) | (1 << PL_POS) |
                (0 << MSIZE_POS) | (0 << PSIZE_POS) | MINC | DIR_M2P | TCIE;
            NVIC->enable_isr(isrnum::DMA2S2);
            NVIC->enable_isr(isrnum::DMA2S7);
            NVIC->PRI[isrnum::DMA2S2] = 0x09;
            NVIC->PRI[isrnum::DMA2S7] = 0x09;
        }

        // Configure Pins PB6, PB7
        GPIOB->MODER |= MODE_UART1_TX | MODE_UART1_RX;
        GPIOB->AFRL |= AFRL_UART1_TX | AFRL_UART1_RX;

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
            NVIC->PRI[isrnum::USART1] = 0x09;
        }
    } else if(num == 2) {
        m_uart = USART2;
        m_dma = DMA1;
        m_rx_stream = 5;
        m_tx_stream = 6;

        // Enable used port clocks
        RCC->AHB1ENR |= GPIOAEN | DMA1EN;
        RCC->APB1ENR |= USART2EN;

        {
            using namespace dev::dma;
            m_dma->STREAM[m_rx_stream].PAR = (WORD)&m_uart->DR;
            m_dma->STREAM[m_rx_stream].CR = (4 << CHSEL_POS) | (1 << PL_POS) |
                (0 << MSIZE_POS) | (0 << PSIZE_POS) | MINC | DIR_P2M | TCIE;
            m_dma->STREAM[m_tx_stream].PAR = (WORD)&m_uart->DR;
            m_dma->STREAM[m_tx_stream].CR = (4 << CHSEL_POS) | (1 << PL_POS) |
                (0 << MSIZE_POS) | (0 << PSIZE_POS) | MINC | DIR_M2P | TCIE;
            NVIC->enable_isr(isrnum::DMA1S5);
            NVIC->enable_isr(isrnum::DMA1S6);
            NVIC->PRI[isrnum::DMA1S5] = 0x09;
            NVIC->PRI[isrnum::DMA1S6] = 0x09;
        }

        // Configure Pins PA2, PA3
        GPIOA->MODER |= MODE_UART2_TX | MODE_UART2_RX;
        GPIOA->AFRL |= AFRL_UART2_TX | AFRL_UART2_RX;

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
            NVIC->PRI[isrnum::USART2] = 0x09;
        }
    } else if(num == 6) {
        m_uart = USART6;
        m_dma = DMA2;
        m_rx_stream = 1;
        m_tx_stream = 6;

        // Enable used GPIO port clocks
        RCC->AHB1ENR |= GPIOCEN | DMA2EN;
        RCC->APB2ENR |= USART6EN;

        {
            using namespace dev::dma;
            m_dma->STREAM[m_rx_stream].PAR = (WORD)&m_uart->DR;
            m_dma->STREAM[m_rx_stream].CR = (5 << CHSEL_POS) | (1 << PL_POS) |
                (0 << MSIZE_POS) | (0 << PSIZE_POS) | MINC | DIR_P2M | TCIE;
            m_dma->STREAM[m_tx_stream].PAR = (WORD)&m_uart->DR;
            m_dma->STREAM[m_tx_stream].CR = (5 << CHSEL_POS) | (1 << PL_POS) |
                (0 << MSIZE_POS) | (0 << PSIZE_POS) | MINC | DIR_M2P | TCIE;
            NVIC->enable_isr(isrnum::DMA2S1);
            NVIC->enable_isr(isrnum::DMA2S6);
            NVIC->PRI[isrnum::DMA2S1] = 0x09;
            NVIC->PRI[isrnum::DMA2S6] = 0x09;
        }

        // Configure Pins PC6, PC7
        GPIOC->MODER |= MODE_UART6_TX | MODE_UART6_RX;
        GPIOC->AFRL |= AFRL_UART6_TX | AFRL_UART6_RX;

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
            NVIC->PRI[isrnum::USART6] = 0x09;
        }
    } else {
        asm volatile(" svc 1");
    }

}

void Uart::write(BYTE* buffer, BYTE length) {
    using namespace dev;
    using namespace dev::dma;

    while(m_tx);
    m_tx = 1;
    if(length > BUFFER_SIZE)
        length = BUFFER_SIZE;
    memcpy(m_tx_buffer, buffer, length);

    m_dma->STREAM[m_tx_stream].M0AR = (WORD)m_tx_buffer;
    m_dma->STREAM[m_tx_stream].NDTR = length;
    m_dma->STREAM[m_tx_stream].CR |= EN;
}

BYTE Uart::read() {
    using namespace dev;
    while(m_rx_buffer.size() == 0);

    NVIC->disable_isr(isrnum::USART6);
    BYTE data = m_rx_buffer.pop();
    NVIC->enable_isr(isrnum::USART6);
    return data;
}

void Uart::read(BYTE* buffer, BYTE length) {
    using namespace dev;
    using namespace dev::usart;

    m_rx = 1;
    m_uart->CR1 &= ~RXNEIE;
    m_uart->CR3 |= DMAR;
    m_dma->STREAM[m_rx_stream].M0AR = (WORD)buffer;
    m_dma->STREAM[m_rx_stream].NDTR = length;
    m_dma->STREAM[m_rx_stream].CR |= dma::EN;

    while(m_rx);
    m_uart->CR3 &= ~DMAR;
    m_uart->CR1 |= RXNEIE;
}

void Uart::ISR() {
    using namespace dev::usart;
    m_rx_buffer.push(m_uart->DR);
    m_uart->SR = RXNE;
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
