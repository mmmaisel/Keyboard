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
#pragma once

#include "event.h"
#include "key_matrix.h"
#include "led_matrix.h"

struct Wiring {
    Wiring(BYTE mod);

    /// Event sink for this module
    EventSink * keyboard;
    /// Key matrix config for this module
    const KeyMatrixConfig* key_config;
    /// Key matrix GPIO init routine for this module
    void(*key_matrix_hw_init)();
    /// LED matrix config for this module
    const LedMatrixConfig* led_config;
    /// LED matrix GPIO init routine for this module
    void(*led_matrix_hw_init)();
};
