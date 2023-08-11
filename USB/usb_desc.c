/* Teensyduino Core Library
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2019 PJRC.COM, LLC.
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

#define USB_DESC_LIST_DEFINE
#include "usb_desc.h"

#ifdef NUM_ENDPOINTS
#include "usb_names.h"
#include "MIMXRT1062.h"
#include <stdlib.h>

 // USB Descriptors are binary data which the USB host reads to
 // automatically detect a USB device's capabilities.  The format
 // and meaning of every field is documented in numerous USB
 // standards.  When working with USB descriptors, despite the
 // complexity of the standards and poor writing quality in many
 // of those documents, remember descriptors are nothing more
 // than constant binary data that tells the USB host what the
 // device can do.  Computers will load drivers based on this data.
 // Those drivers then communicate on the endpoints specified by
 // the descriptors.

 // To configure a new combination of interfaces or make minor
 // changes to existing configuration (eg, change the name or ID
 // numbers), usually you would edit "usb_desc.h".  This file
 // is meant to be configured by the header, so generally it is
 // only edited to add completely new USB interfaces or features.



 // **************************************************************
 //   USB Device
 // **************************************************************

#define LSB(n) ((n) & 255)
#define MSB(n) (((n) >> 8) & 255)

#ifdef CDC_IAD_DESCRIPTOR
#ifndef DEVICE_CLASS
#define DEVICE_CLASS 0xEF
#endif
#ifndef DEVICE_SUBCLASS
#define DEVICE_SUBCLASS 0x02
#endif
#ifndef DEVICE_PROTOCOL
#define DEVICE_PROTOCOL 0x01
#endif
#endif


// USB Device Descriptor.  The USB host reads this first, to learn
// what type of device is connected.
static uint8_t device_descriptor[]={
		18,                                     // bLength
		1,                                      // bDescriptorType
		0x00, 0x02,                             // bcdUSB
#ifdef DEVICE_CLASS
		DEVICE_CLASS,                           // bDeviceClass
#else
	0,
#endif
#ifdef DEVICE_SUBCLASS
		DEVICE_SUBCLASS,                        // bDeviceSubClass
#else
	0,
#endif
#ifdef DEVICE_PROTOCOL
		DEVICE_PROTOCOL,                        // bDeviceProtocol
#else
	0,
#endif
		EP0_SIZE,                               // bMaxPacketSize0
		LSB(VENDOR_ID), MSB(VENDOR_ID),         // idVendor
		LSB(PRODUCT_ID), MSB(PRODUCT_ID),       // idProduct
#ifdef BCD_DEVICE
	LSB(BCD_DEVICE), MSB(BCD_DEVICE),       // bcdDevice
#else
		0x79, 0x02, // Teensy 4.0
#endif
		1,                                      // iManufacturer
		2,                                      // iProduct
		3,                                      // iSerialNumber
		1                                       // bNumConfigurations
};

__attribute__((section(".flashmem"))) static const uint8_t qualifier_descriptor[]={	// 9.6.2 Device_Qualifier, page 264
	10,					// bLength
	6,					// bDescriptorType
	0x00, 0x02,				// bcdUSB
#ifdef DEVICE_CLASS
		DEVICE_CLASS,                           // bDeviceClass
#else
	0,
#endif
#ifdef DEVICE_SUBCLASS
		DEVICE_SUBCLASS,                        // bDeviceSubClass
#else
	0,
#endif
#ifdef DEVICE_PROTOCOL
		DEVICE_PROTOCOL,                        // bDeviceProtocol
#else
	0,
#endif
		EP0_SIZE,                               // bMaxPacketSize0
		1,					// bNumConfigurations
		0                                       // bReserved
};

// These descriptors must NOT be "const", because the USB DMA
// has trouble accessing flash memory with enough bandwidth
// while the processor is executing from flash.

#ifdef EXPERIMENTAL_INTERFACE
static uint8_t microsoft_os_string_desc[]={
	18, 3,
	'M', 0, 'S', 0, 'F', 0, 'T', 0, '1', 0, '0', 0, '0', 0,
	0xF8, 0  // GET_MS_DESCRIPTOR will use bRequest=0xF8
};
static uint8_t microsoft_os_compatible_id_desc[]={
	40, 0, 0, 0, // total length, 16 header + 24 function * 1
	0, 1, 4, 0,  // version 1.00, wIndex=4 (Compat ID)
	1, 0, 0, 0, 0, 0, 0, 0, // 1 function
	EXPERIMENTAL_INTERFACE, 1,
	'W','I','N','U','S','B',0,0, // compatibleID
	0,0,0,0,0,0,0,0,             // subCompatibleID
	0,0,0,0,0,0
};
#endif

// **************************************************************
//   USB Descriptor Sizes
// **************************************************************

// pre-compute the size and position of everything in the config descriptor
//
#define CONFIG_HEADER_DESCRIPTOR_SIZE	9

#define CDC_IAD_DESCRIPTOR_POS		CONFIG_HEADER_DESCRIPTOR_SIZE
#ifdef  CDC_IAD_DESCRIPTOR
#define CDC_IAD_DESCRIPTOR_SIZE		8
#else
#define CDC_IAD_DESCRIPTOR_SIZE		0
#endif

#define CDC_DATA_INTERFACE_DESC_POS	CDC_IAD_DESCRIPTOR_POS+CDC_IAD_DESCRIPTOR_SIZE
#ifdef  CDC_DATA_INTERFACE
#define CDC_DATA_INTERFACE_DESC_SIZE	9+5+5+4+5+7+9+7+7
#else
#define CDC_DATA_INTERFACE_DESC_SIZE	0
#endif

#define EXPERIMENTAL_INTERFACE_DESC_POS	CDC_DATA_INTERFACE_DESC_POS+CDC_DATA_INTERFACE_DESC_SIZE
#ifdef  EXPERIMENTAL_INTERFACE
#define EXPERIMENTAL_INTERFACE_DESC_SIZE 9+7+7
#define EXPERIMENTAL_HID_DESC_OFFSET	CDC_DATA_INTERFACE_DESC_POS+9
#else
#define EXPERIMENTAL_INTERFACE_DESC_SIZE 0
#endif

#define CONFIG_DESC_SIZE		EXPERIMENTAL_INTERFACE_DESC_POS+EXPERIMENTAL_INTERFACE_DESC_SIZE

// **************************************************************
//   USB Configuration
// **************************************************************

// USB Configuration Descriptor.  This huge descriptor tells all
// of the devices capabilities.

__attribute__((section(".flashmem"))) const uint8_t usb_config_descriptor_480[CONFIG_DESC_SIZE]={
	// configuration descriptor, USB spec 9.6.3, page 264-266, Table 9-10
	9,                                      // bLength;
	2,                                      // bDescriptorType;
	LSB(CONFIG_DESC_SIZE),                 // wTotalLength
	MSB(CONFIG_DESC_SIZE),
	NUM_INTERFACE,                          // bNumInterfaces
	1,                                      // bConfigurationValue
	0,                                      // iConfiguration
	0xC0,                                   // bmAttributes
	50,                                     // bMaxPower

#ifdef CDC_IAD_DESCRIPTOR
	// interface association descriptor, USB ECN, Table 9-Z
	8,                                      // bLength
	11,                                     // bDescriptorType
	CDC_STATUS_INTERFACE,                   // bFirstInterface
	2,                                      // bInterfaceCount
	0x02,                                   // bFunctionClass
	0x02,                                   // bFunctionSubClass
	0x01,                                   // bFunctionProtocol
	0,                                      // iFunction
#endif

#ifdef CDC_DATA_INTERFACE
	// configuration for 480 Mbit/sec speed
		// interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
		9,                                      // bLength
		4,                                      // bDescriptorType
		CDC_STATUS_INTERFACE,			// bInterfaceNumber
		0,                                      // bAlternateSetting
		1,                                      // bNumEndpoints
		0x02,                                   // bInterfaceClass
		0x02,                                   // bInterfaceSubClass
		0x01,                                   // bInterfaceProtocol
		0,                                      // iInterface
		// CDC Header Functional Descriptor, CDC Spec 5.2.3.1, Table 26
		5,                                      // bFunctionLength
		0x24,                                   // bDescriptorType
		0x00,                                   // bDescriptorSubtype
		0x10, 0x01,                             // bcdCDC
		// Call Management Functional Descriptor, CDC Spec 5.2.3.2, Table 27
		5,                                      // bFunctionLength
		0x24,                                   // bDescriptorType
		0x01,                                   // bDescriptorSubtype
		0x01,                                   // bmCapabilities
		1,                                      // bDataInterface
		// Abstract Control Management Functional Descriptor, CDC Spec 5.2.3.3, Table 28
		4,                                      // bFunctionLength
		0x24,                                   // bDescriptorType
		0x02,                                   // bDescriptorSubtype
		0x06,                                   // bmCapabilities
		// Union Functional Descriptor, CDC Spec 5.2.3.8, Table 33
		5,                                      // bFunctionLength
		0x24,                                   // bDescriptorType
		0x06,                                   // bDescriptorSubtype
		CDC_STATUS_INTERFACE,                   // bMasterInterface
		CDC_DATA_INTERFACE,                     // bSlaveInterface0
		// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
		7,                                      // bLength
		5,                                      // bDescriptorType
		CDC_ACM_ENDPOINT|0x80,                // bEndpointAddress
		0x03,                                   // bmAttributes (0x03=intr)
		LSB(CDC_ACM_SIZE),MSB(CDC_ACM_SIZE),    // wMaxPacketSize
		5,                                      // bInterval
		// interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
		9,                                      // bLength
		4,                                      // bDescriptorType
		CDC_DATA_INTERFACE,                     // bInterfaceNumber
		0,                                      // bAlternateSetting
		2,                                      // bNumEndpoints
		0x0A,                                   // bInterfaceClass
		0x00,                                   // bInterfaceSubClass
		0x00,                                   // bInterfaceProtocol
		0,                                      // iInterface
		// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
		7,                                      // bLength
		5,                                      // bDescriptorType
		CDC_RX_ENDPOINT,                        // bEndpointAddress
		0x02,                                   // bmAttributes (0x02=bulk)
		LSB(CDC_RX_SIZE_480),MSB(CDC_RX_SIZE_480),// wMaxPacketSize
		0,                                      // bInterval
		// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
		7,                                      // bLength
		5,                                      // bDescriptorType
		CDC_TX_ENDPOINT|0x80,                 // bEndpointAddress
		0x02,                                   // bmAttributes (0x02=bulk)
		LSB(CDC_TX_SIZE_480),MSB(CDC_TX_SIZE_480),// wMaxPacketSize
		0,                                      // bInterval
#endif // CDC_DATA_INTERFACE

#ifdef EXPERIMENTAL_INTERFACE
		// configuration for 480 Mbit/sec speed
			// interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
			9,                                      // bLength
			4,                                      // bDescriptorType
			EXPERIMENTAL_INTERFACE,                 // bInterfaceNumber
			0,                                      // bAlternateSetting
			2,                                      // bNumEndpoints
			0xFF,                                   // bInterfaceClass (0xFF = Vendor)
			0x6A,                                   // bInterfaceSubClass
			0xC7,                                   // bInterfaceProtocol
			0,                                      // iInterface
			// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
			7,                                      // bLength
			5,                                      // bDescriptorType
			1|0x80,                               // bEndpointAddress
			0x02,                                   // bmAttributes (0x02=bulk)
			LSB(512), MSB(512),                     // wMaxPacketSize
			1,                                      // bInterval
			// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
			7,                                      // bLength
			5,                                      // bDescriptorType
			1,                                      // bEndpointAddress
			0x02,                                   // bmAttributes (0x02=bulk)
			LSB(512), MSB(512),                     // wMaxPacketSize
			1,                                      // bInterval
		#endif // EXPERIMENTAL_INTERFACE
};


__attribute__((section(".flashmem"))) const uint8_t usb_config_descriptor_12[CONFIG_DESC_SIZE]={
	// configuration descriptor, USB spec 9.6.3, page 264-266, Table 9-10
	9,                                      // bLength;
	2,                                      // bDescriptorType;
	LSB(CONFIG_DESC_SIZE),                 // wTotalLength
	MSB(CONFIG_DESC_SIZE),
	NUM_INTERFACE,                          // bNumInterfaces
	1,                                      // bConfigurationValue
	0,                                      // iConfiguration
	0xC0,                                   // bmAttributes
	50,                                     // bMaxPower

#ifdef CDC_IAD_DESCRIPTOR
	// interface association descriptor, USB ECN, Table 9-Z
	8,                                      // bLength
	11,                                     // bDescriptorType
	CDC_STATUS_INTERFACE,                   // bFirstInterface
	2,                                      // bInterfaceCount
	0x02,                                   // bFunctionClass
	0x02,                                   // bFunctionSubClass
	0x01,                                   // bFunctionProtocol
	0,                                      // iFunction
#endif

#ifdef CDC_DATA_INTERFACE
	// configuration for 12 Mbit/sec speed
		// interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
		9,                                      // bLength
		4,                                      // bDescriptorType
		CDC_STATUS_INTERFACE,			// bInterfaceNumber
		0,                                      // bAlternateSetting
		1,                                      // bNumEndpoints
		0x02,                                   // bInterfaceClass
		0x02,                                   // bInterfaceSubClass
		0x01,                                   // bInterfaceProtocol
		0,                                      // iInterface
		// CDC Header Functional Descriptor, CDC Spec 5.2.3.1, Table 26
		5,                                      // bFunctionLength
		0x24,                                   // bDescriptorType
		0x00,                                   // bDescriptorSubtype
		0x10, 0x01,                             // bcdCDC
		// Call Management Functional Descriptor, CDC Spec 5.2.3.2, Table 27
		5,                                      // bFunctionLength
		0x24,                                   // bDescriptorType
		0x01,                                   // bDescriptorSubtype
		0x01,                                   // bmCapabilities
		1,                                      // bDataInterface
		// Abstract Control Management Functional Descriptor, CDC Spec 5.2.3.3, Table 28
		4,                                      // bFunctionLength
		0x24,                                   // bDescriptorType
		0x02,                                   // bDescriptorSubtype
		0x06,                                   // bmCapabilities
		// Union Functional Descriptor, CDC Spec 5.2.3.8, Table 33
		5,                                      // bFunctionLength
		0x24,                                   // bDescriptorType
		0x06,                                   // bDescriptorSubtype
		CDC_STATUS_INTERFACE,                   // bMasterInterface
		CDC_DATA_INTERFACE,                     // bSlaveInterface0
		// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
		7,                                      // bLength
		5,                                      // bDescriptorType
		CDC_ACM_ENDPOINT|0x80,                // bEndpointAddress
		0x03,                                   // bmAttributes (0x03=intr)
		CDC_ACM_SIZE, 0,                        // wMaxPacketSize
		16,                                     // bInterval
		// interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
		9,                                      // bLength
		4,                                      // bDescriptorType
		CDC_DATA_INTERFACE,                     // bInterfaceNumber
		0,                                      // bAlternateSetting
		2,                                      // bNumEndpoints
		0x0A,                                   // bInterfaceClass
		0x00,                                   // bInterfaceSubClass
		0x00,                                   // bInterfaceProtocol
		0,                                      // iInterface
		// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
		7,                                      // bLength
		5,                                      // bDescriptorType
		CDC_RX_ENDPOINT,                        // bEndpointAddress
		0x02,                                   // bmAttributes (0x02=bulk)
		LSB(CDC_RX_SIZE_12),MSB(CDC_RX_SIZE_12),// wMaxPacketSize
		0,                                      // bInterval
		// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
		7,                                      // bLength
		5,                                      // bDescriptorType
		CDC_TX_ENDPOINT|0x80,                 // bEndpointAddress
		0x02,                                   // bmAttributes (0x02=bulk)
		LSB(CDC_TX_SIZE_12),MSB(CDC_TX_SIZE_12),// wMaxPacketSize
		0,                                      // bInterval
#endif // CDC_DATA_INTERFACE

#ifdef EXPERIMENTAL_INTERFACE
		// configuration for 12 Mbit/sec speed
			// interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
			9,                                      // bLength
			4,                                      // bDescriptorType
			EXPERIMENTAL_INTERFACE,                 // bInterfaceNumber
			0,                                      // bAlternateSetting
			2,                                      // bNumEndpoints
			0xFF,                                   // bInterfaceClass (0xFF = Vendor)
			0x6A,                                   // bInterfaceSubClass
			0xFF,                                   // bInterfaceProtocol
			0,                                      // iInterface
			// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
			7,                                      // bLength
			5,                                      // bDescriptorType
			1|0x80,                               // bEndpointAddress
			0x02,                                   // bmAttributes (0x02=bulk)
			LSB(64), MSB(64),                       // wMaxPacketSize
			1,                                      // bInterval
			// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
			7,                                      // bLength
			5,                                      // bDescriptorType
			1,                                      // bEndpointAddress
			0x02,                                   // bmAttributes (0x02=bulk)
			LSB(64), MSB(64),                       // wMaxPacketSize
			1,                                      // bInterval
		#endif // EXPERIMENTAL_INTERFACE
};

__attribute__((section(".dmabuffers"), aligned(32))) uint8_t usb_descriptor_buffer[CONFIG_DESC_SIZE];

// **************************************************************
//   String Descriptors
// **************************************************************

// The descriptors above can provide human readable strings,
// referenced by index numbers.  These descriptors are the
// actual string data

/* defined in usb_names.h
struct usb_string_descriptor_struct {
		uint8_t bLength;
		uint8_t bDescriptorType;
		uint16_t wString[];
};
*/

