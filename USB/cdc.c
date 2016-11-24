#include "MK20D7.h"
#include "cdc.h"
#include "hidef.h"
#include "types.h"
#include "usb_cdc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>

#define ERR_OK						0x00
#define ERR_FAILED					0x01
#define ERR_TXFULL					0x02
#define ERR_RXEMPTY					0x03
#define ERR_OVERFLOW				0x04
#define ERR_BUSOFF					0x05
#define ERR_FAULT					0x06

#define CONTROLLER_ID				(0)

#if HIGH_SPEED_DEVICE
static uint_32 g_cdcBuffer[DIC_BULK_OUT_ENDP_PACKET_SIZE>>1];
#endif

/* Virtual COM Application start Init Flag */
static volatile boolean start_app=FALSE;

/* Virtual COM Application Carrier Activate Flag */
static volatile boolean start_transactions=FALSE;
static volatile boolean transactionOngoing=FALSE;

#define CriticalVariable()	uint8_t cpuSR;

#define EnterCritical()							\
	do {										\
		__asm(									\
			"mrs r0, PRIMASK\n\t"				\
			"cpsid i\n\t"						\
			"strb r0, %[output]\n\t"			\
			: [output] "=m" (cpuSR) :: "r0");	\
		__asm ("" ::: "memory");				\
	} while(0)

#define ExitCritical()							\
	do {										\
		__asm(									\
			"ldrb r0, %[input]\n\t"				\
			"msr PRIMASK, r0\n\t"				\
			::[input] "m" (cpuSR) : "r0");		\
	} while(0)

#define BUF_SIZE					64

uint8_t Rx_buffer[BUF_SIZE];	/* ring buffer */
uint8_t Rx_inIdx;				/* input index */
uint8_t Rx_outIdx;				/* output index */
uint8_t Rx_inSize;				/* size data in buffer */
uint8_t Tx_buffer[BUF_SIZE];	/* ring buffer */
uint8_t Tx_inIdx;				/* input index */
uint8_t Tx_outIdx;				/* output index */
uint8_t Tx_inSize;				/* size data in buffer */

uint8_t Rx_Put(uint8_t elem)
{
	CriticalVariable();

	EnterCritical();
	if(Rx_inSize==BUF_SIZE)
	{
		ExitCritical();
		return ERR_TXFULL;
	}
	else
	{
		Rx_buffer[Rx_inIdx]=elem;
		Rx_inIdx++;

		if(Rx_inIdx==BUF_SIZE)
			Rx_inIdx=0;

		Rx_inSize++;
	}
	ExitCritical();

	return ERR_OK;
}

uint8_t Rx_Get(uint8_t *elemP)
{
	CriticalVariable();

	EnterCritical();
	if(Rx_inSize==0)
	{
		ExitCritical();
		return ERR_RXEMPTY;
	}
	else
	{
		*elemP=Rx_buffer[Rx_outIdx];
		Rx_inSize--;
		Rx_outIdx++;

		if(Rx_outIdx==BUF_SIZE)
			Rx_outIdx=0;
	}
	ExitCritical();

	return ERR_OK;
}

uint8_t Rx_NofElements(void)
{
	return Rx_inSize;
}

uint8_t Rx_NofFreeElements(void)
{
	return BUF_SIZE-Rx_inSize;
}

void Rx_Init(void)
{
	CriticalVariable();

	EnterCritical();
	Rx_inIdx=0;
	Rx_outIdx=0;
	Rx_inSize=0;

	memset(Rx_buffer, 0, sizeof(uint8_t)*BUF_SIZE);
	ExitCritical();
}

uint8_t Rx_Peek(uint8_t index, uint8_t *elemP)
{
	int idx;
	CriticalVariable();

	EnterCritical();
	if(index>=BUF_SIZE)
	{
		ExitCritical();
		return ERR_OVERFLOW;
	}
	else if(index<Rx_inSize)
	{
		idx=(Rx_outIdx+index)%BUF_SIZE;
		*elemP=Rx_buffer[idx];
	}
	else
	{
		ExitCritical();
		return ERR_RXEMPTY;
	}
	ExitCritical();

	return ERR_OK;
}

uint8_t Rx_Delete(void)
{
	CriticalVariable();

	EnterCritical();
	if(Rx_inSize==0)
	{
		ExitCritical();
		return ERR_RXEMPTY;
	}
	else
	{
		Rx_inSize--;
		Rx_outIdx++;

		if(Rx_outIdx==BUF_SIZE)
			Rx_outIdx=0;
	}
	ExitCritical();

	return ERR_OK;
}

