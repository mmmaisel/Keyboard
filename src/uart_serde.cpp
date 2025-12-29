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
#include "uart_serde.h"

KeyEvent UartMessage::key_event() {
    DWORD state = 0;
    for(BYTE i = 0; i < STATE_LEN; ++i) {
        state |= DWORD(keys.state[i]) << (8 * i);
    }

    return KeyEvent {
        .page = BYTE((hdr & 0x03) + 1),
        .state = state,
    };
}

EffectEvent UartMessage::effect_event() {
    EffectId id = EFFECT_NONE;
    if(effect.id < EFFECT_COUNT)
        id = static_cast<EffectId>(effect.id);

    return EffectEvent {
        .id = id,
    };
}

UartMessage UartMessage::serialize_keys(BYTE ctr, BYTE page, DWORD state) {
    UartMessage msg = {};

    msg.hdr = (ctr << 4) | ((EVENT_KEYS-1) << 2) | (page - 1);
    for(BYTE i = 0; i < STATE_LEN; ++i) {
        msg.keys.state[i] = state & 0xFF;
        state >>= 8;
    }

    // TODO: crc8
    return msg;
}

UartMessage UartMessage::serialize_effect(BYTE ctr, BYTE id) {
    UartMessage msg = {};

    msg.hdr = (ctr << 4) | ((EVENT_EFFECT-1) << 2);
    msg.effect.id = id;

    // TODO: crc8
    return msg;
}
