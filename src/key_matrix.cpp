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
#include "key_layout.h"
#include "modular_keyboard.h"

#include "pinout.h"
#include "priority.h"
#include "module.h"
#include "dev/core.h"
#include "dev/gpio.h"
#include "dev/rcc.h"
#include "dev/timer.h"

#include <string.h>

BYTE KeyMatrix::m_row_count;
BYTE KeyMatrix::m_column_count;
BYTE KeyMatrix::m_column = 0;
KeyMatrix::Pin KeyMatrix::m_rows[MAX_DIM] = {0};
KeyMatrix::Pin KeyMatrix::m_columns[MAX_DIM] = {0};
BYTE KeyMatrix::m_phase = 0;
const BYTE* KeyMatrix::m_key_layout = 0;
BYTE KeyMatrix::m_key_state[MAX_DIM][MAX_DIM] = {0};
BYTE KeyMatrix::m_key_idx = 0;
BYTE KeyMatrix::m_keys[MAX_KEYS] = {0};

void KeyMatrix::initialize() {
    using namespace dev;
    using namespace dev::rcc;
    using namespace dev::timer;

    switch(Module::get_id()) {
        case Module::RIGHT: {
            using namespace pinout::right;
            RCC->AHB1ENR |= GPIOBEN | GPIOCEN;
            GPIOB->MODER |= MODE_KR1 | MODE_KR2 | MODE_KR3 | MODE_KR4 |
                MODE_KR5 | MODE_KR6 | MODE_KR7;
            GPIOC->MODER |= MODE_KC1 | MODE_KC2 | MODE_KC3 | MODE_KC4 | MODE_KC5 |
                MODE_KC6;
            GPIOB->PUPDR |= PUPD_KR1 | PUPD_KR2 | PUPD_KR3 | PUPD_KR4 | PUPD_KR5 |
                PUPD_KR6 | PUPD_KR7;
            GPIOC->clear_odr(KC1|KC2|KC3|KC4|KC5|KC6);

            m_key_layout = &KEY_LAYOUT_RIGHT[0][0];

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
            break;
        }

        case Module::LEFT: {
            using namespace pinout::left;
            RCC->AHB1ENR |= GPIOBEN | GPIOCEN;
            GPIOB->MODER |= MODE_KR7;
            GPIOC->MODER |= MODE_KC1 | MODE_KC2 | MODE_KC3 | MODE_KC4 | MODE_KC5 |
                MODE_KC6 | MODE_KR1 | MODE_KR2 | MODE_KR3 | MODE_KR4 | MODE_KR5 |
                MODE_KR6;
            GPIOB->PUPDR |= PUPD_KR7;
            GPIOC->PUPDR |= PUPD_KR1 | PUPD_KR2 | PUPD_KR3 | PUPD_KR4 | PUPD_KR5 |
                PUPD_KR6;
            GPIOC->clear_odr(KC1|KC2|KC3|KC4|KC5|KC6);

            m_key_layout = &KEY_LAYOUT_LEFT[0][0];

            m_row_count = 7;
            m_rows[0] = Pin { .port = GPIOC, .pin = KR1 };
            m_rows[1] = Pin { .port = GPIOC, .pin = KR2 };
            m_rows[2] = Pin { .port = GPIOC, .pin = KR3 };
            m_rows[3] = Pin { .port = GPIOC, .pin = KR4 };
            m_rows[4] = Pin { .port = GPIOC, .pin = KR5 };
            m_rows[5] = Pin { .port = GPIOC, .pin = KR6 };
            m_rows[6] = Pin { .port = GPIOB, .pin = KR7 };

            m_column_count = 6;
            m_columns[0] = Pin { .port = GPIOC, .pin = KC1 };
            m_columns[1] = Pin { .port = GPIOC, .pin = KC2 };
            m_columns[2] = Pin { .port = GPIOC, .pin = KC3 };
            m_columns[3] = Pin { .port = GPIOC, .pin = KC4 };
            m_columns[4] = Pin { .port = GPIOC, .pin = KC5 };
            m_columns[5] = Pin { .port = GPIOC, .pin = KC6 };
            break;
        }

        case Module::NAV: {
            using namespace pinout::nav;
            RCC->AHB1ENR |= GPIOBEN | GPIOCEN;
            GPIOC->MODER |= MODE_KC1 | MODE_KC2 | MODE_KC3 |
                MODE_KR1 | MODE_KR2 | MODE_KR3 | MODE_KR4 | MODE_KR5;
            GPIOC->PUPDR |= PUPD_KR1 | PUPD_KR2 | PUPD_KR3 | PUPD_KR4 | PUPD_KR5;
            GPIOC->clear_odr(KC1|KC2|KC3);

            m_key_layout = &KEY_LAYOUT_NAV[0][0];

            m_row_count = 5;
            m_rows[0] = Pin { .port = GPIOC, .pin = KR1 };
            m_rows[1] = Pin { .port = GPIOC, .pin = KR2 };
            m_rows[2] = Pin { .port = GPIOC, .pin = KR3 };
            m_rows[3] = Pin { .port = GPIOC, .pin = KR4 };
            m_rows[4] = Pin { .port = GPIOC, .pin = KR5 };

            m_column_count = 3;
            m_columns[0] = Pin { .port = GPIOC, .pin = KC1 };
            m_columns[1] = Pin { .port = GPIOC, .pin = KC2 };
            m_columns[2] = Pin { .port = GPIOC, .pin = KC3 };
            break;
        }

        case Module::NUM: {
            using namespace pinout::num;
            RCC->AHB1ENR |= GPIOBEN | GPIOCEN;
            GPIOC->MODER |= MODE_KC1 | MODE_KC2 | MODE_KC3 | MODE_KC4 |
                MODE_KR1 | MODE_KR2 | MODE_KR3 | MODE_KR4 | MODE_KR5;
            GPIOC->PUPDR |= PUPD_KR1 | PUPD_KR2 | PUPD_KR3 | PUPD_KR4 | PUPD_KR5;
            GPIOC->clear_odr(KC1|KC2|KC3|KC4);

            m_key_layout = &KEY_LAYOUT_NUM[0][0];

            m_row_count = 5;
            m_rows[0] = Pin { .port = GPIOC, .pin = KR1 };
            m_rows[1] = Pin { .port = GPIOC, .pin = KR2 };
            m_rows[2] = Pin { .port = GPIOC, .pin = KR3 };
            m_rows[3] = Pin { .port = GPIOC, .pin = KR4 };
            m_rows[4] = Pin { .port = GPIOC, .pin = KR5 };

            m_column_count = 4;
            m_columns[0] = Pin { .port = GPIOC, .pin = KC1 };
            m_columns[1] = Pin { .port = GPIOC, .pin = KC2 };
            m_columns[2] = Pin { .port = GPIOC, .pin = KC3 };
            m_columns[3] = Pin { .port = GPIOC, .pin = KC4 };
            break;
        }
    }

    // Configure 10 kHz timer
    // Timer clock is divided by 2 again if APB1_DIV != 1
    RCC->APB1ENR |= TIM3EN;
    TIM3->CR1 = DIR | URS;
    TIM3->ARR = 10;
    TIM3->PSC = 1050;
    TIM3->DIER |= UIE;
    TIM3->CR1 |= CEN;

    // Enable Timer interrupt
    NVIC->enable_isr(isrnum::TIM3);
    NVIC->set_priority(isrnum::TIM3, priority::KEY_MATRIX);
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
                    if(m_key_idx < MAX_KEYS)
                        m_keys[m_key_idx++] =
                            *(m_key_layout + MAX_DIM*row + m_column);
                } else {
                    ++m_key_state[row][m_column];
                }
            } else {
                m_key_state[row][m_column] = 0;
            }
        }
        m_columns[m_column].port->clear_odr(m_columns[m_column].pin);

        if(++m_column == m_column_count) {
            for(BYTE i = m_key_idx; i < MAX_KEYS; ++i) {
                m_keys[i] = 0;
            }
            m_column = 0;
            m_key_idx = 0;
            ModularKeyboard::update_keys(Module::get_id(), m_keys);
        }
    }
}

extern "C" void tim3_vector() {
    KeyMatrix::ISR();
}
