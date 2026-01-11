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
#include "led_matrix.h"

#include "priority.h"
#include "dev/core.h"
#include "dev/gpio.h"
#include "dev/rcc.h"
#include "dev/timer.h"

const LedMatrixConfig* LedMatrix::_config = nullptr;
const BYTE LedMatrix::LOG_SCALE[16] = {
    0, 1, 2, 4, 5, 6, 7, 9, 10, 12, 14, 17, 20, 23, 27, 32,
};
BYTE LedMatrix::_phase = LedMatrix::PHASE_DRIVE;
BYTE LedMatrix::_frac = 0;
BYTE LedMatrix::_col = 0;
BYTE LedMatrix::_colors[LedMatrixConfig::MAX_DIM][LedMatrixConfig::MAX_DIM] =
    {{0}};

void LedMatrix::initialize(const LedMatrixConfig* config) {
    using namespace dev;
    using namespace dev::rcc;
    using namespace dev::timer;

    _config = config;

    // Configure 24 kHz timer
    // Timer clock is multiplied by 2 if APB1_DIV != 1
    // f_mat = f_tim / FRAC_COUNT / n_row
    // TODO: check frequency, currently it is 21.6 kHz, not 24
    RCC->APB1ENR |= TIM2EN;
    TIM2->CR1 = DIR | URS;
    TIM2->ARR = 5;
    TIM2->PSC = 100;
    TIM2->DIER |= UIE;
    TIM2->CR1 |= CEN;

    // Enable Timer interrupt
    NVIC->enable_isr(isrnum::TIM2);
    NVIC->set_priority(isrnum::TIM2, priority::LED_MATRIX);
}

void LedMatrix::clear() {
    for(BYTE row = 0; row < _config->rows; ++row) {
        for(BYTE column = 0; column < _config->cols; ++column) {
            _colors[row][column] = 0;
        }
    }
}

void LedMatrix::set_led(BYTE num, Color color) {
    BYTE row = _config->mapping[num].row;
    BYTE column = _config->mapping[num].col;

    if(row >= LedMatrixConfig::MAX_DIM-2 || column >= LedMatrixConfig::MAX_DIM)
        return;

    _colors[row][column] = clamp_color(color.blue);
    _colors[row+1][column] = clamp_color(color.green);
    _colors[row+2][column] = clamp_color(color.red);
}

BYTE LedMatrix::clamp_color(BYTE color) {
    if(color > 15)
        return 15;
    else
        return color;
}

void LedMatrix::ISR() {
    using namespace dev;
    using namespace dev::timer;

    TIM2->SR &= ~UIF;

    if(_phase == PHASE_DRIVE) {
        if(++_frac >= FRAC_COUNT) {
            _frac = 0;

            for(BYTE row = 0; row < _config->rows; ++row) {
                // Disable all rows before column switch
                _config->row_pins[row].port->clear_odr(
                    _config->row_pins[row].pin
                );
            }

            // Columns are low active
            _config->col_pins[_col].port->set_odr(_config->col_pins[_col].pin);
            if(++_col >= _config->cols) {
                _col = 0;
                if(_config->nops != 0) {
                    _phase = PHASE_NOP;
                    return;
                }
            }
            _config->col_pins[_col].port->clear_odr(_config->col_pins[_col].pin);
        }
    } else if(_phase == PHASE_NOP) {
        if(++_frac >= FRAC_COUNT) {
            _frac = 0;
            if(++_col >= _config->nops) {
                _col = 0;
                _phase = PHASE_DRIVE;
                _config->col_pins[_col].port->clear_odr(
                    _config->col_pins[_col].pin
                );
            } else {
                return;
            }
        } else {
            return;
        }
    }

    for(BYTE row = 0; row < _config->rows; ++row) {
        if(LOG_SCALE[_colors[row][_col]] > _frac) {
            // Rows are high active
            _config->row_pins[row].port->set_odr(_config->row_pins[row].pin);
        } else {
            _config->row_pins[row].port->clear_odr(
                _config->row_pins[row].pin
            );
        }
    }
}

extern "C" void tim2_vector() {
    LedMatrix::ISR();
}
