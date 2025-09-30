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
#include "key_matrix.h"

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/queue.h"

#include "dev/core.h"
#include "dev/gpio.h"
#include "dev/rcc.h"
#include "dev/timer.h"

#include "priority.h"

#include <string.h>

const KeyMatrixConfig* KeyMatrix::_config = nullptr;
QueueHandle_t KeyMatrix::_qtx = nullptr;
BYTE KeyMatrix::_col = 0;
BYTE KeyMatrix::_phase = KeyMatrix::PHASE_DRIVE;
DWORD KeyMatrix::_key_state[KeyMatrix::STATE_CNT] = {0};
BYTE KeyMatrix::_state_idx = 0;

void KeyMatrix::initialize(const KeyMatrixConfig* config, QueueHandle_t qtx) {
    using namespace dev;
    using namespace dev::rcc;
    using namespace dev::timer;

    /*switch(Module::get_id()) {
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

            m_page.id = 1;
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

            m_page.id = 2;
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

            m_page.id = 3;
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
    }*/

    _config = config;
    _qtx = qtx;

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

    if(_phase == PHASE_DRIVE) {
        // Drive next column.
        _config->col_pins[_col].port->set_odr(_config->col_pins[_col].pin);
        _phase = PHASE_READ;
    } else {
        // Read current columns rows
        for(BYTE row = 0; row < _config->row_count; ++row) {
            if(_config->row_pins[row].port->IDR & _config->row_pins[row].pin)
                _key_state[_state_idx] |= (1 << _config->mapping[row][_col]);
        }

        // Stop drive current column
        _config->col_pins[_col].port->clear_odr(_config->col_pins[_col].pin);

        // All columns scanned?
        if(++_col == _config->col_count) {
            _col= 0;
            // Rotate state
            if(++_state_idx >= STATE_CNT)
                _state_idx = 0;

            // Debounce state
            DWORD state = _key_state[0];
            for(BYTE i = 1; i < STATE_CNT; ++i)
                state &= _key_state[i];
            // Clear state for next scan
            _key_state[_state_idx] = 0;

            // Send event
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xQueueSendFromISR(_qtx, &state, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }

        _phase = PHASE_DRIVE;
    }
}

extern "C" void tim3_vector() {
    KeyMatrix::ISR();
}
