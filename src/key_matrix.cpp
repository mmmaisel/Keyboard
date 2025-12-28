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

#include "dev/core.h"
#include "dev/gpio.h"
#include "dev/rcc.h"
#include "dev/timer.h"

#include "event.h"
#include "priority.h"

const KeyMatrixConfig* KeyMatrix::_config = nullptr;
BYTE KeyMatrix::_col = 0;
BYTE KeyMatrix::_phase = KeyMatrix::PHASE_DRIVE;
DWORD KeyMatrix::_key_state[KeyMatrix::STATE_CNT] = {0};
BYTE KeyMatrix::_state_idx = 0;

void KeyMatrix::initialize(const KeyMatrixConfig* config) {
    using namespace dev;
    using namespace dev::rcc;
    using namespace dev::timer;

    _config = config;

    // Configure 10 kHz timer
    // Timer clock is multiplied by 2 if APB1_DIV != 1
    // f_mat = f_tim / 2 / n_col
    // TODO: check frequency, currently it is 9.06 kHz, not 10 ??
    RCC->APB1ENR |= TIM3EN;
    TIM3->CR1 = DIR | URS;
    TIM3->ARR = 10;
    TIM3->PSC = 120;
    TIM3->DIER |= UIE;
    TIM3->CR1 |= CEN;

    // Enable Timer interrupt
    NVIC->enable_isr(isrnum::TIM3);
    NVIC->set_priority(isrnum::TIM3, priority::KEY_MATRIX);
}

void KeyMatrix::ISR() {
    using namespace dev;
    using namespace dev::timer;
    BaseType_t task_woken = pdFALSE;

    TIM3->SR &= ~UIF;

    if(_phase == PHASE_DRIVE) {
        // Drive next column.
        _config->col_pins[_col].port->set_odr(_config->col_pins[_col].pin);
        _phase = PHASE_READ;
    } else {
        // Read current columns rows
        for(BYTE row = 0; row < _config->row_count; ++row) {
            if(_config->row_pins[row].port->IDR & _config->row_pins[row].pin)
                _key_state[_state_idx] |=
                    (1ull << (_config->mapping[row][_col] - 1));
        }

        // Stop drive current column
        _config->col_pins[_col].port->clear_odr(_config->col_pins[_col].pin);

        // All columns scanned?
        if(++_col >= _config->col_count) {
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
            Event event = {
                .type = EVENT_KEYS,
                .keys = KeyEvent {
                    .page = _config->page,
                    .state = state,
                }
            };
            EventDispatcher::send_from_isr(&event, &task_woken);
        }

        _phase = PHASE_DRIVE;
    }
    portYIELD_FROM_ISR(task_woken);
}

extern "C" void tim3_vector() {
    KeyMatrix::ISR();
}