extern struct usb_string_descriptor_struct usb_string_manufacturer_name __attribute__((weak, alias("usb_string_manufacturer_name_default")));
extern struct usb_string_descriptor_struct usb_string_product_name __attribute__((weak, alias("usb_string_product_name_default")));
extern struct usb_string_descriptor_struct usb_string_serial_number __attribute__((weak, alias("usb_string_serial_number_default")));

__attribute__((section(".flashmem"))) const struct usb_string_descriptor_struct string0={
		4,
		3,
		{0x0409}
};

__attribute__((section(".flashmem"))) const struct usb_string_descriptor_struct usb_string_manufacturer_name_default={
		2+MANUFACTURER_NAME_LEN*2,
		3,
		MANUFACTURER_NAME
};
__attribute__((section(".flashmem"))) const struct usb_string_descriptor_struct usb_string_product_name_default={
	2+PRODUCT_NAME_LEN*2,
		3,
		PRODUCT_NAME
};
struct usb_string_descriptor_struct usb_string_serial_number_default={
		12,
		3,
		{0,0,0,0,0,0,0,0,0,0}
};

void usb_init_serialnumber(void)
{
	char buf[11];
	uint32_t i, num;

	num=OCOTP->MAC0&0xFFFFFF;
	// add extra zero to work around OS-X CDC-ACM driver bug
	if(num<10000000) num=num*10;
	utoa(num, buf, 10);
	for(i=0; i<10; i++)
	{
		char c=buf[i];
		if(!c) break;
		usb_string_serial_number_default.wString[i]=c;
	}
	usb_string_serial_number_default.bLength=i*2+2;
}


// **************************************************************
//   Descriptors List
// **************************************************************

// This table provides access to all the descriptor data above.

const usb_descriptor_list_t usb_descriptor_list[]={
	//wValue, wIndex, address,          length
	{0x0100, 0x0000, device_descriptor, sizeof(device_descriptor)},
	{0x0600, 0x0000, qualifier_descriptor, sizeof(qualifier_descriptor)},
	{0x0200, 0x0000, usb_config_descriptor_480, CONFIG_DESC_SIZE},
	{0x0700, 0x0000, usb_config_descriptor_12, CONFIG_DESC_SIZE},
#ifdef EXPERIMENTAL_INTERFACE
	{0x03EE, 0x0000, microsoft_os_string_desc, 18},
	{0x0000, 0xEE04, microsoft_os_compatible_id_desc, 40},
#endif
		{0x0300, 0x0000, (const uint8_t *)&string0, 0},
		{0x0301, 0x0409, (const uint8_t *)&usb_string_manufacturer_name, 0},
		{0x0302, 0x0409, (const uint8_t *)&usb_string_product_name, 0},
		{0x0303, 0x0409, (const uint8_t *)&usb_string_serial_number, 0},
	{0, 0, NULL, 0}
};

#endif // NUM_ENDPOINTS
