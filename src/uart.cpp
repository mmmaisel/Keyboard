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
    .dma_ch = 4,
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
    .dma_ch = 4,
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

    // UART6 shares pins with LED matrix on left module.
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
    .dma_ch = 5,
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
    _tx_semaphore = xSemaphoreCreateBinaryStatic(&_tx_semaphore_mem);

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
            (_config->dma_ch << CHSEL_POS) | (1 << PL_POS) | (0 << MSIZE_POS) |
            (0 << PSIZE_POS) | MINC | DIR_P2M | TCIE;
        _config->dma->STREAM[_config->tx_stream].PAR = &_config->uart->DR;
        _config->dma->STREAM[_config->tx_stream].CR =
            (_config->dma_ch << CHSEL_POS) |(1 << PL_POS) | (0 << MSIZE_POS) |
            (0 << PSIZE_POS) | MINC | DIR_M2P | TCIE;
    }
    {
        using namespace dev::usart;
        _config->uart->CR1 &= CR1_MASK;
        _config->uart->CR2 &= CR2_MASK;
        _config->uart->CR3 &= CR3_MASK;
        _config->uart->BRR =
            (_config->uart->BRR & BRR_MASK) | (1 << DIV_MANT_POS) |
            (10 << DIV_FRAC_POS); // 460800 @ 12MHz
        _config->uart->CR1 |= UE;
    }
}

void Uart::debug_write(BYTE data) {
    using namespace dev::usart;

    // There is no FIFO, wait for UART to be ready
    while((_config->uart->SR & (TXE | TC)) != (TXE | TC))
        asm volatile("nop");
    _config->uart->CR3 &= ~DMAT;

    // Send IDLE
    _config->uart->CR1 &= ~TE;
    _config->uart->SR &= ~TC;
    _config->uart->CR1 |= TE;
    // Send data
    _config->uart->DR = data;
}

void Uart::write(const BYTE* buffer, BYTE length) {
    using namespace dev;
    using namespace dev::usart;

    // There is no FIFO, wait for UART to be ready
    while((_config->uart->SR & (TXE | TC)) != (TXE | TC))
        asm volatile("nop");
    _config->uart->CR3 |= DMAT;

    // Send IDLE
    _config->uart->CR1 &= ~TE;
    _config->uart->SR &= ~TC;
    _config->uart->CR1 |= TE;

    // Setup data with DMA
    _config->dma->STREAM[_config->tx_stream].M0AR =
        const_cast<WORD*>(reinterpret_cast<const WORD*>(buffer));
    _config->dma->STREAM[_config->tx_stream].NDTR = length;
    _config->dma->STREAM[_config->tx_stream].CR |= dma::EN;

    // TODO: wait for TC ISR ?
    // TODO: parallel write support
    xSemaphoreTake(_tx_semaphore, portMAX_DELAY);
}

BYTE Uart::read(BYTE* buffer, BYTE length) {
    using namespace dev;
    using namespace dev::usart;

    _config->uart->CR1 = ((_config->uart->CR1 & ~RXNEIE) | IDLEIE | RE);
    _config->uart->CR3 |= DMAR;
    _config->dma->STREAM[_config->rx_stream].M0AR = reinterpret_cast<WORD*>(buffer);
    _config->dma->STREAM[_config->rx_stream].NDTR = length;
    _config->dma->STREAM[_config->rx_stream].CR |= dma::EN;

    xSemaphoreTake(_rx_semaphore, portMAX_DELAY);

    return length - _dma_rem;
}

void Uart::ISR() {
    using namespace dev;
    using namespace dev::usart;

    WORD sr = _config->uart->SR;
    if(sr & IDLE) {
        // clear IDLE bit
        BYTE _dr = _config->uart->DR;
        (void)_dr;

        _config->dma->STREAM[_config->rx_stream].CR &= ~dma::EN;
        // This will trigger DMA_RX_ISR
    }
}

void Uart::DMA_TX_ISR() {
    using namespace dev;
    using namespace dev::dma;
    _config->dma->clear_isr(_config->tx_stream);

    BaseType_t task_woken = pdFALSE;
    xSemaphoreGiveFromISR(_tx_semaphore, &task_woken);
    portYIELD_FROM_ISR(task_woken);
}

// DMA RX was either finished or aborted.
void Uart::DMA_RX_ISR() {
    using namespace dev;
    using namespace dev::dma;
    using namespace dev::usart;

    _config->dma->clear_isr(_config->rx_stream);
    _config->dma->STREAM[_config->rx_stream].CR &= ~EN;
    _dma_rem = _config->dma->STREAM[_config->rx_stream].NDTR;
    _config->uart->CR1 &= ~RE;
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