uint8_t Tx_Put(uint8_t elem)
{
	CriticalVariable();

	EnterCritical();
	if(Tx_inSize==BUF_SIZE)
	{
		ExitCritical();
		return ERR_TXFULL;
	}
	else
	{
		Tx_buffer[Tx_inIdx]=elem;
		Tx_inIdx++;

		if(Tx_inIdx==BUF_SIZE)
			Tx_inIdx=0;

		Tx_inSize++;
	}
	ExitCritical();

	return ERR_OK;
}

uint8_t Tx_Get(uint8_t *elemP)
{
	CriticalVariable();

	EnterCritical();
	if(Tx_inSize==0)
	{
		ExitCritical();
		return ERR_RXEMPTY;
	}
	else
	{
		*elemP=Tx_buffer[Tx_outIdx];
		Tx_inSize--;
		Tx_outIdx++;

		if(Tx_outIdx==BUF_SIZE)
			Tx_outIdx=0;
	}
	ExitCritical();

	return ERR_OK;
}

uint8_t Tx_NofElements(void)
{
	return Tx_inSize;
}

uint8_t Tx_NofFreeElements(void)
{
	return BUF_SIZE-Tx_inSize;
}

void Tx_Init(void)
{
	CriticalVariable();

	EnterCritical();
	Tx_inIdx=0;
	Tx_outIdx=0;
	Tx_inSize=0;

	memset(Tx_buffer, 0, sizeof(uint8_t)*BUF_SIZE);
	ExitCritical();
}

uint8_t Tx_Peek(uint8_t index, uint8_t *elemP)
{
	int idx;
	CriticalVariable();

	EnterCritical();

	if(index>=BUF_SIZE)
	{
		ExitCritical();
		return ERR_OVERFLOW;
	}
	else if(index<Tx_inSize)
	{
		idx=(Tx_outIdx+index)%BUF_SIZE;
		*elemP=Tx_buffer[idx];
	}
	else
	{
		ExitCritical();
		return ERR_RXEMPTY;
	}
	ExitCritical();

	return ERR_OK;
}

uint8_t Tx_Delete(void)
{
	CriticalVariable();

	EnterCritical();
	if(Tx_inSize==0)
	{
		ExitCritical();
		return ERR_RXEMPTY;
	}
	else
	{
		Tx_inSize--;
		Tx_outIdx++;

		if(Tx_outIdx==BUF_SIZE)
			Tx_outIdx=0;
	}

	ExitCritical();
	return ERR_OK;
}

uint16_t CDC_GetCharsInRxBuf(void)
{
	static uint8_t txBuf[BUF_SIZE];

	if(CDC_App_Task(txBuf, sizeof(txBuf))!=ERR_OK)
	{ /* call USB handler: if active, then this will empty the buffer */
	}

	return (uint16_t)Rx_NofElements();
}

uint8_t CDC_RecvChar(CDC_TComData *Chr)
{
	while(Rx_Get(Chr)!=ERR_OK)
	{
		/* retry receiving until success */
	}

	return ERR_OK;
}

uint8_t CDC_SendChar(CDC_TComData Chr)
{
	static uint8_t txBuf[BUF_SIZE];
	uint8_t res;

	if(Tx_Put(Chr)==ERR_TXFULL)
	{
		res=CDC_App_Task(txBuf, sizeof(txBuf));

		if(res==ERR_BUSOFF)
			return ERR_FAILED;
		else if(res!=ERR_OK)
			return ERR_TXFULL;
		else
			return Tx_Put(Chr);
	}

	return ERR_OK;
}

uint8_t CDC_SendBlock(uint8_t *data, uint16_t dataSize)
{
	uint8_t res=ERR_OK;

	while(dataSize>0)
	{
		if(CDC_SendChar(*data)!=ERR_OK)
			res=ERR_TXFULL;

		dataSize--;
		data++;
	}

	return res;
}

uint8_t CDC_SendString(CDC_TComData *Chr)
{
	uint8_t res=ERR_OK;

	while(*Chr!='\0')
	{
		if(CDC_SendChar(*Chr)!=ERR_OK)
			res=ERR_TXFULL;

		Chr++;
	}

	return res;
}

