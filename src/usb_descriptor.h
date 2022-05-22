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
#pragma once

#include "types.h"

enum {
    DESCRIPTOR_TYPE_DEVICE = 1,
    DESCRIPTOR_TYPE_CONFIG = 2,
    DESCRIPTOR_TYPE_STRING = 3,
    DESCRIPTOR_TYPE_INTERFACE = 4,
    DESCRIPTOR_TYPE_ENDPOINT = 5,
    DESCRIPTOR_TYPE_INTERFACE_ASSOC = 11,
    DESCRIPTOR_TYPE_HID = 33,
    DESCRIPTOR_TYPE_HID_REPORT = 34,
    DESCRIPTOR_TYPE_CS_INTERFACE = 36
};

enum {
    CLASS_DEVICE = 0,
    CLASS_COMM = 2,
    CLASS_HID = 3,
    CLASS_CDC_DATA = 10,
    SUBCLASS_DEVICE = 0,
    SUBCLASS_HID_BOOT = 1,
    SUBCLASS_COMM_ACM = 2,
    PROTOCOL_DEVICE = 0,
    PROTOCOL_ACM = 1,
    PROTOCOL_HID_KEYBOARD = 1,

    CLASS_MISC = 254,
    SUBCLASS_MISC_COMMON = 2,
    PROTOCOL_INTERFACE_ASSOC = 1
};

enum {
    ENDPOINT_DIR_OUT = 0,
    ENDPOINT_DIR_IN = 0x80,

    ENDPOINT_MODE_BULK = 2,
    ENDPOINT_MODE_INTERRUPT = 3
};

struct __attribute__((__packed__)) DeviceDescriptor {
    BYTE bLength;
    BYTE bDescriptorType;
    HWORD bcdUSB;
    BYTE bDeviceClass;
    BYTE bDeviceSubClass;
    BYTE bDeviceProtocol;
    BYTE bMaxPacketSize;
    HWORD idVendor;
    HWORD idProduct;
    HWORD bcdDevice;
    BYTE iManufacturer;
    BYTE iProduct;
    BYTE iSerialNumber;
    BYTE bNumConfigurations;
};

struct __attribute__((__packed__)) StringDescriptor {
    BYTE bLength;
    BYTE bDescriptorType;
};

struct __attribute__((__packed__)) EndpointDescriptor {
    BYTE bLength;
    BYTE bDescriptorType;
    BYTE bEndpointAddress;
    BYTE bmAttributes;
    HWORD wMaxPacketSize;
    BYTE bInterval;
};

struct __attribute__((__packed__)) InterfaceDescriptor {
    BYTE bLength;
    BYTE bDescriptorType;
    BYTE bInterfaceNumber;
    BYTE bAlternateSetting;
    BYTE bNumEndpoints;
    BYTE bInterfaceClass;
    BYTE bInterfaceSubClass;
    BYTE bInterfaceProtocol;
    BYTE iInterface;
};

struct __attribute__((__packed__)) InterfaceAssociationDescriptor {
    BYTE bLength;
    BYTE bDescriptorType;
    BYTE bFirstInterface;
    BYTE bInterfaceCount;
    BYTE bFunctionClass;
    BYTE bFunctionSubclass;
    BYTE bFunctionProtocol;
    BYTE iFunction;
};

struct __attribute__((__packed__)) ConfigDescriptor {
    BYTE blength;
    BYTE bDescriptorType;
    HWORD wTotalLength;
    BYTE bNumInterfaces;
    BYTE bConfigurationValue;
    BYTE iConfiguration;
    BYTE bmAttributes;
    BYTE bMaxPower;
};

struct __attribute__((__packed__)) HidDescriptor {
    BYTE bLength;
    BYTE bDescriptorType;
    HWORD bcdHID;
    BYTE bCountryCode;
    BYTE bNumDescriptors;
    BYTE bDescriptorType2;
    HWORD wItemLength;
};

struct __attribute__((__packed__)) HidReportDescriptor0 {
    BYTE report[64];
};

struct __attribute__((__packed__)) KeyboardBootHidReport {
    BYTE modifiers;
    BYTE _padding;
    BYTE keyId[6];
};

struct __attribute__((__packed__)) CdcHeaderDescriptor {
    BYTE bLength;
    BYTE bDescriptorType;
    BYTE bDescriptorSubtype;
    HWORD bcdCDC;
};

struct __attribute__((__packed__)) CdcAcmDescriptor {
    BYTE bLength;
    BYTE bDescriptorType;
    BYTE bDescriptorSubtype;
    BYTE bmCapabilities;
};

struct __attribute__((__packed__)) CdcUnionDescriptor {
    BYTE bLength;
    BYTE bDescriptorType;
    BYTE bDescriptorSubtype;
    BYTE bControlInterface;
    BYTE bSubordinateInterface;
};

struct __attribute__((__packed__)) CdcManagementDescriptor {
    BYTE bLength;
    BYTE bDescriptorType;
    BYTE bDescriptorSubtype;
    BYTE bmCapabilities;
    BYTE bDataInterface;
};

struct __attribute__((__packed__)) ConfigDescriptor0 {
    ConfigDescriptor config;
    InterfaceDescriptor interface;
    HidDescriptor hid;
    EndpointDescriptor ep;
};

struct __attribute__((__packed__)) StringDescriptorLang {
    StringDescriptor descriptor;
    HWORD languages[1];
};

struct __attribute__((__packed__)) StringDescriptorManf {
    StringDescriptor descriptor;
    char16_t string[10];
};

struct __attribute__((__packed__)) StringDescriptorProduct {
    StringDescriptor descriptor;
    char16_t string[8];
};

struct __attribute__((__packed__)) StringDescriptorSerial {
    StringDescriptor descriptor;
    char16_t string[8];
};

struct Descriptor {
    USHORT length;
    BYTE type;
    BYTE* data;
};

const Descriptor* get_usb_descriptor(BYTE type, BYTE index);
