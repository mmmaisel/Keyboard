/**********************************************************************\
 * Keyboard
 *
 * USB descriptors
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
#include "usb_descriptor.h"

// XXX: add PROGMEM support

DeviceDescriptor DEVICE_DESCRIPTOR = {
    sizeof(DeviceDescriptor),
    DESCRIPTOR_TYPE_DEVICE,
    0x200,
    CLASS_DEVICE,
    SUBCLASS_DEVICE,
    PROTOCOL_DEVICE,
    64,
    0xDEAD,
    0xBEEF,
    0x100,
    1,
    2,
    3,
    1
};

ConfigDescriptor0 CONFIG_DESCRIPTOR = {
    sizeof(ConfigDescriptor),
    DESCRIPTOR_TYPE_CONFIG,
    sizeof(ConfigDescriptor0),
    1,
    1,
    0,
    0x80,
    250,
    {
        sizeof(InterfaceDescriptor),
        DESCRIPTOR_TYPE_INTERFACE,
        0,
        0,
        1,
        CLASS_HID,
        SUBCLASS_HID_BOOT,
        PROTOCOL_HID_KEYBOARD,
        0
    },
    {
        sizeof(HidDescriptor),
        DESCRIPTOR_TYPE_HID,
        0x0110,
        9,
        1,
        DESCRIPTOR_TYPE_HID_REPORT,
        sizeof(HidReportDescriptor0)
    },
    {
        sizeof(EndpointDescriptor),
        DESCRIPTOR_TYPE_ENDPOINT,
        1 | ENDPOINT_DIR_IN,
        ENDPOINT_MODE_INTERRUPT,
        64,
        10
    }
};

StringDescriptorLang STR_LANG_DESCRIPTOR = {
    sizeof(StringDescriptorLang),
    DESCRIPTOR_TYPE_STRING,
    {
        0x0407  // German (Default)
    }
};

StringDescriptorManf STR_MANF_DESCRIPTOR = {
    sizeof(StringDescriptorManf),
    DESCRIPTOR_TYPE_STRING,
    { 'A', 'C', 'M', 'E', ' ', 'C', 'o', 'r', 'p', '.' }
};

StringDescriptorProduct STR_PRODUCT_DESCRIPTOR = {
    sizeof(StringDescriptorProduct),
    DESCRIPTOR_TYPE_STRING,
    { 'K', 'e', 'y', 'b', 'o', 'a', 'r', 'd' }
};

StringDescriptorSerial STR_SERIAL_DESCRIPTOR = {
    sizeof(StringDescriptorSerial),
    DESCRIPTOR_TYPE_STRING,
    { '0', '0', '0', '0', '0', '0', '0', '0' }
};

HidReportDescriptor0 HID_REPORT_DESCRIPTOR = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard Left Control)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right Super)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x81, 0x01,                    //   INPUT (Cnst,Ary,Abs)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right Super)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0x95, 0x03,                    //   REPORT_COUNT (3),
    0x75, 0x01,                    //   REPORT_SIZE (1),
    0x05, 0x08,                    //   USAGE_PAGE (LEDs),
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock),
    0x29, 0x03,                    //   USAGE_MAXIMUM (Scroll Lock),
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs),
    0x95, 0x05,                    //   REPORT_COUNT (5),
    0x75, 0x01,                    //   REPORT_SIZE (1),
    0x91, 0x01,                    //   OUTPUT (Cnst,Ary,Abs),
    0xc0                           // END_COLLECTION
};

// XXX: pad descriptors to word boundaries to avoid leaking memory
#define DESCRIPTOR_ENTRY(descriptor, type) \
    { sizeof(descriptor), DESCRIPTOR_TYPE_ ## type, reinterpret_cast<BYTE*>(&descriptor) }

static const Descriptor DESCRIPTORS[] = {
    // Descriptors of the same type must be ordered as they are
    // indexed like an array.
    DESCRIPTOR_ENTRY(DEVICE_DESCRIPTOR, DEVICE),
    DESCRIPTOR_ENTRY(CONFIG_DESCRIPTOR, CONFIG),
    DESCRIPTOR_ENTRY(STR_LANG_DESCRIPTOR, STRING),
    DESCRIPTOR_ENTRY(STR_MANF_DESCRIPTOR, STRING),
    DESCRIPTOR_ENTRY(STR_PRODUCT_DESCRIPTOR, STRING),
    DESCRIPTOR_ENTRY(STR_SERIAL_DESCRIPTOR, STRING),
    DESCRIPTOR_ENTRY(HID_REPORT_DESCRIPTOR, HID_REPORT)
};

const Descriptor* get_usb_descriptor(BYTE type, BYTE index) {
    BYTE i = 0;
    for(const auto& descriptor : DESCRIPTORS) {
        if(descriptor.type == type) {
            if(i++ == index)
                return &descriptor;
        }
        else
            i = 0;
    }
    return nullptr;
}
