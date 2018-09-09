// TeensyCNC
// Copyright 2016 Matt Williams
//
// ARM startup and basic hardware Initialization

#include "MK20D7.h"
#include <string.h>
#include <stdlib.h>

typedef struct
{
	void *__ptr;
	void *__fun[0x6E];
} tVectorTable;

extern uint32_t __SP_INIT[];
extern uint32_t __START_BSS[];
extern uint32_t __END_BSS[];

void __thumb_startup(void);

extern void __attribute__ ((interrupt)) Cpu_ivINT_FTM1(void);
extern void __attribute__ ((interrupt)) Cpu_ivINT_PORTB(void);
extern void __attribute__ ((interrupt)) Cpu_ivINT_PORTC(void);
extern void __attribute__ ((interrupt)) USB_ISR(void);

void __attribute__ ((interrupt)) Cpu_Interrupt(void)
{
}

volatile uint32_t SysTick;

void __attribute__ ((interrupt)) Cpu_ivINT_SysTick(void)
{
	SysTick++;
}

// Interrupt vector table
__attribute__ ((section (".vectortable"))) const tVectorTable __vect_table=
{
	&__SP_INIT,					//	0x00	ivINT_Initial_Stack_Pointer
	{
		&__thumb_startup,		//	0x01	ivINT_Initial_Program_Counter
		&Cpu_Interrupt,			//	0x02	ivINT_NMI
		&Cpu_Interrupt,			//	0x03	ivINT_Hard_Fault
		&Cpu_Interrupt,			//	0x04	ivINT_Mem_Manage_Fault
		&Cpu_Interrupt,			//	0x05	ivINT_Bus_Fault
		&Cpu_Interrupt,			//	0x06	ivINT_Usage_Fault
		&Cpu_Interrupt,			//	0x07	ivINT_Reserved7
		&Cpu_Interrupt,			//	0x08	ivINT_Reserved8
		&Cpu_Interrupt,			//	0x09	ivINT_Reserved9
		&Cpu_Interrupt,			//	0x0A	ivINT_Reserved10
		&Cpu_Interrupt,			//	0x0B	ivINT_SVCall
		&Cpu_Interrupt,			//	0x0C	ivINT_DebugMonitor
		&Cpu_Interrupt,			//	0x0D	ivINT_Reserved13
		&Cpu_Interrupt,			//	0x0E	ivINT_PendableSrvReq
		&Cpu_ivINT_SysTick,		//	0x0F	ivINT_SysTick
		&Cpu_Interrupt,			//	0x10	ivINT_DMA0
		&Cpu_Interrupt,			//	0x11	ivINT_DMA1
		&Cpu_Interrupt,			//	0x12	ivINT_DMA2
		&Cpu_Interrupt,			//	0x13	ivINT_DMA3
		&Cpu_Interrupt,			//	0x14	ivINT_DMA4
		&Cpu_Interrupt,			//	0x15	ivINT_DMA5
		&Cpu_Interrupt,			//	0x16	ivINT_DMA6
		&Cpu_Interrupt,			//	0x17	ivINT_DMA7
		&Cpu_Interrupt,			//	0x18	ivINT_DMA8
		&Cpu_Interrupt,			//	0x19	ivINT_DMA9
		&Cpu_Interrupt,			//	0x1A	ivINT_DMA10
		&Cpu_Interrupt,			//	0x1B	ivINT_DMA11
		&Cpu_Interrupt,			//	0x1C	ivINT_DMA12
		&Cpu_Interrupt,			//	0x1D	ivINT_DMA13
		&Cpu_Interrupt,			//	0x1E	ivINT_DMA14
		&Cpu_Interrupt,			//	0x1F	ivINT_DMA15
		&Cpu_Interrupt,			//	0x20	ivINT_DMA_Error
		&Cpu_Interrupt,			//	0x21	ivINT_MCM
		&Cpu_Interrupt,			//	0x22	ivINT_FTFL
		&Cpu_Interrupt,			//	0x23	ivINT_Read_Collision
		&Cpu_Interrupt,			//	0x24	ivINT_LVD_LVW
		&Cpu_Interrupt,			//	0x25	ivINT_LLW
		&Cpu_Interrupt,			//	0x26	ivINT_Watchdog
		&Cpu_Interrupt,			//	0x27	ivINT_Reserved39
		&Cpu_Interrupt,			//	0x28	ivINT_I2C0
		&Cpu_Interrupt,			//	0x29	ivINT_I2C1
		&Cpu_Interrupt,			//	0x2A	ivINT_SPI0
		&Cpu_Interrupt,			//	0x2B	ivINT_SPI1
		&Cpu_Interrupt,			//	0x2C	ivINT_Reserved44
		&Cpu_Interrupt,			//	0x2D	ivINT_CAN0_ORed_Message_buffer
		&Cpu_Interrupt,			//	0x2E	ivINT_CAN0_Bus_Off
		&Cpu_Interrupt,			//	0x2F	ivINT_CAN0_Error
		&Cpu_Interrupt,			//	0x30	ivINT_CAN0_Tx_Warning
		&Cpu_Interrupt,			//	0x31	ivINT_CAN0_Rx_Warning
		&Cpu_Interrupt,			//	0x32	ivINT_CAN0_Wake_Up
		&Cpu_Interrupt,			//	0x33	ivINT_I2S0_Tx
		&Cpu_Interrupt,			//	0x34	ivINT_I2S0_Rx
		&Cpu_Interrupt,			//	0x35	ivINT_Reserved53
		&Cpu_Interrupt,			//	0x36	ivINT_Reserved54
		&Cpu_Interrupt,			//	0x37	ivINT_Reserved55
		&Cpu_Interrupt,			//	0x38	ivINT_Reserved56
		&Cpu_Interrupt,			//	0x39	ivINT_Reserved57
		&Cpu_Interrupt,			//	0x3A	ivINT_Reserved58
		&Cpu_Interrupt,			//	0x3B	ivINT_Reserved59
		&Cpu_Interrupt,			//	0x3C	ivINT_UART0_LON
		&Cpu_Interrupt,			//	0x3D	ivINT_UART0_RX_TX
		&Cpu_Interrupt,			//	0x3E	ivINT_UART0_ERR
		&Cpu_Interrupt,			//	0x3F	ivINT_UART1_RX_TX
		&Cpu_Interrupt,			//	0x40	ivINT_UART1_ERR
		&Cpu_Interrupt,			//	0x41	ivINT_UART2_RX_TX
		&Cpu_Interrupt,			//	0x42	ivINT_UART2_ERR
		&Cpu_Interrupt,			//	0x43	ivINT_UART3_RX_TX
		&Cpu_Interrupt,			//	0x44	ivINT_UART3_ERR
		&Cpu_Interrupt,			//	0x45	ivINT_UART4_RX_TX
		&Cpu_Interrupt,			//	0x46	ivINT_UART4_ERR
		&Cpu_Interrupt,			//	0x47	ivINT_Reserved71
		&Cpu_Interrupt,			//	0x48	ivINT_Reserved72
		&Cpu_Interrupt,			//	0x49	ivINT_ADC0
		&Cpu_Interrupt,			//	0x4A	ivINT_ADC1
		&Cpu_Interrupt,			//	0x4B	ivINT_CMP0
		&Cpu_Interrupt,			//	0x4C	ivINT_CMP1
		&Cpu_Interrupt,			//	0x4D	ivINT_CMP2
		&Cpu_Interrupt,			//	0x4E	ivINT_FTM0
		&Cpu_ivINT_FTM1,		//	0x4F	ivINT_FTM1
		&Cpu_Interrupt,			//	0x50	ivINT_FTM2
		&Cpu_Interrupt,			//	0x51	ivINT_CMT
		&Cpu_Interrupt,			//	0x52	ivINT_RTC
		&Cpu_Interrupt,			//	0x53	ivINT_RTC_Seconds
		&Cpu_Interrupt,			//	0x54	ivINT_PIT0
		&Cpu_Interrupt,			//	0x55	ivINT_PIT1
		&Cpu_Interrupt,			//	0x56	ivINT_PIT2
		&Cpu_Interrupt,			//	0x57	ivINT_PIT3
		&Cpu_Interrupt,			//	0x58	ivINT_PDB0
		&USB_ISR,				//	0x59	ivINT_USB0
		&Cpu_Interrupt,			//	0x5A	ivINT_USBDCD
		&Cpu_Interrupt,			//	0x5B	ivINT_Reserved91
		&Cpu_Interrupt,			//	0x5C	ivINT_Reserved92
		&Cpu_Interrupt,			//	0x5D	ivINT_Reserved93
		&Cpu_Interrupt,			//	0x5E	ivINT_Reserved94
		&Cpu_Interrupt,			//	0x5F	ivINT_Reserved95
		&Cpu_Interrupt,			//	0x60	ivINT_Reserved96
		&Cpu_Interrupt,			//	0x61	ivINT_DAC0
		&Cpu_Interrupt,			//	0x62	ivINT_Reserved98
		&Cpu_Interrupt,			//	0x63	ivINT_TSI0
		&Cpu_Interrupt,			//	0x64	ivINT_MCG
		&Cpu_Interrupt,			//	0x65	ivINT_LPTimer
		&Cpu_Interrupt,			//	0x66	ivINT_Reserved102
		&Cpu_Interrupt,			//	0x67	ivINT_PORTA
		&Cpu_ivINT_PORTB,		//	0x68	ivINT_PORTB
		&Cpu_ivINT_PORTC,		//	0x69	ivINT_PORTC
		&Cpu_Interrupt,			//	0x6A	ivINT_PORTD
		&Cpu_Interrupt,			//	0x6B	ivINT_PORTE
		&Cpu_Interrupt,			//	0x6C	ivINT_Reserved108
		&Cpu_Interrupt,			//	0x6D	ivINT_Reserved109
		&Cpu_Interrupt			//	0x6E	ivINT_SWI
	}
};

