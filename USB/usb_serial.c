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

#include "usb_dev.h"
#include "usb_serial.h"
#include <string.h> // for memcpy()

// defined by usb_dev.h -> usb_desc.h
#if defined(CDC_STATUS_INTERFACE)&&defined(CDC_DATA_INTERFACE)

uint32_t usb_cdc_line_coding[2];

volatile uint32_t usb_cdc_line_rtsdtr_millis;
volatile uint8_t usb_cdc_line_rtsdtr=0;
volatile uint8_t usb_cdc_transmit_flush_timer=0;

static volatile uint8_t tx_noautoflush=0;

extern volatile uint8_t usb_high_speed;

// TODO: should be 2 different timeouts, high speed (480) vs full speed (12)
#define TRANSMIT_FLUSH_TIMEOUT	75   /* in microseconds */

static void timer_config(void (*callback)(void), uint32_t microseconds);
static void timer_start_oneshot();
static void timer_stop();
static void usb_serial_flush_callback(void);

#define TX_NUM   4
#define TX_SIZE  2048 /* should be a multiple of CDC_TX_SIZE */
static transfer_t tx_transfer[TX_NUM] __attribute__ ((used, aligned(32)));

__attribute__((section(".dmabuffers"), used)) static uint8_t txbuffer[TX_SIZE * TX_NUM] __attribute__ ((aligned(32)));

static uint8_t tx_head=0;
static uint16_t tx_available=0;
static uint16_t tx_packet_size=0;

#define RX_NUM  8
static transfer_t rx_transfer[RX_NUM] __attribute__ ((used, aligned(32)));

__attribute__((section(".dmabuffers"), used)) static uint8_t rx_buffer[RX_NUM * CDC_RX_SIZE_480] __attribute__ ((aligned(32)));

static uint16_t rx_count[RX_NUM];
static uint16_t rx_index[RX_NUM];
static uint16_t rx_packet_size=0;

static volatile uint8_t rx_head;
static volatile uint8_t rx_tail;

static uint8_t rx_list[RX_NUM + 1];

static volatile uint32_t rx_available;

static void rx_queue_transfer(int i);
static void rx_event(transfer_t *t);


void usb_serial_reset(void)
{
}

void usb_serial_configure(void)
{
	int i;

	if(usb_high_speed)
	{
		tx_packet_size=CDC_TX_SIZE_480;
		rx_packet_size=CDC_RX_SIZE_480;
	} else
	{
		tx_packet_size=CDC_TX_SIZE_12;
		rx_packet_size=CDC_RX_SIZE_12;
	}

	memset(tx_transfer, 0, sizeof(tx_transfer));

	tx_head = 0;
	tx_available = 0;

	memset(rx_transfer, 0, sizeof(rx_transfer));
	memset(rx_count, 0, sizeof(rx_count));
	memset(rx_index, 0, sizeof(rx_index));

	rx_head = 0;
	rx_tail = 0;

	rx_available = 0;

	usb_config_tx(CDC_ACM_ENDPOINT, CDC_ACM_SIZE, 0, NULL); // size same 12 & 480
	usb_config_rx(CDC_RX_ENDPOINT, rx_packet_size, 0, rx_event);
	usb_config_tx(CDC_TX_ENDPOINT, tx_packet_size, 1, NULL);

	for(i=0;i<RX_NUM;i++)
		rx_queue_transfer(i);

	timer_config(usb_serial_flush_callback, TRANSMIT_FLUSH_TIMEOUT);
}

/*************************************************************************/
/**                               Receive                               **/
/*************************************************************************/
static void rx_queue_transfer(int i)
{
	NVIC_DisableIRQ(USB_OTG1_IRQn);

	void *buffer=rx_buffer+i*CDC_RX_SIZE_480;

	usb_prepare_transfer(rx_transfer+i, buffer, rx_packet_size, i);
	SCB_CleanDCache_by_Addr(buffer, rx_packet_size);
	usb_receive(CDC_RX_ENDPOINT, rx_transfer+i);

	NVIC_EnableIRQ(USB_OTG1_IRQn);
}

