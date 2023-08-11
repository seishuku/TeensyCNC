/* Teensyduino Core Library
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2017 PJRC.COM, LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * 2. If the Software is incorporated into a build system that allows
 * selection among a list of target devices, then similar target
 * devices manufactured by PJRC.COM must be included in the list of
 * target devices and selectable in the same manner.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once
// This header is NOT meant to be included when compiling
// user sketches in Arduino.  The low-level functions
// provided by usb_dev.c are meant to be called only by
// code which provides higher-level interfaces to the user.

#include <stdint.h>
#include <stddef.h>

#define ENDPOINT_TRANSMIT_UNUSED	0x00020000
#define ENDPOINT_TRANSMIT_ISOCHRONOUS	0x00C40000
#define ENDPOINT_TRANSMIT_BULK		0x00C80000
#define ENDPOINT_TRANSMIT_INTERRUPT	0x00CC0000
#define ENDPOINT_RECEIVE_UNUSED		0x00000002
#define ENDPOINT_RECEIVE_ISOCHRONOUS	0x000000C4
#define ENDPOINT_RECEIVE_BULK		0x000000C8
#define ENDPOINT_RECEIVE_INTERRUPT	0x000000CC

/*
Each group of #define lines below corresponds to one of the
settings in the Tools > USB Type menu.  This file defines what
type of USB device is actually created for each of those menu
options.

Each "interface" is a set of functionality your PC or Mac will
use and treat as if it is a unique device.  Within each interface,
the "endpoints" are the actual communication channels.  Most
interfaces use 1, 2 or 3 endpoints.  By editing only this file,
you can customize the USB Types to be any collection of interfaces.

To modify a USB Type, delete the XYZ_INTERFACE lines for any
interfaces you wish to remove, and copy them from another USB Type
for any you want to add.

Give each interface a unique number, and edit NUM_INTERFACE to
reflect the total number of interfaces.

Next, assign unique endpoint numbers to all the endpoints across
all the interfaces your device has.  You can reuse an endpoint
number for transmit and receive, but the same endpoint number must
not be used twice to transmit, or twice to receive.

Most endpoints also require their maximum size, and some also
need an interval specification (the number of milliseconds the
PC will check for data from that endpoint).  For existing
interfaces, usually these other settings should not be changed.

Edit NUM_ENDPOINTS to be at least the largest endpoint number used.

Edit the ENDPOINT*_CONFIG lines so each endpoint is configured
the proper way (transmit, receive, or both).

If you are using existing interfaces (making your own device with
a different set of interfaces) the code in all other files should
automatically adapt to the new endpoints you specify here.

If you need to create a new type of interface, you'll need to write
the code which sends and receives packets, and presents an API to
the user.  Usually, a pair of files are added for the actual code,
and code is also added in usb_dev.c for any control transfers,
interrupt-level code, or other very low-level stuff not possible
from the packet send/receive functons.  Code also is added in
usb_inst.c to create an instance of your C++ object.  This message
gives a quick summary of things you will need to know:
https://forum.pjrc.com/threads/49045?p=164512&viewfull=1#post164512

You may edit the Vendor and Product ID numbers, and strings.  If
the numbers are changed, Teensyduino may not be able to automatically
find and reboot your board when you click the Upload button in
the Arduino IDE.  You will need to press the Program button on
Teensy to initiate programming.

Some operating systems, especially Windows, may cache USB device
info.  Changes to the device name may not update on the same
computer unless the vendor or product ID numbers change, or the
"bcdDevice" revision code is increased.

If these instructions are missing steps or could be improved, please
let me know?  http://forum.pjrc.com/forums/4-Suggestions-amp-Bug-Reports
*/

#define USB_SERIAL
#if defined(USB_SERIAL)
#define VENDOR_ID				0x16C0
#define PRODUCT_ID				0x0483
#define MANUFACTURER_NAME		{ 'T', 'e', 'e', 'n', 's', 'y', 'd', 'u', 'i', 'n', 'o' }
#define MANUFACTURER_NAME_LEN	11
#define PRODUCT_NAME			{ 'U', 'S', 'B', ' ', 'S', 'e', 'r', 'i', 'a', 'l' }
#define PRODUCT_NAME_LEN		10
#define EP0_SIZE				64
#define NUM_ENDPOINTS			4
#define NUM_USB_BUFFERS			12
#define NUM_INTERFACE			3
#define CDC_IAD_DESCRIPTOR		1       // Serial
#define CDC_STATUS_INTERFACE	0
#define CDC_DATA_INTERFACE		1
#define CDC_ACM_ENDPOINT		2
#define CDC_RX_ENDPOINT			3
#define CDC_TX_ENDPOINT			4
#define CDC_ACM_SIZE			16
#define CDC_RX_SIZE_480			512
#define CDC_TX_SIZE_480			512
#define CDC_RX_SIZE_12			64
#define CDC_TX_SIZE_12			64
#define EXPERIMENTAL_INTERFACE	2
#define ENDPOINT2_CONFIG		ENDPOINT_RECEIVE_UNUSED+ENDPOINT_TRANSMIT_INTERRUPT
#define ENDPOINT3_CONFIG		ENDPOINT_RECEIVE_BULK+ENDPOINT_TRANSMIT_UNUSED
#define ENDPOINT4_CONFIG		ENDPOINT_RECEIVE_UNUSED+ENDPOINT_TRANSMIT_BULK
#endif

#ifdef USB_DESC_LIST_DEFINE
#if defined(NUM_ENDPOINTS)&&(NUM_ENDPOINTS>0)

// NUM_ENDPOINTS = number of non-zero endpoints (0 to 7)
extern const uint32_t usb_endpoint_config_table[NUM_ENDPOINTS];

typedef struct
{
	uint16_t wValue, wIndex;
	const uint8_t *addr;
	uint16_t length;
} usb_descriptor_list_t;

extern const usb_descriptor_list_t usb_descriptor_list[];

#endif // NUM_ENDPOINTS
#endif // USB_DESC_LIST_DEFINE
