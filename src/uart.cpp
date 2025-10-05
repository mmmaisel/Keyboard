/******************************************************************************\
    Split Keyboard
    Copyright (C) 2022-2025 - Max Maisel

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
#include "uart.h"

#include "pinout.h"
#include "priority.h"
#include "module.h"
#include "dev/core.h"
#include "dev/dma.h"
#include "dev/rcc.h"

#include <cstring>

BYTE uart1_hw_init() {
    using namespace dev;
    using namespace dev::rcc;

    // Enable used port clocks and configure pins PB6 and PB7.
    // This is identical for all modules.
    using namespace pinout::right;
    RCC->AHB1ENR |= GPIOBEN | DMA2EN;
    RCC->APB2ENR |= USART1EN;
    GPIOB->MODER |= MODE_UART1_TX | MODE_UART1_RX;
    GPIOB->AFRL |= AFRL_UART1_TX | AFRL_UART1_RX;

    NVIC->enable_isr(isrnum::DMA2S2);
    NVIC->enable_isr(isrnum::DMA2S7);
    NVIC->set_priority(isrnum::DMA2S2, priority::UART);
    NVIC->set_priority(isrnum::DMA2S7, priority::UART);

    NVIC->enable_isr(isrnum::USART1);
    NVIC->set_priority(isrnum::USART1, priority::UART);

    return 0;
}

UartConfig uart1_config = {
    .uart = dev::USART1,
    .dma = dev::DMA2,
    .rx_stream = 2,
    .tx_stream = 7,
    .hw_init_fn = uart1_hw_init,
#ifdef DEBUG
    .rx_queue_name = "UART1RX",
    .tx_queue_name = "UART1TX",
#endif
};

BYTE uart2_hw_init() {
    using namespace dev;
    using namespace dev::rcc;

    // UART2 only exists on right module.
    if(Module::get_id() != Module::RIGHT)
        return 1;

    // Enable used port clocks and configure pins PA2 and PA3.
    using namespace pinout::right;
    RCC->AHB1ENR |= GPIOAEN | DMA1EN;
    RCC->APB1ENR |= USART2EN;
    GPIOA->MODER |= MODE_UART2_TX | MODE_UART2_RX;
    GPIOA->AFRL |= AFRL_UART2_TX | AFRL_UART2_RX;

    NVIC->enable_isr(isrnum::DMA1S5);
    NVIC->enable_isr(isrnum::DMA1S6);
    NVIC->set_priority(isrnum::DMA1S5, priority::UART);
    NVIC->set_priority(isrnum::DMA1S6, priority::UART);

    NVIC->enable_isr(isrnum::USART2);
    NVIC->set_priority(isrnum::USART2, priority::UART);

    return 0;
}

UartConfig uart2_config = {
    .uart = dev::USART2,
    .dma = dev::DMA1,
    .rx_stream = 5,
    .tx_stream = 6,
    .hw_init_fn = uart2_hw_init,
#ifdef DEBUG
    .rx_queue_name = "UART2RX",
    .tx_queue_name = "UART2TX",
#endif
};

BYTE uart6_hw_init() {
    using namespace dev;
    using namespace dev::rcc;

    // UART6 shared pins with LED matrix on left module.
    if(Module::get_id() == Module::LEFT)
        return 1;

    // Enable used GPIO port clocks and configure pins.
    if(Module::get_id() == Module::RIGHT) {
        using namespace pinout::right;
        RCC->AHB1ENR |= GPIOCEN | DMA2EN;
        RCC->APB2ENR |= USART6EN;
        GPIOC->MODER |= MODE_UART6_TX | MODE_UART6_RX;
        GPIOC->AFRL |= AFRL_UART6_TX | AFRL_UART6_RX;
    } else {
        // This is identical on num and nav module.
        using namespace pinout::left;
        RCC->AHB1ENR |= GPIOAEN | DMA2EN;
        RCC->APB2ENR |= USART6EN;
        GPIOA->MODER |= MODE_UART6_TX | MODE_UART6_RX;
        GPIOA->AFRL |= AFRH_UART6_TX | AFRH_UART6_RX;
    }

    NVIC->enable_isr(isrnum::DMA2S1);
    NVIC->enable_isr(isrnum::DMA2S6);
    NVIC->set_priority(isrnum::DMA2S1, priority::UART);
    NVIC->set_priority(isrnum::DMA2S6, priority::UART);

    NVIC->enable_isr(isrnum::USART6);
    NVIC->set_priority(isrnum::USART6, priority::UART);

    return 0;
}

UartConfig uart6_config = {
    .uart = dev::USART6,
    .dma = dev::DMA2,
    .rx_stream = 1,
    .tx_stream = 6,
    .hw_init_fn = uart6_hw_init,
#ifdef DEBUG
    .rx_queue_name = "UART6RX",
    .tx_queue_name = "UART6TX",
#endif
};

Uart uart1(&uart1_config);
Uart uart2(&uart2_config);
Uart uart6(&uart6_config);

Uart::Uart(UartConfig* config) :
    _config(config)
{
    _rx_semaphore = xSemaphoreCreateBinaryStatic(&_rx_semaphore_mem);
    xSemaphoreGive(_rx_semaphore);
    _tx_semaphore = xSemaphoreCreateBinaryStatic(&_tx_semaphore_mem);
    xSemaphoreGive(_tx_semaphore);

#ifdef DEBUG
    vQueueAddToRegistry(_rx_semaphore, config->rx_queue_name);
    vQueueAddToRegistry(_tx_semaphore, config->tx_queue_name);
#endif

    if(_config->hw_init_fn())
        return;

    {
        using namespace dev::dma;
        _config->dma->STREAM[_config->rx_stream].PAR = &_config->uart->DR;
        _config->dma->STREAM[_config->rx_stream].CR =
            (4 << CHSEL_POS) | (1 << PL_POS) | (0 << MSIZE_POS) |
            (0 << PSIZE_POS) | MINC | DIR_P2M | TCIE;
        _config->dma->STREAM[_config->tx_stream].PAR = &_config->uart->DR;
        _config->dma->STREAM[_config->tx_stream].CR =
            (4 << CHSEL_POS) |(1 << PL_POS) | (0 << MSIZE_POS) |
            (0 << PSIZE_POS) | MINC | DIR_M2P | TCIE;
    }
    {
        using namespace dev::usart;
        _config->uart->CR1 &= CR1_MASK;
        _config->uart->CR2 &= CR2_MASK;
        _config->uart->CR3 &= CR3_MASK;
        _config->uart->BRR =
            (_config->uart->BRR & BRR_MASK) | (6 << DIV_MANT_POS) |
            (8 << DIV_FRAC_POS); // 460800 @ uart6
        // TODO: use 240k baud
        // TODO: enable idle isr
        _config->uart->CR3 |= DMAT;
        _config->uart->CR1 |= UE | TE | RE;
    }
}

void Uart::debug_write(BYTE data) {
    // TODO: write without DMA to FIFO
    //write(&data, 1);
}

void Uart::write(const BYTE* buffer, BYTE length) {
    using namespace dev;
    using namespace dev::dma;

    _config->dma->STREAM[_config->tx_stream].M0AR =
        const_cast<WORD*>(reinterpret_cast<const WORD*>(buffer));
    _config->dma->STREAM[_config->tx_stream].NDTR = length;
    _config->dma->STREAM[_config->tx_stream].CR |= EN;

    // TODO: sync with ISR here
}

/*void Uart::start_read_byte() {
    m_uart->CR1 |= dev::usart::RXNEIE;
}

void Uart::abort_read() {
    using namespace dev;
    m_dma->STREAM[m_rx_stream].CR &= ~dma::EN;
}*/

