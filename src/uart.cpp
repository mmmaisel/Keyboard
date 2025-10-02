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

#include <cstring>

UartMessage::UartMessage(BYTE id, BYTE dma, BYTE data) :
    m_type_id(id | (dma ? DMA_COMPLETE : 0)),
    m_data(data)
{
}

Uart Uart1(Uart::UART1_ID);
Uart Uart2(Uart::UART2_ID);
Uart Uart6(Uart::UART6_ID);

Uart* const Uart::BY_ID[] = { &Uart1, &Uart2, &Uart6 };

Uart::Uart(BYTE id) :
    m_id(id)
{
    using namespace dev;
    using namespace dev::rcc;

    memset(m_tx_buffer, 0, BUFFER_SIZE);
    m_rx_semaphore = xSemaphoreCreateBinaryStatic(&m_rx_semaphore_mem);
    xSemaphoreGive(m_rx_semaphore);
    m_tx_semaphore = xSemaphoreCreateBinaryStatic(&m_tx_semaphore_mem);
    xSemaphoreGive(m_tx_semaphore);

    if(id == UART1_ID) {
        m_uart = USART1;
        m_dma = DMA2;
        m_rx_stream = 2;
        m_tx_stream = 7;

#ifdef DEBUG
        vQueueAddToRegistry(m_rx_semaphore, "UART1RX");
        vQueueAddToRegistry(m_tx_semaphore, "UART1TX");
#endif

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
            m_uart->CR1 |= UE | TE | RE;
            NVIC->enable_isr(isrnum::USART1);
            NVIC->set_priority(isrnum::USART1, priority::UART);
        }
    } else if(id == UART2_ID) {
        // UART2 only exists on right module.
        if(Module::get_id() != Module::RIGHT)
            return;

        m_uart = USART2;
        m_dma = DMA1;
        m_rx_stream = 5;
        m_tx_stream = 6;

#ifdef DEBUG
        vQueueAddToRegistry(m_rx_semaphore, "UART2RX");
        vQueueAddToRegistry(m_tx_semaphore, "UART2TX");
#endif

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
            m_uart->CR1 |= UE | TE | RE;
            NVIC->enable_isr(isrnum::USART2);
            NVIC->set_priority(isrnum::USART2, priority::UART);
        }
    } else if(id == UART6_ID) {
        // UART6 shared pins with LED matrix on left module.
        if(Module::get_id() == Module::LEFT)
            return;

        m_uart = USART6;
        m_dma = DMA2;
        m_rx_stream = 1;
        m_tx_stream = 6;

#ifdef DEBUG
        vQueueAddToRegistry(m_rx_semaphore, "UART6RX");
        vQueueAddToRegistry(m_tx_semaphore, "UART6TX");
#endif

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
            m_uart->CR1 |= UE | TE | RE;
            NVIC->enable_isr(isrnum::USART6);
            NVIC->set_priority(isrnum::USART6, priority::UART);
        }
    }
}

void Uart::write(BYTE data) {
    write(&data, 1);
}

void Uart::write(const BYTE* buffer, BYTE length) {
    using namespace dev;
    using namespace dev::dma;

    if(xSemaphoreTake(m_tx_semaphore, portMAX_DELAY) != pdTRUE)
        for(;;);

    if(length > BUFFER_SIZE)
        length = BUFFER_SIZE;
    memcpy(m_tx_buffer, buffer, length);

    m_dma->STREAM[m_tx_stream].M0AR = reinterpret_cast<WORD*>(m_tx_buffer);
    m_dma->STREAM[m_tx_stream].NDTR = length;
    m_dma->STREAM[m_tx_stream].CR |= EN;
}

void Uart::start_read_byte() {
    m_uart->CR1 |= dev::usart::RXNEIE;
}

void Uart::abort_read() {
    using namespace dev;
    m_dma->STREAM[m_rx_stream].CR &= ~dma::EN;
}

void Uart::start_read(BYTE* buffer, BYTE length) {
    using namespace dev;
    using namespace dev::usart;

    if(xSemaphoreTake(m_rx_semaphore, portMAX_DELAY) != pdTRUE)
        for(;;);

    m_uart->CR1 &= ~RXNEIE;
    m_uart->CR3 |= DMAR;
    m_dma->STREAM[m_rx_stream].M0AR = reinterpret_cast<WORD*>(buffer);
    m_dma->STREAM[m_rx_stream].NDTR = length;
    m_dma->STREAM[m_rx_stream].CR |= dma::EN;
}

void Uart::ISR() {
    using namespace dev::usart;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    UartMessage msg(m_id, 0, m_uart->DR);
    volatile WORD _dummy = m_uart->SR;
    m_uart->CR1 &= ~RXNEIE;

    if(m_rx_queue == 0)
        return;

    xQueueSendFromISR(m_rx_queue, &msg, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void Uart::DMA_TX_ISR() {
    using namespace dev;
    using namespace dev::dma;
    m_dma->clear_isr(m_tx_stream);

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(m_tx_semaphore, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void Uart::DMA_RX_ISR() {
    using namespace dev;
    using namespace dev::dma;
    using namespace dev::usart;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    UartMessage msg(m_id, 1, 0);

    m_dma->clear_isr(m_rx_stream);
    m_uart->CR3 &= ~DMAR;

    xSemaphoreGiveFromISR(m_rx_semaphore, &xHigherPriorityTaskWoken);

    if(m_rx_queue != 0) {
        BaseType_t xHigherPriorityTaskWoken2 = pdFALSE;
        xQueueSendFromISR(m_rx_queue, &msg, &xHigherPriorityTaskWoken2);
        xHigherPriorityTaskWoken |= xHigherPriorityTaskWoken2;
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
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
