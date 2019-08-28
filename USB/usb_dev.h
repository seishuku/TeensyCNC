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

#ifndef _usb_dev_h_
#define _usb_dev_h_

#define USB_DESC_LIST_DEFINE
#include "usb_desc.h"

// This header is NOT meant to be included when compiling
// user sketches in Arduino.  The low-level functions
// provided by usb_dev.c are meant to be called only by
// code which provides higher-level interfaces to the user.

#include "usb_mem.h"

void usb_init(void);
__attribute__ ((interrupt)) void usb_isr(void);
usb_packet_t *usb_rx(uint32_t endpoint);
uint32_t usb_tx_byte_count(uint32_t endpoint);
uint32_t usb_tx_packet_count(uint32_t endpoint);
void usb_tx(uint32_t endpoint, usb_packet_t *packet);
void usb_tx_isochronous(uint32_t endpoint, void *data, uint32_t len);

extern volatile uint8_t usb_configuration;

extern uint16_t usb_rx_byte_count_data[NUM_ENDPOINTS];

__attribute__((always_inline)) static inline uint32_t usb_rx_byte_count(uint32_t endpoint)
{
        endpoint--;

		if(endpoint>=NUM_ENDPOINTS)
			return 0;

		return usb_rx_byte_count_data[endpoint];
}

extern volatile uint8_t usb_cdc_transmit_flush_timer;
extern void usb_serial_flush_callback(void);

#endif
