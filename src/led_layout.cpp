/**********************************************************************\
 * Keyboard
 *
 * HID keycode to LED matrix row+column lookup tables
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
#include "led_layout.h"

static const BYTE NA = 255;

const LedCoordinate LED_LAYOUT_RIGHT[LED_LAYOUT_SIZE] = {
    // Keycode 0
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {9, 7},   {12, 1},  {9, 5},   {12, 0},  {0, 7},
    // Keycode 16
    {9, 6},   {6, 7},   {0, 1},   {3, 1},   {NA, NA}, {6, 6},   {NA, NA}, {NA, NA},
    {9, 0},   {NA, NA}, {NA, NA}, {NA, NA}, {9, 1},   {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 32
    {NA, NA}, {NA, NA}, {NA, NA}, {9, 2},   {9, 8},   {12, 2},  {0, 2},   {3, 8},
    {6, 5},   {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {3, 2},   {6, 1},
    // Keycode 48
    {3, 0},   {NA, NA}, {3, 5},   {3, 7},   {6, 2},   {NA, NA}, {12, 5},  {0, 5},
    {3, 6},   {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 64
    {6, 3},   {9, 3},   {9, 4},   {12, 3},  {0, 3},   {3, 3},   {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 80
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 96
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 112
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {0, 0},   {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 128
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 144
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 160
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 176
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 192
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 208
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 224
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {0, 8},   {0, 4},   {12, 4},  {0, 6},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 240
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {6, 0},   {6, 8},   {6, 4},   {3, 4},
};

const LedCoordinate LED_LAYOUT_LEFT[LED_LAYOUT_SIZE] = {
    // Keycode 0
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {9, 3},   {6, 4},   {0, 4},   {0, 3},
    {0, 2},   {0, 5},   {6, 3},   {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 16
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {9, 2},   {0, 7},   {9, 5},   {6, 2},
    {NA, NA}, {0, 6},   {9, 7},   {9, 6},   {NA, NA}, {9, 4},   {9, 1},   {9, 8},
    // Keycode 32
    {0, 1},   {0, 8},   {6, 1},   {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {6, 5},   {3, 0},   {NA, NA}, {3, 2},   {6, 6},   {6, 8},   {NA, NA}, {NA, NA},
    // Keycode 48
    {NA, NA}, {6, 7},   {NA, NA}, {NA, NA}, {NA, NA}, {3, 1},   {NA, NA}, {NA, NA},
    {NA, NA}, {3, 3},   {9, 0},   {9, 9},   {0, 0},   {0, 9},   {6, 0},   {6, 9},
    // Keycode 64
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 80
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 96
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 112
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 128
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 144
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 160
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 176
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 192
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 208
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 224
    {3, 5},   {3, 4},   {3, 7},   {3, 6},   {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 240
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {3, 9},   {3, 8},   {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
};

const LedCoordinate LED_LAYOUT_NUM[LED_LAYOUT_SIZE] = {
    // Keycode 0
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 16
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 32
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 48
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 64
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 80
    {NA, NA}, {NA, NA}, {NA, NA}, {0, 0},   {3, 0},   {3, 3},   {6, 3},   {6, 1},
    {6, 5},   {0, 2},   {0, 4},   {6, 2},   {0, 1},   {3, 4},   {3, 5},   {0, 3},
    // Keycode 96
    {3, 1},   {3, 2},   {0, 5},   {6, 4},   {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 112
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {6, 0},
    // Keycode 128
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 144
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 160
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 176
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 192
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 208
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 224
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 240
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
};

const LedCoordinate LED_LAYOUT_NAV[LED_LAYOUT_SIZE] = {
    // Keycode 0
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 16
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 32
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 48
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 64
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {0, 0},   {3, 0},
    {6, 0},   {0, 1},   {3, 1},   {6, 1},   {0, 2},   {3, 2},   {6, 2},   {6, 3},
    // Keycode 80
    {0, 3},   {3, 3},   {3, 4}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 96
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 112
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 128
    {6, 4},   {0, 4},   {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 144
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 160
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 176
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 192
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 208
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 224
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    // Keycode 240
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
    {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA}, {NA, NA},
};