void CDC_App_Callback(uint8_t controller_ID, uint8_t event_type, void *val)
{
	UNUSED(controller_ID);
	UNUSED(val);

	if(event_type==USB_APP_BUS_RESET)
		start_app=FALSE;
	else if(event_type==USB_APP_ENUM_COMPLETE)
	{
#if HIGH_SPEED_DEVICE
		USB_Class_CDC_Interface_DIC_Recv_Data(&controller_ID, (uint_8_ptr)g_cdcBuffer, DIC_BULK_OUT_ENDP_PACKET_SIZE);
#else
		(void)USB_Class_CDC_Interface_DIC_Recv_Data(CONTROLLER_ID, NULL, 0);
#endif
		start_app=TRUE;
	}
	else if((event_type==USB_APP_DATA_RECEIVED)&&(start_transactions==TRUE))
	{
		USB_PACKET_SIZE BytesToBeCopied;
		APP_DATA_STRUCT *dp_rcv=(APP_DATA_STRUCT *)val;
		uint_8 idx;

		BytesToBeCopied=(USB_PACKET_SIZE)((dp_rcv->data_size>BUF_SIZE)?BUF_SIZE:dp_rcv->data_size);

		for(idx=0;idx<BytesToBeCopied;idx++)
		{
			if(Rx_Put(dp_rcv->data_ptr[idx])!=ERR_OK)
			{
			}
		}

		(void)USB_Class_CDC_Interface_DIC_Recv_Data(CONTROLLER_ID, NULL, 0);
	}
	else if((event_type==USB_APP_SEND_COMPLETE)&&(start_transactions==TRUE))
	{
		transactionOngoing=FALSE;
	}
	else if(event_type==USB_APP_ERROR)
	{
		start_app=FALSE;
		start_transactions=FALSE;
	}
}

void CDC_Notify_Callback(uint8_t controller_ID, uint8_t event_type, void *val)
{
	UNUSED(controller_ID);
	UNUSED(val);

	if(start_app==TRUE)
	{
		if(event_type==USB_APP_CDC_CARRIER_ACTIVATED)
			start_transactions=TRUE;
		else if(event_type==USB_APP_CDC_CARRIER_DEACTIVATED)
			start_transactions=FALSE;
	}

	start_transactions=TRUE;
}

uint8_t CDC_SendDataBlock(uint8_t *data, uint16_t dataSize)
{
	uint8_t res=ERR_OK;

	transactionOngoing=TRUE;

	if(USB_Class_CDC_Interface_DIC_Send_Data(CONTROLLER_ID, data, dataSize)!=USB_OK)
	{
		transactionOngoing=FALSE;
		return ERR_FAULT;
	}

	while(transactionOngoing)
		DelayMS(10);

	return res;
}

uint8_t CDC_App_Task(uint8_t *txBuf, uint16_t txBufSize)
{
	uint16_t i;
	uint8_t res;

	USB_Class_CDC_Periodic_Task();

	if((start_app==TRUE)&&(start_transactions==TRUE))
	{
		if(Tx_NofElements()!=0)
		{
			i=0;

			while(i<txBufSize&&Tx_Get(&txBuf[i])==ERR_OK)
				i++;

			res=CDC_SendDataBlock(txBuf, i);

			if(res!=ERR_OK)
				return res;

			if((i%8)==0)
			{
				res=CDC_SendDataBlock(txBuf, 0);

				if(res!=ERR_OK)
					return res;
			}
		}

		return ERR_OK;
	}
	else
		return ERR_BUSOFF;
}

extern uint8_t g_Mem[];