// Flash configuration field
__attribute__ ((section (".cfmconfig"))) const uint8_t _cfm[0x10]={ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7E, 0xFB, 0xFF, 0xFF };

extern int main(void);

// format of the ROM table info entry
typedef struct RomInfo
{
	unsigned long Source;
	unsigned long Target;
	unsigned long Size;
} RomInfo;

// linker defined symbol
extern RomInfo __S_romp[] __attribute__((weak));

// Current value of the FAULTMASK register and lock
volatile uint8_t SR_reg;
volatile uint8_t SR_lock=0x00;

// Routine to copy a single section from ROM to RAM
void __copy_rom_section(uint32_t dst, uint32_t src, uint32_t size)
{
	while(size--)
		*((uint8_t *)dst++)=*((uint8_t *)src++);
}

// Routine that copies all sections the user marked as ROM into their target RAM addresses.
// __S_romp is defined in the linker command file, it is a table of RomInfo structures.
// The final entry in the table has all-zero fields.
void __copy_rom_sections_to_ram(void)
{
	int index;

	if(__S_romp==0)
		return;

	// Go through the entire table, copying sections from ROM to RAM.
	for(index=0;__S_romp[index].Source!=0||__S_romp[index].Target!=0||__S_romp[index].Size!=0;++index)
		__copy_rom_section(__S_romp[index].Target, __S_romp[index].Source, __S_romp[index].Size);
}

