/**********************************************************************\
 * Keyboard
 *
 * debug write only UART class
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
#include "dev/uart.h"

class SimpleUart
{
    SimpleUart(const SimpleUart&) = delete;
    SimpleUart(SimpleUart&&) = delete;

    public:
        SimpleUart(volatile dev::UsartStruct* usart);
        ~SimpleUart() {}

        inline void write(BYTE c)
        {
            using namespace dev;
            using namespace dev::usart;

            while(!(m_uart->SR & TXE));
            m_uart->DR = c;
        }

    private:
        volatile dev::UsartStruct* m_uart;
};

extern SimpleUart UART6;
