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
        KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12, KEY_NONE,
        KEY_NONE, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0, KEY_EQUAL,
        KEY_NONE, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P, KEY_LEFTBRACE,
        KEY_ENTER, KEY_H, KEY_J, KEY_L, KEY_SEMICOLON, KEY_APOSTROPHE,
        KEY_N, KEY_M, KEY_COMMA, KEY_DOT, KEY_SLASH, KEY_RIGHTSHIFT,
        KEY_BACKSPACE, KEY_RIGHTALT, KEY_FN, KEY_MENU, KEY_RIGHTCTRL,
    },
    // TODO
    {},
    {
        KEY_PRINT, KEY_SCROLL, KEY_PAUSE,
        KEY_INSERT, KEY_HOME, KEY_PAGEUP,
        KEY_DELETE, KEY_END, KEY_PAGEDOWN,
        KEY_VOLUMEDOWN, KEY_UP, KEY_VOLUMEUP,
        KEY_LEFT, KEY_DOWN, KEY_RIGHT,
    },
    {
        KEY_NUMLOCK, KEY_KPSLASH, KEY_KPASTERISK, KEY_NONE,
        KEY_KP7, KEY_KP8, KEY_KP9, KEY_KPMINUS,
        KEY_KP4, KEY_KP5, KEY_KP6, KEY_KPPLUS,
        KEY_KP1, KEY_KP2, KEY_KP3,
        KEY_KP0, KEY_KPDOT, KEY_KPENTER,
    },
};
