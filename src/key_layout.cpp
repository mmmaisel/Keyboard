/**********************************************************************\
 * Keyboard
 *
 * USB HID Keyboard layout
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
#include "key_layout.h"
#include "key_matrix.h"

using namespace keycodes;

static const BYTE NA = 0;

const BYTE KEY_LAYOUT_RIGHT[KeyMatrix::MAX_DIM][KeyMatrix::MAX_DIM] = {
    // Row 0
    {KEY_F10, KEY_8, KEY_I, KEY_K, KEY_RIGHTSUPER, KEY_COMMA, NA, NA,
        NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 1
    {KEY_F8, KEY_6, KEY_Y, KEY_H, KEY_SPACE, KEY_N, NA, NA,
        NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 2
    {KEY_F9, KEY_7, KEY_U, KEY_J, KEY_RIGHTALT, KEY_M, NA, NA,
        NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 3
    {KEY_F12, KEY_0, KEY_P, KEY_SEMICOLON, KEY_RIGHTCTRL, KEY_SLASH, NA, NA,
        NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 4
    {KEY_F11, KEY_9, KEY_O, KEY_L, KEY_MENU, KEY_DOT, NA, NA,
        NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 5
    {KEY_F7, KEY_APOSTROPHE, KEY_LEFTBRACE, KEY_HASHTILDE, KEY_ENTER, KEY_RIGHTSHIFT, NA, NA,
        NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 6
    {KEY_FN, KEY_EQUAL, KEY_RIGHTBRACE, NA, NA, NA, NA, NA,
        NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 7
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 8
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 9
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 10
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 11
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 12
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 13
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 14
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 15
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
};
const BYTE KEY_LAYOUT_LEFT[KeyMatrix::MAX_DIM][KeyMatrix::MAX_DIM] = {
    // Row 0
    {KEY_2, KEY_F2, KEY_LEFTALT, KEY_S, KEY_W, KEY_X, NA, NA,
        NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 1
    {KEY_3, KEY_F3, KEY_SPACE, KEY_D, KEY_E, KEY_C, NA, NA,
        NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 2
    {KEY_5, KEY_F5, KEY_ENTER, KEY_G, KEY_T, KEY_B, NA, NA,
        NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 3
    {KEY_1, KEY_F1, KEY_LEFTSUPER, KEY_A, KEY_Q, KEY_Z, NA, NA,
        NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 4
    {KEY_GRAVE, KEY_ESC, KEY_LEFTCTRL, KEY_CAPSLOCK, KEY_TAB, KEY_LEFTSHIFT, NA, NA,
        NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 5
    {KEY_4, KEY_F4, NA, KEY_F, KEY_R, KEY_V, NA, NA,
        NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 6
    {KEY_MINUS, KEY_F6, KEY_BACKSLASH, NA, NA, NA, NA, NA,
        NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 7
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 8
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 9
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 10
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 11
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 12
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 13
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 14
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 15
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
};
const BYTE KEY_LAYOUT_NUM[KeyMatrix::MAX_DIM][KeyMatrix::MAX_DIM] = {
    // Row 0
    {KEY_KP0, KEY_KPENTER, KEY_MUTE, KEY_KPMINUS, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 1
    {KEY_KP1, KEY_KP4, KEY_NUMLOCK, KEY_KP7, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 2
    {KEY_KP3, KEY_KP6, KEY_KPASTERISK, KEY_KP9, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 3
    {KEY_KP2, KEY_KP5, KEY_KPSLASH, KEY_KP8, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 4
    {KEY_KPDOT, KEY_KPPLUS, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 5
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 6
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 7
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 8
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 9
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 10
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 11
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 12
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 13
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 14
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 15
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
};
const BYTE KEY_LAYOUT_NAV[KeyMatrix::MAX_DIM][KeyMatrix::MAX_DIM] = {
    // Row 0
    {KEY_SCROLL, KEY_END, KEY_UP, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 1
    {KEY_PRINT, KEY_INSERT, KEY_VOLUMEDOWN, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 2
    {KEY_PAUSE, KEY_PAGEDOWN, KEY_DOWN, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 3
    {KEY_PAGEUP, KEY_VOLUMEUP, KEY_RIGHT, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 4
    {KEY_HOME, KEY_DELETE, KEY_LEFT, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 5
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 6
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 7
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 8
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 9
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 10
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 11
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 12
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 13
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 14
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
    // Row 15
    {NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA},
};
