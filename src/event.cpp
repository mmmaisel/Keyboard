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

EventQueue::EventQueue() {
    _queue = xQueueCreateStatic(1, sizeof(Event),
        reinterpret_cast<BYTE*>(&_queue_items), &_queue_mem);
#ifdef DEBUG
    vQueueAddToRegistry(_queue, "ModKbd");
#endif
}

void EventQueue::send_from_isr(Event* event) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR(_queue, event, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

BYTE EventQueue::recv(Event* event) {
    return xQueueReceive(_queue, event, portMAX_DELAY);
}
