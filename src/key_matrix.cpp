/**********************************************************************\
 * Keyboard
 *
 * Key matrix
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
#include "key_matrix.h"

#include "pinout.h"
#include "dev/core.h"
#include "dev/gpio.h"
#include "dev/rcc.h"
#include "dev/timer.h"

#include "system.h"
#include "uart.h"

#include <string.h>

BYTE KeyMatrix::m_row_count;
BYTE KeyMatrix::m_column_count;
BYTE KeyMatrix::m_column;
KeyMatrix::Pin KeyMatrix::m_rows[16];
KeyMatrix::Pin KeyMatrix::m_columns[16];
BYTE KeyMatrix::m_phase;
const BYTE KeyMatrix::KEY_VALUES[16][16] = {0}; // Keyboard layout
BYTE KeyMatrix::m_key_state[16][16] = {0};
BYTE KeyMatrix::m_key_idx;
BYTE KeyMatrix::m_keys[16];

// TODO: evaluate module
void KeyMatrix::initialize(module::Module module) {
    using namespace dev;
    using namespace dev::rcc;
    using namespace dev::timer;
    using namespace pinout;

    RCC->AHB1ENR |= GPIOBEN | GPIOCEN;
    GPIOB->MODER |= MODE_LVEN | MODE_KR1 | MODE_KR2 | MODE_KR3 | MODE_KR4 |
        MODE_KR5 | MODE_KR6 | MODE_KR7;
    GPIOC->MODER |= MODE_KC1 | MODE_KC2 | MODE_KC3 | MODE_KC4 | MODE_KC5 |
        MODE_KC6;
    GPIOB->PUPDR |= PUPD_KR1 | PUPD_KR2 | PUPD_KR3 | PUPD_KR4 | PUPD_KR5 |
        PUPD_KR6;

    GPIOB->clear_odr(LVEN);

    m_column = 0;
    m_phase = 0;
    m_key_idx = 0;

    m_row_count = 7;
    m_rows[0] = Pin { .port = GPIOB, .pin = KR1 };
    m_rows[1] = Pin { .port = GPIOB, .pin = KR2 };
    m_rows[2] = Pin { .port = GPIOB, .pin = KR3 };
    m_rows[3] = Pin { .port = GPIOB, .pin = KR4 };
    m_rows[4] = Pin { .port = GPIOB, .pin = KR5 };
    m_rows[5] = Pin { .port = GPIOB, .pin = KR6 };
    m_rows[6] = Pin { .port = GPIOB, .pin = KR7 };

    m_column_count = 6;
    m_columns[0] = Pin { .port = GPIOC, .pin = KC1 };
    m_columns[1] = Pin { .port = GPIOC, .pin = KC2 };
    m_columns[2] = Pin { .port = GPIOC, .pin = KC3 };
    m_columns[3] = Pin { .port = GPIOC, .pin = KC4 };
    m_columns[4] = Pin { .port = GPIOC, .pin = KC5 };
    m_columns[5] = Pin { .port = GPIOC, .pin = KC6 };

    // Configure 10 kHz timer
    // Timer clock is divided by 2 again if APB1_DIV != 1
    RCC->APB1ENR |= TIM3EN;
    TIM3->CR1 = DIR | URS;
    TIM3->ARR = 10;
    TIM3->PSC = 1050;
    TIM3->DIER |= UIE;
    TIM3->CR1 |= CEN;

    // Enable Timer interrupt
    NVIC->EN[0] = (1 << 29);
    NVIC->PRI[29]  = 0x10;

    GPIOC->set_odr(KC1|KC2|KC3|KC4|KC5|KC6);
}

void KeyMatrix::get_keys(BYTE* keys) {
    using namespace dev;
    NVIC->DIS[0] = (1 << 29);
    memcpy(keys, m_keys, 17);
    NVIC->EN[0] = (1 << 29);
}

void KeyMatrix::ISR() {
    using namespace dev;
    using namespace dev::timer;

    TIM3->SR &= ~UIF;

    if(m_phase == 0) {
        m_phase = 1;
        m_columns[m_column].port->set_odr(m_columns[m_column].pin);
    } else {
        m_phase = 0;
        for(BYTE row = 0; row < m_row_count; ++row) {
            if(m_rows[row].port->IDR & m_rows[row].pin) {
                if(m_key_state[row][m_column] == 3) {
                    // XXX: KEY_VALUES[row][column]
                    if(m_key_idx < 16)
                        m_keys[m_key_idx++] = '0' + m_column*16+row;
                } else {
                    ++m_key_state[row][m_column];
                }
            } else {
                m_key_state[row][m_column] = 0;
            }
        }
        m_columns[m_column].port->clear_odr(m_columns[m_column].pin);

        if(++m_column == m_column_count) {
            m_column = 0;
            // XXX: output received values via UART for testing
            for(int i = 0; i < m_key_idx; ++i)
                UART6.write(m_keys[i]);
            UART6.write('\r');
            m_key_idx = 0;
        }
    }
}

extern "C" void tim3_vector() {
    KeyMatrix::ISR();
}