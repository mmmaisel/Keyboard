/**********************************************************************\
 * Keyboard
 *
 * Ringbuffer Template
 **********************************************************************
 * Copyright (C) 2014 - 2022 - Max Maisel
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

/// Ringbuffer template with maximum size of 255.
template <typename T, typename U, U S>
class Ringbuffer {
    public:
        Ringbuffer() {clear();}
        Ringbuffer(const Ringbuffer& rhs);
        ~Ringbuffer() {};

        enum eState {
            GOOD = 0,
            OVERFLOW = (1 << 0),
            UNDERFLOW = (1 << 1)
        };

        inline void push(T src);
        inline void push(T src) volatile;
        inline T pop();
        inline T pop() volatile;
        inline T& peek();
        inline T& peek() volatile;

        // Get amount of bytes in buffer.
        inline U size() const {return m_size;}
        inline U size() volatile const {return m_size;}

        inline BYTE underflow() const
            {BYTE ret = m_state & UNDERFLOW; m_state &= ~UNDERFLOW; return ret;}
        inline BYTE underflow() const volatile
            {BYTE ret = m_state & UNDERFLOW; m_state &= ~UNDERFLOW; return ret;}
        inline BYTE overflow() const
            {BYTE ret = m_state & OVERFLOW; m_state &= ~OVERFLOW; return ret;}
        inline BYTE overflow() const volatile
            {BYTE ret = m_state & OVERFLOW; m_state &= ~OVERFLOW; return ret;}

        inline void clear();
        inline void clear() volatile;

    private:
        T m_buffer[S];
        U m_pos_head;
        U m_pos_tail;
        U m_size;
        mutable BYTE m_state;
};

#include "ringbuffer.inl"