void __zero_fill_bss(void)
{
	uint32_t len=__END_BSS-__START_BSS;
	uint32_t dst=(uint32_t)__START_BSS;

	while(len--)
		*((uint8_t *)dst++)=0;
}

void __init_hardware(void)
{
	SCB_VTOR=(uint32_t)(&__vect_table); /* Set the interrupt vector table position */

	/* Disable the WDOG module */
	WDOG_UNLOCK=WDOG_UNLOCK_WDOGUNLOCK(0xC520); /* Key 1 */
	WDOG_UNLOCK=WDOG_UNLOCK_WDOGUNLOCK(0xD928); /* Key 2 */
	WDOG_STCTRLH=WDOG_STCTRLH_BYTESEL(0x00)|WDOG_STCTRLH_WAITEN_MASK|WDOG_STCTRLH_STOPEN_MASK|WDOG_STCTRLH_ALLOWUPDATE_MASK|WDOG_STCTRLH_CLKSRC_MASK|0x0100U;

	/* System clock initialization */
	SIM_CLKDIV1=SIM_CLKDIV1_OUTDIV1(0x00)|SIM_CLKDIV1_OUTDIV2(0x01)|SIM_CLKDIV1_OUTDIV4(0x03); /* Set the system prescalers to safe value */
	SIM_SCGC5|=SIM_SCGC5_PORTD_MASK|SIM_SCGC5_PORTC_MASK|SIM_SCGC5_PORTB_MASK|SIM_SCGC5_PORTA_MASK;   /* Enable clock gate for ports to enable pin routing */

	if((PMC_REGSC&PMC_REGSC_ACKISO_MASK)!=0x0U)
		PMC_REGSC|=PMC_REGSC_ACKISO_MASK; /* Release IO pads after wakeup from VLLS mode. */

	SIM_CLKDIV1=SIM_CLKDIV1_OUTDIV1(0x00)|SIM_CLKDIV1_OUTDIV2(0x01)|SIM_CLKDIV1_OUTDIV4(0x02); /* Update system prescalers */
	SIM_SOPT2|=SIM_SOPT2_PLLFLLSEL_MASK; /* Select PLL as a clock source for various peripherals */
	SIM_SOPT1|=SIM_SOPT1_OSC32KSEL(0x03); /* LPO 1kHz oscillator drives 32 kHz clock for various peripherals */

	PORTA_PCR18&=~(PORT_PCR_ISF_MASK|PORT_PCR_MUX(0x07));
	PORTA_PCR19&=~(PORT_PCR_ISF_MASK|PORT_PCR_MUX(0x07));

	/* Switch to FEI Mode */
	MCG_C1=MCG_C1_CLKS(0x00)|MCG_C1_FRDIV(0x00)|MCG_C1_IREFS_MASK|MCG_C1_IRCLKEN_MASK;
	MCG_C2=MCG_C2_RANGE0(0x02)|MCG_C2_EREFS0_MASK;
	MCG_C4=(MCG_C4&~MCG_C4_DRST_DRS(0x01))|(MCG_C4_DMX32_MASK|MCG_C4_DRST_DRS(0x02));

	OSC_CR=OSC_CR_ERCLKEN_MASK;

	MCG_C7&=~MCG_C7_OSCSEL_MASK;
	MCG_C5=MCG_C5_PRDIV0(0x07);
	MCG_C5|=MCG_C5_PLLCLKEN0_MASK; /* Enable the PLL */
	MCG_C6=MCG_C6_VDIV0(0x00);

	while((MCG_S&MCG_S_IREFST_MASK)==0x00U); /* Check that the source of the FLL reference clock is the internal reference clock. */
	while((MCG_S&0x0CU)!=0x00U); /* Wait until output of the FLL is selected */
}

