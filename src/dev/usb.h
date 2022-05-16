/**********************************************************************\
 * Keyboard
 *
 * STM32F401 USB register map
 **********************************************************************
 * Copyright (C) 2021-2022 - Max Maisel
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
    struct UsbOtgStruct {
        WORD GOTGCTL;
        WORD GOTGINT;
        WORD GAHBCFG;
        WORD GUSBCFG;
        WORD GRSTCTL;
        WORD GINTSTS;
        WORD GINTMSK;
        WORD GRXSTSR;
        WORD GRXSTSP;
        WORD GRXFSIZ;
        WORD DIEPTXF0_HNPTXFSIZ;
        WORD HNPTXSTS;
        DWORD : 64;
        WORD GCCFG;
        WORD CID;
        WORD  _reserved[48];
        WORD HPTXFSIZ;
        WORD DIEPTXF[16];
    };

    volatile UsbOtgStruct* const USB = (volatile UsbOtgStruct*)0x50000000;

    struct UsbDeviceStruct {
        WORD DCFG;
        WORD DCTL;
        WORD DSTS;
        WORD : 32;
        WORD DIEPMSK;
        WORD DOEPMSK;
        WORD DAINT;
        WORD DAINTMSK;
        DWORD : 64;
        WORD DVBUSDIS;
        WORD DVBUSPULSE;
        WORD DTHRCTL;
        WORD DIEPEMPMSK;
        WORD DEACHINT;
        WORD DEACHMSK;
        WORD : 32;
        WORD DINEP1MSK;
        WORD _reserved[15];
        WORD DOUTEP1MSK;
    };

    volatile UsbDeviceStruct* const USBDEV =
        (volatile UsbDeviceStruct*)0x50000800;

    struct UsbInEndpointStruct {
        WORD DIEPCTL;
        WORD : 32;
        WORD DIEPINT;
        WORD : 32;
        WORD DIEPTSIZ;
        WORD DIEPDMA;
        WORD DTXFSTS;
        WORD : 32;
    };

    // Endpoints are arrays of 4
    volatile UsbInEndpointStruct* const USB_INEP =
        (volatile UsbInEndpointStruct*)0x50000900;

    struct UsbOutEndpointStruct {
        WORD DOEPCTL;
        WORD : 32;
        WORD DOEPINT;
        WORD : 32;
        WORD DOEPTSIZ;
        WORD DOEPDMA;
        DWORD : 64;
    };

    // Endpoints are arrays of 4
    volatile UsbOutEndpointStruct* const USB_OUTEP =
        (volatile UsbOutEndpointStruct*)0x50000B00;

    volatile WORD* const USB_PCGCCTL = (volatile WORD*)0x50000E00;

    const WORD USB_OTG_FIFO_SIZE = 0x1000;

    struct UsbFifoStruct {
        WORD data;
        WORD _padding[USB_OTG_FIFO_SIZE/4-1];
    };

    volatile UsbFifoStruct* const USB_FIFO =
        (volatile UsbFifoStruct*)0x50001000;

    namespace usb {
        // GOTGINT
        enum : WORD {
            DBCDNE = (1 << 19),
            ADTOCHG = (1 << 18),
            HNGDET = (1 << 17),
            HNSSCHG = (1 << 9),
            SRSSCHG = (1 << 8),
            SEDET = (1 << 2)
        };

        // GAHBCFG
        enum : WORD {
            PTXFELVL = (1 << 8),
            TXFELVL = (1 << 7),
            GINTMSK = (1 << 0)
        };

        // GUSBCFG
        enum : WORD {
            CTXPKT = (1U << 31),
            FDMOD = (1 << 30),
            FHMOD = (1 << 29),
            TRDT_MASK = 0x00003C00,
            TRDT_POS = 10,
            HNPCAP = (1 << 9),
            SRPCAP = (1 << 8),
            PHYSEL = (1 << 6),
            TOCAL_MASK = 0x00000007,
            TOCAL_POS = 0
        };

        // GRSTCTL
        enum : WORD {
            AHBIDL = (1U << 31),
            GTXFNUM_MASK = 0x000007C0,
            GTXFNUM_POS = 6,
            TXFFLSH = (1 << 5),
            RXFFLSH = (1 << 4),
            FCRST = (1 << 2),
            HSRST = (1 << 1),
            CSRST = (1 << 0)
        };

        // GINTSTS | GINTMSK
        enum : WORD {
            WKUPINT = (1U << 31),
            SRQINT = (1 << 30),
            DISCINT = (1 << 29),
            CIDSCHG = (1 << 28),
            PTXFE = (1 << 26),
            HCINT = (1 << 25),
            HPRTINT = (1 << 24),
            IPXFR = (1 << 21),
            IISOIXFR = (1 << 20),
            OEPINT = (1 << 19),
            IEPINT = (1 << 18),
            EOPF = (1 << 15),
            ISOODRP = (1 << 14),
            ENUMDNE = (1 << 13),
            USBRST = (1 << 12),
            USBSUSP = (1 << 11),
            ESUSP = (1 << 10),
            GONAKEFF = (1 << 7),
            GINAKEFF = (1 << 6),
            NPTXFE = (1 << 5),
            RXFLVL = (1 << 4),
            SOF = (1 << 3),
            OTGINT = (1 << 2),
            MMIS = (1 << 1),
            CMOD = (1 << 0)
        };

        // GRXSTSR | GRXSTSP
        enum : WORD {
            // Host
            PKTSTS_MASK = 0x001E0000,
            PKTSTS_POS = 17,
            CHNUM_MASK = 0x0000000F,
            CHNUM_POS = 0,
            // Device
            FRMNUM_MASK = 0x01700000,
            FRMNUM_POS = 21,
            EPNUM_MASK = 0x0000000F,
            EPNUM_POS = 0,
            // Common
            DPID_MASK = 0x00018000,
            DPID_POS = 15,
            DPID_DATA0 = (0 << 15),
            DPID_DATA1 = (2 << 15),
            DPID_DATA2 = (1 << 15),
            DPID_MDATA = (3 << 15),
            BCNT_MASK = 0x00007FF0,
            BCNT_POS = 4
        };

        // GRXFSIZ
        enum : WORD {
            RXFD_MASK = 0x0000FFFF,
            RXFD_POS = 0
        };

        // HNPTXFSIZ | DIEPTXF0
        enum : WORD {
            // Host
            NPTXFD_MASK = 0xFFFF0000,
            NPTXFD_POS = 16,
            NPTXFSA_MASK = 0x0000FFFF,
            NPTXFSA_POS = 0,
            // Device
            TX0FD_MASK = 0xFFFF0000,
            TX0FD_POS = 16,
            TX0FSA_MASK = 0x0000FFFF,
            TX0FSA_POS = 0
        };

        // GCCFG
        enum : WORD {
            NOVBUSSENS = (1 << 21),
            SOFOUTEN = (1 << 20),
            VBUSBSEN = (1 << 19),
            VBUSASOM = (1 << 18),
            PWRDWN = (1 << 16)
        };

        // DIEPTXFn
        enum : WORD {
            INEPTXFD_MASK = 0xFFFF0000,
            INEPTXFD_POS = 16,
            INEPTXSA_MASK = 0x0000FFFF,
            INEPTXSA_POS = 0
        };

        // DCFG
        enum : WORD {
            PFIVL_MASK = 0x00001800,
            PFIVL_80 = (0 << 11),
            PFIVL_85 = (1 << 11),
            PFIVL_90 = (2 << 11),
            PFIVL_95 = (3 << 11),
            DADR_MASK = 0x000007F0,
            DADR_POS = 4,
            NZLSOHSK = (1 << 2),
            DSPD_FULL = (3 << 0)
        };

        // DCTL
        enum : WORD {
            POPRGDNE = (1 << 11),
            CGOANK = (1 << 10),
            SGONAK = (1 << 9),
            CGINAK = (1 << 8),
            SGINAK = (1 << 7),
            TCTL_MASK = 0x00000070,
            TCTL_J = (1 << 4),
            TCTL_K = (2 << 4),
            TCTL_SE0_NAK = (3 << 4),
            TCTL_PKT = (4 << 4),
            TCTL_FORCE_EN = (5 << 4),
            GONSTS = (1 << 3),
            GINSTS = (1 << 2),
            SDIS = (1 << 1),
            RWUSIG = (1 << 0)
        };

        // DSTS
        enum : WORD {
            FNSOF_MASK = 0x003FFF00,
            FNSOF_POS = 8,
            EERR = (1 << 3),
            ENUMSPD_MASK = 0x00000006,
            ENUMSPD_FULL = 0x00000006,
            SUSPSTS = (1 << 0)
        };

        // DIEPMSK | DOEPMSK | DIEPINT | DOEPINT
        enum : WORD {
            // Common
            NAK = (1 << 13),
            EPDISINT = (1 << 1),
            XFRC = (1 << 0),
            // DOEPMSK
            BERR = (1 << 12),
            OUTPKTERR = (1 << 8),
            STSPHSRX = (1 << 5),
            OTEPDIS = (1 << 4),
            STUP = (1 << 3),
            // DIEPMSK
            INEPNE = (1 << 6),
            INEPNM = (1 << 5),
            ITTXFE = (1 << 4),
            TOC = (1 << 3),
            // DIEPINT
            PKTDRPSTS = (1 << 11),
            TXFE = (1 << 7),
        };

        // DAINT | DAINTMSK
        enum : WORD {
            OEPINT_MASK = 0xFFFF0000,
            OEPINT_POS = 16,
            IEPINT_MASK = 0x0000FFFF,
            IEPINT_POS = 0
        };

        // DVBUSDIS
        enum : WORD {
            VBUSDT_MASK = 0x0000FFFF,
            VBUSDT_POS = 0
        };

        // DVBUSPULSE
        enum : WORD {
            DVBUSP_MASK = 0x00000FFF,
            DVBUS_POS = 0
        };

        // DIEPEMPMSK
        enum : WORD {
            INEPTXFEM_MASK = 0x0000FFFF,
            INEPTXFEM_POS = 0,
        };

        // DIEPCTLx | DOEPCTLx
        enum : WORD {
            EPENA = (1U << 31),
            EPDIS = (1 << 30),
            SD1PID = (1 << 29),
            SODDFRM = (1 << 29),
            SD0PID = (1 << 28),
            SEVNFRM = (1 << 28),
            SNAK = (1 << 27),
            CNAK = (1 << 26),
            TXFNUM_MASK = 0x03C00000,
            TXFNUM_POS = 22,
            STALL = (1 << 21),
            SNPM = (1 << 20),
            EPTYP_MASK = 0x000C0000,
            EPTYP_CTRL = (0 << 18),
            EPTYP_ISO = (1 << 18),
            EPTYP_BULK = (2 << 18),
            EPTYP_INT = (3 << 18),
            NAKSTS = (1 << 17),
            EONUM = (1 << 16),
            DPID = (1 << 16),
            USBAEP = (1 << 15),
            MPSIZ_MASK = 0x000003FF,
            MPSIZ_POS = 0,
            // EP0
            MPSIZ0_MASK = 0x00000003,
            MPSIZ0_64 = (0 << 0),
            MPSIZ0_32 = (1 << 0),
            MPSIZ0_16 = (2 << 0),
            MPSIZ0_8 = (3 << 0)
        };

        // DIEPTSIZx | DOEPTSIZx
        enum : WORD {
            // DOEPTSIZ0
            STUPCNT_MASK = 0x60000000,
            STUPCNT_POS = 29,
            PKTCNTO_MASK = 0x00180000,
            // DIEPTSIZ0
            PKTCNTI_MASK = 0x00080000,
            // DxEPTSIZ0
            XFRSIZ0_MASK = 0x0000007F,
            // DOEPTSIZx
            RXDPID_MASK = 0x60000000,
            RXPID_DATA0 = (0 << 29),
            RXPID_DATA2 = (1 << 29),
            RXPID_DATA1 = (2 << 29),
            RXPID_MDATA = (3 << 29),
            // DxEPTSIZx
            PKTCNT_MASK = 0x1FF80000,
            PKTCNT_POS = 19,
            XFRSIZ_MASK = 0x0007FFFF,
            XFRSIZ_POS = 0
        };

        // DTXFSTSx
        enum : WORD {
            INEPTFSAV_MASK = 0x0000FFFF,
            INEPTFSAVE_POS = 0
        };

        // PCGCCTL
        enum : WORD {
            PHYSUSP = (1 << 4),
            GATEHCLK = (1 << 1),
            STPPCLK = (1 << 0)
        };
    }
}
