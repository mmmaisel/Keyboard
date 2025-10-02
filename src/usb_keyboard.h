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

class UsbKeyboard : public EventSink {
    public:
        static const BYTE BUFFER_SIZE = 16;
        static const BYTE PAGE_COUNT = 4;

        virtual void on_event(Event* event) override;

        //static void send_page(KeyMatrix::Page* page);
        //static void send_page_from_isr(KeyMatrix::Page* page);

        void get_keys(BYTE* buffer);
        //static void set_led(const LedMatrix::Led& led);

    private:
        DWORD _pages[PAGE_COUNT];

        //static void update_keys();
};

extern UsbKeyboard usb_keyboard;
