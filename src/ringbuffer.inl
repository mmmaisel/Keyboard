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

#include <cstring>

template <typename T, typename U, U S>
Ringbuffer<T, U, S>::Ringbuffer(const Ringbuffer& rhs) :
    m_pos_head(rhs.m_pos_head),
    m_pos_tail(rhs.m_pos_tail),
    m_state(rhs.m_state)
{
    memcpy(const_cast<T*>(m_buffer), const_cast<const T*>(rhs.m_buffer), S * sizeof(T));
}

/// Insert one byte into ringbuffer.
template <typename T, typename U, U S>
inline void Ringbuffer<T, U, S>::push(T src) {
    if(m_size == S) {
        m_state |= OVERFLOW;
    } else {
        m_buffer[m_pos_head] = src;
        ++m_size;
        if(m_pos_head == S - 1)
            m_pos_head = 0;
        else
            ++m_pos_head;
    }
}

/// Insert one byte into ringbuffer.
template <typename T, typename U, U S>
inline void Ringbuffer<T, U, S>::push(T src) volatile {
    if(m_size == S) {
        m_state |= OVERFLOW;
    } else {
        m_buffer[m_pos_head] = src;
        ++m_size;
        if(m_pos_head == S - 1)
            m_pos_head = 0;
        else
            ++m_pos_head;
    }
}

/// Read one byte from ringbuffer.
template <typename T, typename U, U S>
inline T Ringbuffer<T, U, S>::pop() {
    if(!m_size) {
        m_state |= UNDERFLOW;
        return 0;
    } else {
        T retval;
        retval = m_buffer[m_pos_tail];
        --m_size;
        if(m_pos_tail == S - 1)
            m_pos_tail = 0;
        else
            ++m_pos_tail;
        return retval;
    }
}

/// Read one byte from ringbuffer.
template <typename T, typename U, U S>
inline T Ringbuffer<T, U, S>::pop() volatile {
    if(!m_size) {
        m_state |= UNDERFLOW;
        return 0;
    } else {
        T retval;
        retval = m_buffer[m_pos_tail];
        --m_size;
        if(m_pos_tail == S - 1)
            m_pos_tail = 0;
        else
            ++m_pos_tail;
        return retval;
    }
}

/// Read next byte from ringbuffer
/// without removing it from buffer.
template <typename T, typename U, U S>
inline T& Ringbuffer<T, U, S>::peek() {
    return m_buffer[m_pos_tail];
}

/// Read next byte from ringbuffer
/// without removing it from buffer.
template <typename T, typename U, U S>
inline T& Ringbuffer<T, U, S>::peek() volatile {
    return m_buffer[m_pos_tail];
}

/// Clear the buffer.
template <typename T, typename U, U S>
void Ringbuffer<T, U, S>::clear() {
    m_pos_head = 0;
    m_pos_tail = 0;
    m_size = 0;
    m_state = 0;
}

/// Clear the buffer.
template <typename T, typename U, U S>
void Ringbuffer<T, U, S>::clear() volatile {
    m_pos_head = 0;
    m_pos_tail = 0;
    m_size = 0;
    m_state = 0;
}
