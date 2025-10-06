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

#include "types.h"

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/queue.h"

struct KeyEvent {
    BYTE page;
    DWORD state;
};

enum EventType {
    EVENT_KEYS = 1,
};

struct Event {
    BYTE type;
    union {
        KeyEvent keys;
    };
};

class EventDispatcher {
    // Static class
    EventDispatcher() = delete;
    EventDispatcher(const EventDispatcher&) = delete;
    EventDispatcher(EventDispatcher&&) = delete;
    ~EventDispatcher() = delete;

    public:
        static void initialize();
        static void send(Event* event);
        static void send_from_isr(Event* event, BaseType_t* task_woken);
        static void next_event(Event* event);

    private:
        static const BYTE QUEUE_LEN = 3;

        static QueueHandle_t _queue;
        static StaticQueue_t _queue_mem;
        static Event _queue_items[QUEUE_LEN];
};

class EventSink {
    public:
        EventSink() = default;
        EventSink(const EventSink&) = delete;
        EventSink(EventSink&&) = delete;

        virtual void on_event(Event* event) = 0;

        [[noreturn]] static void task(void* pContext);

    private:
        Event _event;
};
