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
#pragma once

#include "types.h"

namespace keycodes {
    // modifiers - first byte in the report
    enum {
        MOD_LCTRL = (1 << 0),
        MOD_LSHIFT = (1 << 1),
        MOD_LALT = (1 << 2),
        MOD_LSUPER = (1 << 3),
        MOD_RCTRL = (1 << 4),
        MOD_RSHIFT = (1 << 5),
        MOD_RALT = (1 << 6),
        MOD_RSUPER = (1 << 7)
    };

    // scancodes - bytes 3-8 in the report
    enum {
        KEY_NONE = 0,
        KEY_ERROR_OVERFLOW = 1,
        KEY_ERROR_POST = 2,
        KEY_ERROR_GENERIC = 3,

        KEY_A = 4,
        KEY_B = 5,
        KEY_C = 6,
        KEY_D = 7,
        KEY_E = 8,
        KEY_F = 9,
        KEY_G = 10,
        KEY_H = 11,
        KEY_I = 12,
        KEY_J = 13,
        KEY_K = 14,
        KEY_L = 15,
        KEY_M = 16,
        KEY_N = 17,
        KEY_O = 18,
        KEY_P = 19,
        KEY_Q = 20,
        KEY_R = 21,
        KEY_S = 22,
        KEY_T = 23,
        KEY_U = 24,
        KEY_V = 25,
        KEY_W = 26,
        KEY_X = 27,
        KEY_Y = 28,
        KEY_Z = 29,

        KEY_1 = 30,
        KEY_2 = 31,
        KEY_3 = 32,
        KEY_4 = 33,
        KEY_5 = 34,
        KEY_6 = 35,
        KEY_7 = 36,
        KEY_8 = 37,
        KEY_9 = 38,
        KEY_0 = 39,

        KEY_ENTER = 40,
        KEY_ESC = 41,
        KEY_BACKSPACE = 42,
        KEY_TAB = 43,
        KEY_SPACE = 44,
        KEY_MINUS = 45,
        KEY_EQUAL = 46,
        KEY_LEFTBRACE = 47,
        KEY_RIGHTBRACE = 48,
        KEY_BACKSLASH = 49,
        KEY_HASHTILDE = 50,
        KEY_SEMICOLON = 51,
        KEY_APOSTROPHE = 52,
        KEY_GRAVE = 53,
        KEY_COMMA = 54,
        KEY_DOT = 55,
        KEY_SLASH = 56,
        KEY_CAPSLOCK = 57,

        KEY_F1 = 58,
        KEY_F2 = 59,
        KEY_F3 = 60,
        KEY_F4 = 61,
        KEY_F5 = 62,
        KEY_F6 = 63,
        KEY_F7 = 64,
        KEY_F8 = 65,
        KEY_F9 = 66,
        KEY_F10 = 67,
        KEY_F11 = 68,
        KEY_F12 = 69,

        KEY_PRINT = 70,
        KEY_SCROLL = 71,
        KEY_PAUSE = 72,
        KEY_INSERT = 73,
        KEY_HOME = 74,
        KEY_PAGEUP = 75,
        KEY_DELETE = 76,
        KEY_END = 77,
        KEY_PAGEDOWN = 78,
        KEY_RIGHT = 79,
        KEY_LEFT = 80,
        KEY_DOWN = 81,
        KEY_UP = 82,

        KEY_NUMLOCK = 83,
        KEY_KPSLASH = 84,
        KEY_KPASTERISK = 85,
        KEY_KPMINUS = 86,
        KEY_KPPLUS = 87,
        KEY_KPENTER = 88,
        KEY_KP1 = 89,
        KEY_KP2 = 90,
        KEY_KP3 = 91,
        KEY_KP4 = 92,
        KEY_KP5 = 93,
        KEY_KP6 = 94,
        KEY_KP7 = 95,
        KEY_KP8 = 96,
        KEY_KP9 = 97,
        KEY_KP0 = 98,
        KEY_KPDOT = 99,

        KEY_PIPEBRACKET = 100,
        KEY_COMPOSE = 101,
        KEY_POWER = 102,
        KEY_KPEQUAL = 103,

        KEY_F13 = 104,
        KEY_F14 = 105,
        KEY_F15 = 106,
        KEY_F16 = 107,
        KEY_F17 = 108,
        KEY_F18 = 109,
        KEY_F19 = 110,
        KEY_F20 = 111,
        KEY_F21 = 112,
        KEY_F22 = 113,
        KEY_F23 = 114,
        KEY_F24 = 115,

        KEY_EXEC = 116,
        KEY_HELP = 117,
        KEY_MENU = 118,
        KEY_SELECT = 119,
        KEY_STOP = 120,
        KEY_AGAIN = 121,
        KEY_UNDO = 122,
        KEY_CUT = 123,
        KEY_COPY = 124,
        KEY_PASTE = 125,
        KEY_FIND = 126,
        KEY_MUTE = 127,
        KEY_VOLUMEUP = 128,
        KEY_VOLUMEDOWN = 129,

        KEY_LEFTCTRL = 224,
        KEY_LEFTSHIFT = 225,
        KEY_LEFTALT = 226,
        KEY_LEFTSUPER = 227,
        KEY_RIGHTCTRL = 228,
        KEY_RIGHTSHIFT = 229,
        KEY_RIGHTALT = 230,
        KEY_RIGHTSUPER = 231
    };
}

extern const BYTE LAYOUT_RIGHT[16][16];
extern const BYTE LAYOUT_LEFT[16][16];
extern const BYTE LAYOUT_NUM[16][16];
extern const BYTE LAYOUT_NAV[16][16];
