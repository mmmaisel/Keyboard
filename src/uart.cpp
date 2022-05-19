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

// UART1: DMA2, CH4, Stream 2,7
// UART2: DMA1, CH4, Stream 5,6
// UART6: DMA2, CH5, Stream 1,6

Uart Uart6(dev::USART6);

Uart::Uart(volatile dev::UsartStruct* uart) :
    m_uart(uart),
    m_tx(0),
    m_rx(0)
{
    using namespace dev;
    using namespace dev::rcc;
    using namespace pinout;

    // Enable used GPIO port clocks
    // XXX: evaluate which uart to use
    // XXX: this is for UART6
    RCC->AHB1ENR |= GPIOCEN | DMA1EN | DMA2EN;
    RCC->APB2ENR |= USART6EN;

    {
        using namespace dev::dma;
        // RX stream
        DMA2->STREAM[1].PAR = (WORD)&m_uart->DR;
        DMA2->STREAM[1].CR = (5 << CHSEL_POS) | (1 << PL_POS) |
            (0 << MSIZE_POS) | (0 << PSIZE_POS) | MINC | DIR_P2M | TCIE;
        // TX stream
        DMA2->STREAM[6].PAR = (WORD)&m_uart->DR;
        DMA2->STREAM[6].CR = (5 << CHSEL_POS) | (1 << PL_POS) |
            (0 << MSIZE_POS) | (0 << PSIZE_POS) | MINC | DIR_M2P | TCIE;
        NVIC->enable_isr(isrnum::DMA2S1);
        NVIC->enable_isr(isrnum::DMA2S6);
        NVIC->PRI[isrnum::DMA2S1] = 0x09;
        NVIC->PRI[isrnum::DMA2S6] = 0x09;
    }

    // Configure Pins PA2, PA3
    GPIOC->MODER |= MODE_UART6_TX | MODE_UART6_RX;
    GPIOC->AFRL |= AFRL_UART6_TX | AFRL_UART6_RX;

    {
        using namespace dev::usart;
        m_uart->CR1 &= CR1_MASK;
        m_uart->CR2 &= CR2_MASK;
        m_uart->CR3 &= CR3_MASK;
        m_uart->BRR = (m_uart->BRR & BRR_MASK) | (6 << DIV_MANT_POS) | (8 << DIV_FRAC_POS); // 460800 @ uart6
        m_uart->CR3 |= DMAT;// | DMAR;
        m_uart->CR1 |= UE | TE | RE | RXNEIE;
        NVIC->enable_isr(isrnum::USART6);
        NVIC->PRI[isrnum::USART6] = 0x09;
    }
}

void Uart::write(BYTE* buffer, BYTE length) {
    // XXX: this is only valid for UART6
    using namespace dev;
    using namespace dev::dma;

    while(m_tx);
    m_tx = 1;
    if(length > BUFFER_SIZE)
        length = BUFFER_SIZE;
    memcpy(m_tx_buffer, buffer, length);

    DMA2->STREAM[6].M0AR = (WORD)m_tx_buffer;
    DMA2->STREAM[6].NDTR = length;
    DMA2->STREAM[6].CR |= EN;
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
    using namespace dev::dma;

    m_rx = 1;
    m_uart->CR1 &= ~usart::RXNEIE;
    m_uart->CR3 |= usart::DMAR;
    DMA2->STREAM[1].M0AR = (WORD)buffer;
    DMA2->STREAM[1].NDTR = length;
    DMA2->STREAM[1].CR |= EN;

    while(m_rx);
    m_uart->CR3 &= ~usart::DMAR;
    m_uart->CR1 |= usart::RXNEIE;
}

void Uart::ISR() {
    using namespace dev::usart;
    m_rx_buffer.push(m_uart->DR);
    m_uart->SR = RXNE;
}

void Uart::DMA_TX_ISR() {
    using namespace dev;
    using namespace dev::dma;
    DMA2->HIFCR = (TCIF << STREAM6_POS);
    m_tx = 0;
}

void Uart::DMA_RX_ISR() {
    using namespace dev;
    using namespace dev::dma;
    // XXX: add helper to clear that stream
    DMA2->LIFCR = (TCIF << STREAM1_POS);
    m_rx = 0;
}

extern "C" void uart1_vector() {
    //Uart1.ISR();
}

extern "C" void uart2_vector(){
    //Uart2.ISR();
}

extern "C" void uart6_vector(){
    Uart6.ISR();
}

extern "C" void dma1s5_vector(){
}

extern "C" void dma1s6_vector(){
}

extern "C" void dma2s1_vector(){
    Uart6.DMA_RX_ISR();
}

extern "C" void dma2s2_vector(){
}

extern "C" void dma2s6_vector(){
    Uart6.DMA_TX_ISR();
}

extern "C" void dma2s7_vector(){
}
