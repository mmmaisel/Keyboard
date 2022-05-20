/**********************************************************************\
 * Keyboard
 *
 * ARM M4 DMA register map
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

#include "../types.h"

namespace dev {
    namespace dma {
        // (L|H)(ISR|FCR)
        enum : WORD {
            TCIF = (1 << 5),
            HTIF = (1 << 4),
            TEIF = (1 << 3),
            DMEIF = (1 << 2),
            FEIF = (1 << 0),

            STREAM0_POS = 0,
            STREAM1_POS = 6,
            STREAM2_POS = 16,
            STREAM3_POS = 22,
            STREAM4_POS = 0,
            STREAM5_POS = 6,
            STREAM6_POS = 16,
            STREAM7_POS = 22
        };

        // SxCR
        enum : WORD {
            CHSEL_MASK = 0x0E000000,
            CHSEL_POS = 25,
            MBURST_MASK = 0x01800000,
            MBURST_POS = 23,
            PBURST_MASK = 0x00600000,
            PBURST_POS = 21,
            CT = (1 << 19),
            DBM = (1 << 18),
            PL_MASK = 0x00030000,
            PL_POS = 16,
            PINCOS = (1 << 15),
            MSIZE_MASK = 0x00006000,
            MSIZE_POS = 13,
            PSIZE_MASK = 0x00001800,
            PSIZE_POS = 11,
            MINC = (1 << 10),
            PINC = (1 << 9),
            CIRC = (1 << 8),
            DIR_MASK = 0x000000C0,
            DIR_P2M = (0 << 6),
            DIR_M2P = (1 << 6),
            DIR_M2M = (2 << 6),
            PFCTRL = (1 << 5),
            TCIE = (1 << 4),
            HTIE = (1 << 3),
            TEIE = (1 << 2),
            DMEIE = (1 << 1),
            EN = (1 << 0)
        };

        // SxNTDR
        enum : WORD {
            NDT_MASK = 0x0000FFFF,
            NDT_POS = 0
        };

        // SxFCR
        enum : WORD {
            FEIE = (1 << 7),
            FS_MASK = 0x00000038,
            FS_POS = 3,
            DMDIS = (1 << 2),
            FTH_MASK = 0x00000003,
            FTH_POS = 0
        };
    }

    struct DmaStreamStruct {
        WORD CR;
        WORD NDTR;
        WORD PAR;
        WORD M0AR;
        WORD M1AR;
        WORD FCR;
    };

    struct DmaStruct {
        WORD LISR;
        WORD HISR;
        WORD LIFCR;
        WORD HIFCR;
        DmaStreamStruct STREAM[8];

        inline void clear_isr(BYTE stream) volatile {
            using namespace dma;
            switch(stream) {
                case 0:
                    LIFCR = (TCIF << STREAM0_POS);
                    break;
                case 1:
                    LIFCR = (TCIF << STREAM1_POS);
                    break;
                case 2:
                    LIFCR = (TCIF << STREAM2_POS);
                    break;
                case 3:
                    LIFCR = (TCIF << STREAM3_POS);
                    break;
                case 4:
                    HIFCR = (TCIF << STREAM4_POS);
                    break;
                case 5:
                    HIFCR = (TCIF << STREAM5_POS);
                    break;
                case 6:
                    HIFCR = (TCIF << STREAM6_POS);
                    break;
                case 7:
                    HIFCR = (TCIF << STREAM7_POS);
                    break;
                default:
                    asm volatile(" svc #1");
                    break;
            }
        }
    };

    volatile DmaStruct* const DMA1 = (volatile DmaStruct*)0x40026000;
    volatile DmaStruct* const DMA2 = (volatile DmaStruct*)0x40026400;
}
