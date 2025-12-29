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
#include "event.h"

#include "effect.h"

QueueHandle_t EventDispatcher::_queue = nullptr;
StaticQueue_t EventDispatcher::_queue_mem = {};
Event EventDispatcher::_queue_items[EventDispatcher::QUEUE_LEN] = {};

void EventDispatcher::initialize() {
    _queue = xQueueCreateStatic(QUEUE_LEN, sizeof(Event),
        reinterpret_cast<BYTE*>(&_queue_items), &_queue_mem);
#ifdef DEBUG
    vQueueAddToRegistry(_queue, "ModKbd");
#endif
}

void EventDispatcher::send(Event* event) {
    xQueueSend(_queue, event, portMAX_DELAY);
}

void EventDispatcher::send_from_isr(Event* event, BaseType_t* task_woken) {
    xQueueSendFromISR(_queue, event, task_woken);
}

void EventDispatcher::next_event(Event* event) {
    while(xQueueReceive(_queue, event, portMAX_DELAY) != pdTRUE);

    switch(event->type) {
        case EVENT_KEYS:
            EffectController::on_keys(event->keys.page, event->keys.state);
            break;
        case EVENT_EFFECT:
            EffectController::set_effect(
                EffectController::effect_by_id(event->effect.id)
            );
            break;
    }
}

void EventSink::task(void* pContext) {
    EventSink* sink = reinterpret_cast<EventSink*>(pContext);
    for(;;) {
        EventDispatcher::next_event(&sink->_event);
        sink->on_event(&sink->_event);
    }
}
