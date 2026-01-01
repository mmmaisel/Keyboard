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
#include "key_layout.h"

using namespace keycodes;

const BYTE KEY_LAYOUT[PAGE_COUNT][KeyMatrixConfig::MAX_KEYS] = {
    {
        KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12, KEY_PIPEBRACKET,
        KEY_RIGHTBRACE, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0, KEY_MINUS,
        KEY_BACKSLASH, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P, KEY_EQUAL /*TAB2*/,
        KEY_ENTER, KEY_H, KEY_J, KEY_K, KEY_L, KEY_SEMICOLON, KEY_HASHTILDE /*CAPS2*/,
        KEY_N, KEY_M, KEY_COMMA, KEY_DOT, KEY_SLASH, KEY_RIGHTSHIFT,
        KEY_BACKSPACE, KEY_RIGHTALT, KEY_FN, KEY_MENU, KEY_RIGHTCTRL,
        LED_CAPSLOCK, LED_NUMLOCK, LED_SCROLL,
    },
    {
        KEY_ESC, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6,
        KEY_GRAVE, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_LEFTBRACE,
        KEY_TAB, KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_APOSTROPHE,
        KEY_CAPSLOCK, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_ENTER,
        KEY_LEFTSHIFT, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B,
        KEY_LEFTCTRL, KEY_LEFTSUPER, KEY_LEFTALT, KEY_SPACE,
    },
    {
        KEY_PRINT, KEY_SCROLL, KEY_PAUSE,
        KEY_INSERT, KEY_HOME, KEY_PAGEUP,
        KEY_DELETE, KEY_END, KEY_PAGEDOWN,
        KEY_VOLUMEDOWN, KEY_UP, KEY_VOLUMEUP,
        KEY_LEFT, KEY_DOWN, KEY_RIGHT,
    },
    {
        KEY_NUMLOCK, KEY_KPSLASH, KEY_KPASTERISK, KEY_RIGHTSUPER /*ASCEND*/,
        KEY_KP7, KEY_KP8, KEY_KP9, KEY_KPMINUS,
        KEY_KP4, KEY_KP5, KEY_KP6, KEY_KPPLUS,
        KEY_KP1, KEY_KP2, KEY_KP3,
        KEY_KP0, KEY_KPDOT, KEY_KPENTER,
    },
};