// called by USB interrupt when any packet is received
static void rx_event(transfer_t *t)
{
	int len=rx_packet_size-((t->status>>16)&0x7FFF);
	int i=t->callback_param;

	if(len>0)
	{
		// received a packet with data
		uint32_t head=rx_head;

		if(head!=rx_tail)
		{
			// a previous packet is still buffered
			uint32_t ii=rx_list[head];
			uint32_t count=rx_count[ii];
			if(len<=CDC_RX_SIZE_480-count)
			{
				// previous buffer has enough free space for this packet's data
				memcpy(rx_buffer+ii*CDC_RX_SIZE_480+count, rx_buffer+i*CDC_RX_SIZE_480, len);

				rx_count[ii]=count+len;
				rx_available+=len;
				rx_queue_transfer(i);

				// TODO: trigger serialEvent
				return;
			}
		}

		// add this packet to rx_list
		rx_count[i]=len;
		rx_index[i]=0;

		if(++head>RX_NUM)
			head=0;

		rx_list[head]=i;
		rx_head=head;
		rx_available+=len;
		// TODO: trigger serialEvent
	}
	else
	{
		// received a zero length packet
		rx_queue_transfer(i);
	}
}

// read a block of bytes to a buffer
int usb_serial_read(void *buffer, uint32_t size)
{
	uint8_t *p=(uint8_t *)buffer;
	uint32_t count=0;

	NVIC_DisableIRQ(USB_OTG1_IRQn);

	uint32_t tail=rx_tail;

	while(count<size&&tail!=rx_head)
	{
		if(++tail>RX_NUM)
			tail=0;

		uint32_t i=rx_list[tail];
		uint32_t len=size-count;
		uint32_t avail=rx_count[i]-rx_index[i];

		//count, size, i, rx_index[i], len, avail, rx_buffer[i * CDC_RX_SIZE_480]);
		if(avail>len)
		{
			// partially consume this packet
			memcpy(p, rx_buffer+i*CDC_RX_SIZE_480+rx_index[i], len);

			rx_available-=len;
			rx_index[i]+=len;

			count+=len;
		}
		else
		{
			// fully consume this packet
			memcpy(p, rx_buffer+i*CDC_RX_SIZE_480+rx_index[i], avail);

			p+=avail;

			rx_available-=avail;
			count+=avail;
			rx_tail=tail;

			rx_queue_transfer(i);
		}
	}

	NVIC_EnableIRQ(USB_OTG1_IRQn);

	return count;
}

// peek at the next character, or -1 if nothing received
int usb_serial_peekchar(void)
{
	uint32_t tail=rx_tail;

	if(tail==rx_head)
		return -1;

	if(++tail>RX_NUM)
		tail=0;

	uint32_t i=rx_list[tail];

	return rx_buffer[i*CDC_RX_SIZE_480+rx_index[i]];
}

// number of bytes available in the receive buffer
int usb_serial_available(void)
{
	return rx_available;
}

// discard any buffered input
void usb_serial_flush_input(void)
{
	uint32_t tail=rx_tail;

	while(tail!=rx_head)
	{
		if(++tail>RX_NUM)
			tail=0;

		uint32_t i=rx_list[tail];

		rx_available-=rx_count[i]-rx_index[i];

		rx_queue_transfer(i);

		rx_tail=tail;
	}
}


// get the next character, or -1 if nothing received
int usb_serial_getchar(void)
{
	uint8_t c;
	if (usb_serial_read(&c, 1)) return c;
	return -1;
}

/*************************************************************************/
/**                               Transmit                              **/
/*************************************************************************/

// When the PC isn't listening, how long do we wait before discarding data?  If this is
// too short, we risk losing data during the stalls that are common with ordinary desktop
// software.  If it's too long, we stall the user's program when no software is running.
#define TX_TIMEOUT_MSEC 120

// When we've suffered the transmit timeout, don't wait again until the computer
// begins accepting data.  If no software is running to receive, we'll just discard
// data as rapidly as Serial.print() can generate it, until there's something to
// actually receive it.
static uint8_t transmit_previous_timeout=0;

// transmit a character.  0 returned on success, -1 on error
int usb_serial_putchar(uint8_t c)
{
	return usb_serial_write(&c, 1);
}

extern volatile uint32_t systick_millis_count;

static void timer_config(void (*callback)(void), uint32_t microseconds);
static void timer_start_oneshot();
static void timer_stop();

static void timer_config(void (*callback)(void), uint32_t microseconds)
{
	usb_timer0_callback=callback;

	USB1->GPTIMER0CTRL=0;
	USB1->GPTIMER0LD=microseconds-1;

	USB1->USBINTR|=USB_USBINTR_TIE0_MASK;
}

static void timer_start_oneshot(void)
{
	// restarts timer if already running (retriggerable one-shot)
	USB1->GPTIMER0CTRL=USB_GPTIMER0CTRL_GPTRUN_MASK|USB_GPTIMER0CTRL_GPTRST_MASK;
}