// TODO: https://github.com/MaJerle/stm32-usart-uart-dma-rx-tx
// TODO: needs IDLE interrupt to detect end of frame on RX side

BYTE Uart::read(BYTE* buffer, BYTE length) {
    using namespace dev;
    using namespace dev::usart;

    _config->uart->CR1 &= ~RXNEIE;
    _config->uart->CR3 |= DMAR;
    _config->dma->STREAM[_config->rx_stream].M0AR = reinterpret_cast<WORD*>(buffer);
    _config->dma->STREAM[_config->rx_stream].NDTR = length;
    _config->dma->STREAM[_config->rx_stream].CR |= dma::EN;

    // TODO: enable IDLE ISR, read until IDLE or buffer full

    // TODO: sync with ISR here
    return 0; // len
}

void Uart::ISR() {
    // TODO
    /*using namespace dev::usart;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    UartMessage msg(m_id, 0, m_uart->DR);
    volatile WORD _dummy = m_uart->SR;
    m_uart->CR1 &= ~RXNEIE;

    if(m_rx_queue == 0)
        return;

    xQueueSendFromISR(m_rx_queue, &msg, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);*/
}

void Uart::DMA_TX_ISR() {
    using namespace dev;
    using namespace dev::dma;
    _config->dma->clear_isr(_config->tx_stream);

    BaseType_t task_woken = pdFALSE;
    xSemaphoreGiveFromISR(_tx_semaphore, &task_woken);
    portYIELD_FROM_ISR(task_woken);
}

void Uart::DMA_RX_ISR() {
    using namespace dev;
    using namespace dev::dma;
    using namespace dev::usart;

    _config->dma->clear_isr(_config->rx_stream);
    _config->uart->CR3 &= ~DMAR;

    BaseType_t task_woken = pdFALSE;
    xSemaphoreGiveFromISR(_rx_semaphore, &task_woken);
    portYIELD_FROM_ISR(task_woken);
}

extern "C" void uart1_vector() {
    uart1.ISR();
}

extern "C" void uart2_vector() {
    uart2.ISR();
}

extern "C" void uart6_vector() {
    uart6.ISR();
}

extern "C" void dma1s5_vector() {
    uart2.DMA_RX_ISR();
}

extern "C" void dma1s6_vector() {
    uart2.DMA_TX_ISR();
}

extern "C" void dma2s1_vector() {
    uart6.DMA_RX_ISR();
}

extern "C" void dma2s2_vector() {
    uart1.DMA_RX_ISR();
}

extern "C" void dma2s6_vector() {
    uart6.DMA_TX_ISR();
}

extern "C" void dma2s7_vector() {
    uart1.DMA_TX_ISR();
}