uint8_t CDC_Init(void)
{
	SIM_SCGC4|=SIM_SCGC4_USBOTG_MASK;
	SIM_CLKDIV2&=~(SIM_CLKDIV2_USBDIV(0x07)|SIM_CLKDIV2_USBFRAC_MASK);
	SIM_SOPT2|=SIM_SOPT2_USBSRC_MASK;
	SIM_SCGC4|=SIM_SCGC4_USBOTG_MASK;

	USB0_CTL|=USB_CTL_ODDRST_MASK;
	USB0_USBCTRL=(USB_USBCTRL_SUSP_MASK|USB_USBCTRL_PDE_MASK);
	USB0_OTGISTAT=USB_OTGISTAT_IDCHG_MASK|USB_OTGISTAT_ONEMSEC_MASK|USB_OTGISTAT_LINE_STATE_CHG_MASK|USB_OTGISTAT_SESSVLDCHG_MASK|USB_OTGISTAT_B_SESS_CHG_MASK|USB_OTGISTAT_AVBUSCHG_MASK|0x12U;
	USB0_ISTAT=USB_ISTAT_STALL_MASK|USB_ISTAT_ATTACH_MASK|USB_ISTAT_RESUME_MASK|USB_ISTAT_SLEEP_MASK|USB_ISTAT_TOKDNE_MASK|USB_ISTAT_SOFTOK_MASK|USB_ISTAT_ERROR_MASK|USB_ISTAT_USBRST_MASK;
	USB0_ERRSTAT=USB_ERRSTAT_BTSERR_MASK|USB_ERRSTAT_DMAERR_MASK|USB_ERRSTAT_BTOERR_MASK|USB_ERRSTAT_DFN8_MASK|USB_ERRSTAT_CRC16_MASK|USB_ERRSTAT_CRC5EOF_MASK|USB_ERRSTAT_PIDERR_MASK|0x40U;
	USB0_INTEN=USB_INTEN_STALLEN_MASK|USB_INTEN_ATTACHEN_MASK|USB_INTEN_RESUMEEN_MASK|USB_INTEN_SLEEPEN_MASK|USB_INTEN_TOKDNEEN_MASK|USB_INTEN_SOFTOKEN_MASK|USB_INTEN_ERROREN_MASK|USB_INTEN_USBRSTEN_MASK;
	USB0_ERREN=0x00U;
	USB0_USBTRC0=(USB_USBTRC0_USBRESMEN_MASK|0x40U);
	USB0_OTGICR=0x00U;
	USB0_ADDR=USB_ADDR_ADDR(0x00);
	USB0_ENDPT0=0x00U;
	USB0_ENDPT1=0x00U;
	USB0_ENDPT2=0x00U;
	USB0_ENDPT3=0x00U;
	USB0_ENDPT4=0x00U;
	USB0_ENDPT5=0x00U;
	USB0_ENDPT6=0x00U;
	USB0_ENDPT7=0x00U;
	USB0_ENDPT8=0x00U;
	USB0_ENDPT9=0x00U;
	USB0_ENDPT10=0x00U;
	USB0_ENDPT11=0x00U;
	USB0_ENDPT12=0x00U;
	USB0_ENDPT13=0x00U;
	USB0_ENDPT14=0x00U;
	USB0_ENDPT15=0x00U;
	USB0_BDTPAGE1=(uint8_t)((((uint32_t)((uint32_t)&g_Mem[0]))>>0x08)&0xFEU);
	USB0_BDTPAGE2=(uint8_t)((((uint32_t)((uint32_t)&g_Mem[0]))>>0x10)&0xFFU);
	USB0_BDTPAGE3=(uint8_t)((((uint32_t)((uint32_t)&g_Mem[0]))>>0x18)&0xFFU);
	USB0_SOFTHLD=USB_SOFTHLD_CNT(0x00);
	USB0_OTGCTL=0x00U;
	USB0_CONTROL=0x00U;
	USB0_CTL=((USB0_CTL&~(USB_CTL_TXSUSPENDTOKENBUSY_MASK|USB_CTL_HOSTMODEEN_MASK|USB_CTL_ODDRST_MASK))|USB_CTL_USBENSOFEN_MASK);

	start_app=FALSE;
	start_transactions=FALSE;
	transactionOngoing=FALSE;

	Rx_Init();
	Tx_Init();

	if(USB_Class_CDC_Init(CONTROLLER_ID, CDC_App_Callback, NULL, CDC_Notify_Callback, TRUE)!=USB_OK)
		return ERR_FAILED;

	NVICICPR2=(1<<9);    /* Clear any pending interrupts on USB (Interrupt Clear-Pending Register) */
	NVICISER2=(1<<9);    /* Enable interrupts from USB module (Interrupt Set-Enable Register) */

	return ERR_OK;
}

uint8_t CDC_PutBufferChecked(uint8_t *buf, uint16_t bufSize)
{
	uint8_t res;

	if(bufSize>CDC_GetFreeInTxBuf())
		res=ERR_TXFULL;
	else
	{
		res=ERR_OK;

		while(bufSize>0&&res==ERR_OK)
		{
			res=Tx_Put(*buf);
			bufSize--;
			buf++;
		}
	}

	return res;
}

uint8_t CDC1_ApplicationStarted(void)
{
	return start_app;
}

uint8_t CDC1_TransactionsStarted(void)
{
	return start_transactions;
}