static void timer_stop(void)
{
	USB1->GPTIMER0CTRL=0;
}


int usb_serial_write(const void *buffer, uint32_t size)
{
	uint32_t sent=0;
	const uint8_t *data=(const uint8_t *)buffer;

	if(!usb_configuration)
		return 0;

	while(size>0)
	{
		tx_noautoflush=1;

		transfer_t *xfer=tx_transfer+tx_head;

		int waiting=0;
		uint32_t wait_begin_at=0;

		while(!tx_available)
		{
			uint32_t status=usb_transfer_status(xfer);

			if(!(status&0x80))
			{
				if(status&0x68)
				{
					// TODO: what if status has errors???
				}

				tx_available=TX_SIZE;
				transmit_previous_timeout=0;
				break;
			}

			asm("dsb" ::: "memory");

			tx_noautoflush=0;

			if(!waiting)
			{
				wait_begin_at=systick_millis_count;
				waiting=1;
			}

			if(transmit_previous_timeout)
				return sent;

			if(systick_millis_count-wait_begin_at>TX_TIMEOUT_MSEC)
			{
				transmit_previous_timeout=1;
				return sent;
			}

			if(!usb_configuration)
				return sent;
			
			tx_noautoflush=1;
		}

		uint8_t *txdata=txbuffer+(tx_head*TX_SIZE)+(TX_SIZE-tx_available);

		if(size>=tx_available)
		{
			memcpy(txdata, data, tx_available);

			//*(txbuffer + (tx_head * TX_SIZE)) = 'A' + tx_head; // to see which buffer
			//*(txbuffer + (tx_head * TX_SIZE) + 1) = ' '; // really see it

			uint8_t *txbuf=txbuffer+(tx_head*TX_SIZE);

			usb_prepare_transfer(xfer, txbuf, TX_SIZE, 0);
			SCB_CleanInvalidateDCache_by_Addr(txbuf, TX_SIZE);
			usb_transmit(CDC_TX_ENDPOINT, xfer);
			
			if(++tx_head>=TX_NUM)
				tx_head=0;

			size-=tx_available;
			sent+=tx_available;
			data+=tx_available;

			tx_available=0;

			timer_stop();
		}
		else
		{
			memcpy(txdata, data, size);
			
			tx_available-=size;
			sent+=size;
			size=0;

			timer_start_oneshot();
		}

		asm("dsb" ::: "memory");

		tx_noautoflush=0;
	}

	return sent;
}

int usb_serial_write_buffer_free(void)
{
	uint32_t sum=0;

	tx_noautoflush=1;

	for(uint32_t i=0;i<TX_NUM;i++)
	{
		if(i==tx_head)
			continue;

		if(!(usb_transfer_status(tx_transfer+i)&0x80))
			sum+=TX_SIZE;
	}
	asm("dsb" ::: "memory");

	tx_noautoflush=0;

	return sum;
}

void usb_serial_flush_output(void)
{
	if(!usb_configuration)
		return;

	if(tx_available==0)
		return;

	tx_noautoflush=1;

	transfer_t *xfer=tx_transfer+tx_head;
	uint8_t *txbuf=txbuffer+(tx_head*TX_SIZE);
	uint32_t txnum=TX_SIZE-tx_available;
	
	usb_prepare_transfer(xfer, txbuf, txnum, 0);
	SCB_CleanInvalidateDCache_by_Addr(txbuf, txnum);
	usb_transmit(CDC_TX_ENDPOINT, xfer);
	
	if(++tx_head>=TX_NUM)
		tx_head=0;

	tx_available=0;

	asm("dsb" ::: "memory");

	tx_noautoflush=0;
}

static void usb_serial_flush_callback(void)
{
	if(tx_noautoflush)
		return;

	if(!usb_configuration)
		return;

	if(tx_available==0)
		return;

	transfer_t *xfer=tx_transfer+tx_head;
	uint8_t *txbuf=txbuffer+(tx_head*TX_SIZE);
	uint32_t txnum=TX_SIZE-tx_available;

	usb_prepare_transfer(xfer, txbuf, txnum, 0);
	SCB_CleanInvalidateDCache_by_Addr(txbuf, txnum);
	usb_transmit(CDC_TX_ENDPOINT, xfer);

	if(++tx_head>=TX_NUM)
		tx_head=0;

	tx_available=0;
}

#endif // CDC_STATUS_INTERFACE && CDC_DATA_INTERFACE
