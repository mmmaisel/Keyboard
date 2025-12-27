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
#pragma once

#include "types.h"

#include "event.h"
#include "hid_keyboard_endpoint.h"
#include "key_layout.h"

class UsbKeyboard : public EventSink {
    public:
        UsbKeyboard();

        static const BYTE BUFFER_SIZE = 16;

        virtual void on_event(Event* event) override;

        BYTE get_report_from_isr(
            HidKeyboardReport* report, BaseType_t* task_woken);

    private:
        BYTE fill_report(HidKeyboardReport* report);
        BYTE handle_fn(HidKeyboardReport* report);
        void replace_keys(HidKeyboardReport* report, BYTE new_key);

        DWORD _pages[PAGE_COUNT];
        // Queue for requesting state by USB host
        QueueHandle_t _queue;
        StaticQueue_t _queue_mem;
        HidKeyboardReport _queue_items;
};

extern UsbKeyboard usb_keyboard;
