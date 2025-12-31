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
BYTE LedMatrix::_phase = 0;
BYTE LedMatrix::_col = 0;
BYTE LedMatrix::_colors[LedMatrixConfig::MAX_DIM][LedMatrixConfig::MAX_DIM] = {{0}};

void LedMatrix::initialize(const LedMatrixConfig* config) {
    using namespace dev;
    using namespace dev::rcc;
    using namespace dev::timer;

    _config = config;

    // Configure 24 kHz timer
    // Timer clock is multiplied by 2 if APB1_DIV != 1
    // f_mat = f_tim / PHASE_COUNT / n_row
    // TODO: check frequency, currently it is 21.6 kHz, not 24
    // TODO: bigger matrices need lower pre-resistors
    RCC->APB1ENR |= TIM2EN;
    TIM2->CR1 = DIR | URS;
    TIM2->ARR = 10;
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

    _colors[row][column] = color.blue;
    _colors[row+1][column] = color.green;
    _colors[row+2][column] = color.red;
}

void LedMatrix::ISR() {
    using namespace dev;
    using namespace dev::timer;

    TIM2->SR &= ~UIF;

    if(++_phase >= PHASE_COUNT) {
        for(BYTE row = 0; row < _config->rows; ++row) {
            // Disable all rows before column switch
            _config->row_pins[row].port->clear_odr(_config->row_pins[row].pin);
        }

        // Columns are low active
        _config->col_pins[_col].port->set_odr(_config->col_pins[_col].pin);
        if(++_col >= _config->cols)
            _col = 0;
        _config->col_pins[_col].port->clear_odr(_config->col_pins[_col].pin);
        _phase = 0;
    }

    for(BYTE row = 0; row < _config->rows; ++row) {
        if(_colors[row][_col] > _phase)
            // Rows are high active
            _config->row_pins[row].port->set_odr(_config->row_pins[row].pin);
        else
            _config->row_pins[row].port->clear_odr(_config->row_pins[row].pin);
    }
}

extern "C" void tim2_vector() {
    LedMatrix::ISR();
}
