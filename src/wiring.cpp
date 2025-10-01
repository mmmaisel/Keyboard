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
#include "pin.h"
#include "pinout.h"

using namespace dev;
using namespace dev::rcc;
using namespace pinout::right;

using Coord = LedMatrixConfig::Coord;

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
    .page = 1,
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
        { 4, 11, 18, 25, 37, 31},
        { 2,  9, 16, 23, 35, 29},
        { 3, 10, 17, 24, 36, 30},
        { 6, 13, 20, 27, 39, 33},
        { 5, 12, 19, 26, 38, 32},
        { 1,  8, 15, 28, 22, 34},
        { 7, 14, 21,  0,  0,  0},
    }
};

void led_matrix_hw_init_right() {
    RCC->AHB1ENR |= GPIOBEN | GPIODEN | GPIOEEN;
    GPIOB->MODER |= MODE_LVEN;
    GPIOD->MODER |= MODE_LC1 | MODE_LC2 | MODE_LC3 | MODE_LC4 | MODE_LC5 |
        MODE_LC6 | MODE_LC7 | MODE_LC8 | MODE_LC9;
    GPIOE->MODER |= MODE_LR1 | MODE_LR2 | MODE_LR3 | MODE_LR4 | MODE_LR5 |
        MODE_LR6 | MODE_LR7 | MODE_LR8 | MODE_LR9 | MODE_LR10 | MODE_LR11 |
        MODE_LR12 | MODE_LR13 | MODE_LR14 | MODE_LR15;
    GPIOB->clear_odr(LVEN);
    GPIOD->set_odr(LC1|LC2|LC3|LC4|LC5|LC6|LC7|LC8|LC9);
    GPIOE->clear_odr(LR1|LR2|LR3|LR4|LR5|LR6|LR7|LR8|LR9|LR10|LR11|LR12|LR13|LR14|LR15);
}

LedMatrixConfig led_matrix_cfg_right = {
    .rows = 15,
    .cols = 9,
    .row_pins = {
        Pin { .port = GPIOE, .pin = LR1 },
        Pin { .port = GPIOE, .pin = LR2 },
        Pin { .port = GPIOE, .pin = LR3 },
        Pin { .port = GPIOE, .pin = LR4 },
        Pin { .port = GPIOE, .pin = LR5 },
        Pin { .port = GPIOE, .pin = LR6 },
        Pin { .port = GPIOE, .pin = LR7 },
        Pin { .port = GPIOE, .pin = LR8 },
        Pin { .port = GPIOE, .pin = LR9 },
        Pin { .port = GPIOE, .pin = LR10 },
        Pin { .port = GPIOE, .pin = LR11 },
        Pin { .port = GPIOE, .pin = LR12 },
        Pin { .port = GPIOE, .pin = LR13 },
        Pin { .port = GPIOE, .pin = LR14 },
        Pin { .port = GPIOE, .pin = LR15 },
    },
    .col_pins = {
        Pin { .port = GPIOD, .pin = LC1 },
        Pin { .port = GPIOD, .pin = LC2 },
        Pin { .port = GPIOD, .pin = LC3 },
        Pin { .port = GPIOD, .pin = LC4 },
        Pin { .port = GPIOD, .pin = LC5 },
        Pin { .port = GPIOD, .pin = LC6 },
        Pin { .port = GPIOD, .pin = LC7 },
        Pin { .port = GPIOD, .pin = LC8 },
        Pin { .port = GPIOD, .pin = LC9 },
    },
    .mapping = {
        Coord { .row =  0, .col = 0 }, // sentinel
        Coord { .row =  6, .col = 3 }, // row 1
        Coord { .row =  9, .col = 3 },
        Coord { .row =  9, .col = 4 },
        Coord { .row = 12, .col = 3 },
        Coord { .row =  0, .col = 3 },
        Coord { .row =  3, .col = 3 },
        Coord { .row =  3, .col = 4 },
        Coord { .row =  6, .col = 2 }, // row 2
        Coord { .row =  9, .col = 2 },
        Coord { .row =  9, .col = 8 },
        Coord { .row = 12, .col = 2 },
        Coord { .row =  0, .col = 2 },
        Coord { .row =  3, .col = 8 },
        Coord { .row =  3, .col = 2 },
        Coord { .row =  6, .col = 1 }, // row 3
        Coord { .row =  9, .col = 1 },
        Coord { .row =  9, .col = 0 },
        Coord { .row = 12, .col = 1 },
        Coord { .row =  0, .col = 1 },
        Coord { .row =  3, .col = 1 },
        Coord { .row =  3, .col = 0 },
        Coord { .row =  6, .col = 5 }, // row 4
        Coord { .row =  9, .col = 7 },
        Coord { .row =  9, .col = 5 },
        Coord { .row = 12, .col = 0 },
        Coord { .row =  0, .col = 7 },
        Coord { .row =  3, .col = 7 },
        Coord { .row =  3, .col = 5 },
        Coord { .row =  6, .col = 7 }, // row 5
        Coord { .row =  9, .col = 6 },
        Coord { .row = 12, .col = 5 },
        Coord { .row =  0, .col = 5 },
        Coord { .row =  3, .col = 6 },
        Coord { .row =  0, .col = 4 },
        Coord { .row =  6, .col = 6 }, // row 6
        Coord { .row = 12, .col = 4 },
        Coord { .row =  0, .col = 6 },
        Coord { .row =  0, .col = 0 },
        Coord { .row =  0, .col = 8 },
    }
};

Wiring::Wiring(BYTE mod_id) {
    switch(mod_id) {
        case Module::RIGHT:
            key_config = &key_matrix_cfg_right;
            key_matrix_hw_init = key_matrix_hw_init_right;
            led_config = &led_matrix_cfg_right;
            led_matrix_hw_init = led_matrix_hw_init_right;
            break;
        default:
            break;
    }
}