void Cpu_SetBASEPRI(uint32_t Level)
{
	__asm__ volatile ("msr basepri, %[input]"::[input] "r" (Level):);
}

void __low_level_init(void)
{
	/* Initialization of the RCM module */
	RCM_RPFW&=~(RCM_RPFW_RSTFLTSEL(0x1F));
	RCM_RPFC&=~(RCM_RPFC_RSTFLTSS_MASK|RCM_RPFC_RSTFLTSRW(0x03));

	/* Initialization of the PMC module */
	PMC_LVDSC1=(PMC_LVDSC1&~(PMC_LVDSC1_LVDIE_MASK|PMC_LVDSC1_LVDV(0x03)))|(PMC_LVDSC1_LVDACK_MASK|PMC_LVDSC1_LVDRE_MASK);
	PMC_LVDSC2=(PMC_LVDSC2&~(PMC_LVDSC2_LVWIE_MASK|PMC_LVDSC2_LVWV(0x03)))|PMC_LVDSC2_LVWACK_MASK;
	PMC_REGSC&=~(PMC_REGSC_BGEN_MASK|PMC_REGSC_ACKISO_MASK|PMC_REGSC_BGBE_MASK);

	SMC_PMPROT=0x00;

	NVICIP73=NVIC_IP_PRI73(0x00); // USB_ISR priority
	NVICIP20=NVIC_IP_PRI20(0x00);

	GPIOB_PDDR&=~GPIO_PDDR_PDD(0x03);
	GPIOC_PDDR&=~GPIO_PDDR_PDD(0xC0);

	// Set up SysTick
	SYST_RVR=(72000000/1000000)-1; // uS resoultion
	SYST_CSR=SysTick_CSR_CLKSOURCE_MASK|SysTick_CSR_TICKINT_MASK|SysTick_CSR_ENABLE_MASK;

	// Enable interrupts of the given priority level
	Cpu_SetBASEPRI(0);

	// Enable interrupts
	__asm__ volatile ("CPSIE i");
}

void __thumb_startup(void)
{
	int addr=(int)__SP_INIT;

	// setup hardware
	__init_hardware();

	// setup the stack before we attempt anything else
	// skip stack setup if __SP_INIT is 0
	// assume sp is already setup.
	__asm(
		"mov r0,%0\n\t"
		"cmp r0,#0\n\t"
		"beq skip_sp\n\t"
		"mov sp,r0\n\t"
		"sub sp,#4\n\t"
		"mov r0,#0\n\t"
		"mvn r0,r0\n\t"
		"str r0,[sp,#0]\n\t"
		"add sp,#4\n\t"
		"skip_sp:\n\t"
		::"r"(addr));

	// zero-fill the .bss section
	__zero_fill_bss();

	// SUPPORT_ROM_TO_RAM
	__copy_rom_sections_to_ram();

	__low_level_init();

	SysTick=0;

	main();

	// should never get here
	while(1);
}
