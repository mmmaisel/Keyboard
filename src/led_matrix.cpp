/**********************************************************************\
 * Keyboard
 *
 * LED matrix
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
#include "led_matrix.h"

#include "pinout.h"
#include "dev/core.h"
#include "dev/gpio.h"
#include "dev/rcc.h"
#include "dev/timer.h"

BYTE LedMatrix::m_phase;
BYTE LedMatrix::m_row;
BYTE LedMatrix::m_row_count;
BYTE LedMatrix::m_column_count;
LedMatrix::Pin LedMatrix::m_rows[MAX_DIM];
LedMatrix::Pin LedMatrix::m_columns[MAX_DIM];
BYTE LedMatrix::m_phases[MAX_DIM][MAX_DIM];

// TODO: evaluate module
void LedMatrix::initialize(module::Module module) {
    using namespace dev;
    using namespace dev::rcc;
    using namespace dev::timer;
    using namespace pinout;

    RCC->AHB1ENR |= GPIOBEN | GPIODEN | GPIOEEN;
    GPIOB->MODER |= MODE_LVEN;
    GPIOD->MODER |= MODE_LC1 | MODE_LC2 | MODE_LC3 | MODE_LC4 | MODE_LC5 |
        MODE_LC6 | MODE_LC7 | MODE_LC8 | MODE_LC9;
    GPIOE->MODER |= MODE_LR1 | MODE_LR2 | MODE_LR3 | MODE_LR4 | MODE_LR5 |
        MODE_LR6 | MODE_LR7 | MODE_LR8 | MODE_LR9 | MODE_LR10 | MODE_LR11 |
        MODE_LR12 | MODE_LR13 | MODE_LR14 | MODE_LR15;

    GPIOB->clear_odr(LVEN);


    m_row_count = 15;
    m_rows[0] = Pin { .port = GPIOE, .pin = LR1 };
    m_rows[1] = Pin { .port = GPIOE, .pin = LR2 };
    m_rows[2] = Pin { .port = GPIOE, .pin = LR3 };
    m_rows[3] = Pin { .port = GPIOE, .pin = LR4 };
    m_rows[4] = Pin { .port = GPIOE, .pin = LR5 };
    m_rows[5] = Pin { .port = GPIOE, .pin = LR6 };
    m_rows[6] = Pin { .port = GPIOE, .pin = LR7 };
    m_rows[7] = Pin { .port = GPIOE, .pin = LR8 };
    m_rows[8] = Pin { .port = GPIOE, .pin = LR9 };
    m_rows[9] = Pin { .port = GPIOE, .pin = LR10 };
    m_rows[10] = Pin { .port = GPIOE, .pin = LR11 };
    m_rows[11] = Pin { .port = GPIOE, .pin = LR12 };
    m_rows[12] = Pin { .port = GPIOE, .pin = LR13 };
    m_rows[13] = Pin { .port = GPIOE, .pin = LR14 };
    m_rows[14] = Pin { .port = GPIOE, .pin = LR15 };

    m_column_count = 9;
    m_columns[0] = Pin { .port = GPIOD, .pin = LC1 };
    m_columns[1] = Pin { .port = GPIOD, .pin = LC2 };
    m_columns[2] = Pin { .port = GPIOD, .pin = LC3 };
    m_columns[3] = Pin { .port = GPIOD, .pin = LC4 };
    m_columns[4] = Pin { .port = GPIOD, .pin = LC5 };
    m_columns[5] = Pin { .port = GPIOD, .pin = LC6 };
    m_columns[6] = Pin { .port = GPIOD, .pin = LC7 };
    m_columns[7] = Pin { .port = GPIOD, .pin = LC8 };
    m_columns[8] = Pin { .port = GPIOD, .pin = LC9 };

    // XXX: Some test values
    m_phases[8][3] = 4;
    m_phases[7][1] = 1;
    m_phases[11][3] = 4;
    m_phases[9][4] = 4;
    m_phases[10][4] = 4;
    m_phases[11][4] = 4;

    // Configure 16 kHz timer
    // Timer clock is divided by 2 again if APB1_DIV != 1
    RCC->APB1ENR |= TIM2EN;
    TIM2->CR1 = DIR | URS;
    TIM2->ARR = 10;
    TIM2->PSC = 168;
    TIM2->DIER |= UIE;
    TIM2->CR1 |= CEN;

    // Enable Timer interrupt
    NVIC->enable_isr(isrnum::TIM2);
    NVIC->PRI[isrnum::TIM2]  = 0x09;

    GPIOD->set_odr(LC1|LC2|LC3|LC4|LC5|LC6|LC7|LC8|LC9);
    GPIOE->clear_odr(LR1|LR2|LR3|LR4|LR5|LR6|LR7|LR8|LR9|LR10|LR11|LR12|LR13|LR14|LR15);
}

void LedMatrix::ISR() {
    using namespace dev;
    using namespace dev::timer;

    TIM2->SR &= ~UIF;

    if(++m_phase == PHASE_COUNT)
    {
        // rows are high active
        m_rows[m_row].port->clear_odr(m_rows[m_row].pin);
        if(++m_row == m_row_count)
            m_row = 0;
        m_rows[m_row].port->set_odr(m_rows[m_row].pin);
        m_phase = 0;
    }

    for(register BYTE column = 0; column < m_column_count; ++column)
    {
        if(m_phases[m_row][column] > m_phase)
            // columns are low active
            m_columns[column].port->clear_odr(m_columns[column].pin);
        else
            m_columns[column].port->set_odr(m_columns[column].pin);
    }
}

extern "C" void tim2_vector() {
    LedMatrix::ISR();
}
