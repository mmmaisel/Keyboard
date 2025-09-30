/******************************************************************************\
    Split Keyboard
    Copyright (C) 2025 - Max Maisel

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
#include "wiring.h"

#include "dev/gpio.h"
#include "dev/rcc.h"

#include "module.h"
#include "pinout.h"

using namespace dev;
using namespace dev::rcc;
using namespace pinout::right;

using Pin = KeyMatrixConfig::Pin;

void key_matrix_hw_init_right() {

    RCC->AHB1ENR |= GPIOBEN | GPIOCEN;
    GPIOB->MODER |= MODE_KR1 | MODE_KR2 | MODE_KR3 | MODE_KR4 |
        MODE_KR5 | MODE_KR6 | MODE_KR7;
    GPIOC->MODER |= MODE_KC1 | MODE_KC2 | MODE_KC3 | MODE_KC4 | MODE_KC5 |
        MODE_KC6;
    GPIOB->PUPDR |= PUPD_KR1 | PUPD_KR2 | PUPD_KR3 | PUPD_KR4 | PUPD_KR5 |
        PUPD_KR6 | PUPD_KR7;
    GPIOC->clear_odr(KC1|KC2|KC3|KC4|KC5|KC6);
}

KeyMatrixConfig key_matrix_cfg_right = {
    .row_count = 7,
    .col_count = 6,
    .row_pins = {
        Pin { .port = GPIOB, .pin = KR1 },
        Pin { .port = GPIOB, .pin = KR2 },
        Pin { .port = GPIOB, .pin = KR3 },
        Pin { .port = GPIOB, .pin = KR4 },
        Pin { .port = GPIOB, .pin = KR5 },
        Pin { .port = GPIOB, .pin = KR6 },
        Pin { .port = GPIOB, .pin = KR7 },
    },
    .col_pins = {
        Pin { .port = GPIOC, .pin = KC1 },
        Pin { .port = GPIOC, .pin = KC2 },
        Pin { .port = GPIOC, .pin = KC3 },
        Pin { .port = GPIOC, .pin = KC4 },
        Pin { .port = GPIOC, .pin = KC5 },
        Pin { .port = GPIOC, .pin = KC6 },
    },
    .mapping = {
        { 4, 11, 18, 25,  0, 31},
        { 2,  9, 16, 23, 35, 29},
        { 3, 10, 17, 24, 36, 30},
        { 6, 13, 20,  0, 39, 33},
        { 5, 12, 19, 26, 38, 32},
        { 1, 14, 27,  0, 22, 34},
        {37, 14,  8,  0,  0,  0},
    }
};

Wiring::Wiring(BYTE mod_id) {
    switch(mod_id) {
        case Module::RIGHT:
            key_config = &key_matrix_cfg_right;
            key_matrix_hw_init = key_matrix_hw_init_right;
            break;
        default:
            break;
    }
}
