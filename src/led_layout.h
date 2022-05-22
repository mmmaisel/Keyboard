/**********************************************************************\
 * Keyboard
 *
 * Keyboard LED layout
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

struct LedCoordinate {
    BYTE row;
    BYTE column;
};

const USHORT LED_LAYOUT_SIZE = 256;

extern const LedCoordinate LED_LAYOUT_RIGHT[LED_LAYOUT_SIZE];
extern const LedCoordinate LED_LAYOUT_LEFT[LED_LAYOUT_SIZE];
extern const LedCoordinate LED_LAYOUT_NUM[LED_LAYOUT_SIZE];
extern const LedCoordinate LED_LAYOUT_NAV[LED_LAYOUT_SIZE];
