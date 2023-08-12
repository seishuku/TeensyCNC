#include <stdint.h>
#include "MIMXRT1062.h"
#include "CMSIS/core_cm7.h"

void ResetISR(void);
void SysTick_Handler(void);
void USB_OTG1_IRQHandler(void);
void PIT_IRQHandler(void);
void GPIO6_7_8_9_IRQHandler(void);

__attribute__((naked)) void unused_interrupt_vector(void)
{
}

extern void _estack(void);

__attribute__((used, aligned(1024), section(".vectors"))) void (*const _VectorTable[])(void)=
{
	&_estack,							// The initial stack pointer
	ResetISR,							// The reset handler
	unused_interrupt_vector,			// NMI Handler
	unused_interrupt_vector,			// Hard Fault Handler
	unused_interrupt_vector,			// MPU Fault Handler
	unused_interrupt_vector,			// Bus Fault Handler
	unused_interrupt_vector,			// Usage Fault Handler
	0,									// Reserved
	0,									// Reserved
	0,									// Reserved
	0,									// Reserved
	unused_interrupt_vector,			// SVCall Handler
	unused_interrupt_vector,			// Debug Monitor Handler
	0,									// Reserved
	unused_interrupt_vector,			// PendSV Handler
	SysTick_Handler,					// SysTick Handler

	// Chip Level - MIMXRT1062
	unused_interrupt_vector,			// 16 : DMA channel 0/16 transfer complete
	unused_interrupt_vector,			// 17 : DMA channel 1/17 transfer complete
	unused_interrupt_vector,			// 18 : DMA channel 2/18 transfer complete
	unused_interrupt_vector,			// 19 : DMA channel 3/19 transfer complete
	unused_interrupt_vector,			// 20 : DMA channel 4/20 transfer complete
	unused_interrupt_vector,			// 21 : DMA channel 5/21 transfer complete
	unused_interrupt_vector,			// 22 : DMA channel 6/22 transfer complete
	unused_interrupt_vector,			// 23 : DMA channel 7/23 transfer complete
	unused_interrupt_vector,			// 24 : DMA channel 8/24 transfer complete
	unused_interrupt_vector,			// 25 : DMA channel 9/25 transfer complete
	unused_interrupt_vector,			// 26 : DMA channel 10/26 transfer complete
	unused_interrupt_vector,			// 27 : DMA channel 11/27 transfer complete
	unused_interrupt_vector,			// 28 : DMA channel 12/28 transfer complete
	unused_interrupt_vector,			// 29 : DMA channel 13/29 transfer complete
	unused_interrupt_vector,			// 30 : DMA channel 14/30 transfer complete
	unused_interrupt_vector,			// 31 : DMA channel 15/31 transfer complete
	unused_interrupt_vector,			// 32 : DMA error interrupt channels 0-15 / 16-31
	unused_interrupt_vector,			// 33 : CTI0_Error
	unused_interrupt_vector,			// 34 : CTI1_Error
	unused_interrupt_vector,			// 35 : CorePlatform exception IRQ
	unused_interrupt_vector,			// 36 : LPUART1 TX interrupt and RX interrupt
	unused_interrupt_vector,			// 37 : LPUART2 TX interrupt and RX interrupt
	unused_interrupt_vector,			// 38 : LPUART3 TX interrupt and RX interrupt
	unused_interrupt_vector,			// 39 : LPUART4 TX interrupt and RX interrupt
	unused_interrupt_vector,			// 40 : LPUART5 TX interrupt and RX interrupt
	unused_interrupt_vector,			// 41 : LPUART6 TX interrupt and RX interrupt
	unused_interrupt_vector,			// 42 : LPUART7 TX interrupt and RX interrupt
	unused_interrupt_vector,			// 43 : LPUART8 TX interrupt and RX interrupt
	unused_interrupt_vector,			// 44 : LPI2C1 interrupt
	unused_interrupt_vector,			// 45 : LPI2C2 interrupt
	unused_interrupt_vector,			// 46 : LPI2C3 interrupt
	unused_interrupt_vector,			// 47 : LPI2C4 interrupt
	unused_interrupt_vector,			// 48 : LPSPI1 single interrupt vector for all sources
	unused_interrupt_vector,			// 49 : LPSPI2 single interrupt vector for all sources
	unused_interrupt_vector,			// 50 : LPSPI3 single interrupt vector for all sources
	unused_interrupt_vector,			// 51 : LPSPI4  single interrupt vector for all sources
	unused_interrupt_vector,			// 52 : CAN1 interrupt
	unused_interrupt_vector,			// 53 : CAN2 interrupt
	unused_interrupt_vector,			// 54 : FlexRAM address out of range Or access hit IRQ
	unused_interrupt_vector,			// 55 : Keypad interrupt
	unused_interrupt_vector,			// 56 : TSC interrupt
	unused_interrupt_vector,			// 57 : GPR interrupt
	unused_interrupt_vector,			// 58 : LCDIF interrupt
	unused_interrupt_vector,			// 59 : CSI interrupt
	unused_interrupt_vector,			// 60 : PXP interrupt
	unused_interrupt_vector,			// 61 : WDOG2 interrupt
	unused_interrupt_vector,			// 62 : SRTC Consolidated Interrupt. Non TZ
	unused_interrupt_vector,			// 63 : SRTC Security Interrupt. TZ
	unused_interrupt_vector,			// 64 : ON-OFF button press shorter than 5 secs (pulse event)
	unused_interrupt_vector,			// 65 : CSU interrupt
	unused_interrupt_vector,			// 66 : DCP_IRQ interrupt
	unused_interrupt_vector,			// 67 : DCP_VMI_IRQ interrupt
	0,									// 68 : Reserved interrupt
	unused_interrupt_vector,			// 69 : TRNG interrupt
	unused_interrupt_vector,			// 70 : SJC interrupt
	unused_interrupt_vector,			// 71 : BEE interrupt
	unused_interrupt_vector,			// 72 : SAI1 interrupt
	unused_interrupt_vector,			// 73 : SAI1 interrupt
	unused_interrupt_vector,			// 74 : SAI3 interrupt
	unused_interrupt_vector,			// 75 : SAI3 interrupt
	unused_interrupt_vector,			// 76 : SPDIF interrupt
	unused_interrupt_vector,			// 77 : Brown-out event interrupt
	0,									// 78 : Reserved interrupt
	unused_interrupt_vector,			// 79 : TempSensor low/high interrupt
	unused_interrupt_vector,			// 80 : TempSensor panic interrupt
	unused_interrupt_vector,			// 81 : USBPHY (UTMI0), Interrupt
	unused_interrupt_vector,			// 82 : USBPHY (UTMI1), Interrupt
	unused_interrupt_vector,			// 83 : ADC1 interrupt
	unused_interrupt_vector,			// 84 : ADC2 interrupt
	unused_interrupt_vector,			// 85 : DCDC interrupt
	0,									// 86 : Reserved interrupt
	unused_interrupt_vector,			// 87 : Combined interrupt indication for GPIO10 signal 0 throughout 31
	unused_interrupt_vector,			// 88 : Active HIGH Interrupt from INT0 from GPIO
	unused_interrupt_vector,			// 89 : Active HIGH Interrupt from INT1 from GPIO
	unused_interrupt_vector,			// 90 : Active HIGH Interrupt from INT2 from GPIO
	unused_interrupt_vector,			// 91 : Active HIGH Interrupt from INT3 from GPIO
	unused_interrupt_vector,			// 92 : Active HIGH Interrupt from INT4 from GPIO
	unused_interrupt_vector,			// 93 : Active HIGH Interrupt from INT5 from GPIO
	unused_interrupt_vector,			// 94 : Active HIGH Interrupt from INT6 from GPIO
	unused_interrupt_vector,			// 95 : Active HIGH Interrupt from INT7 from GPIO
	unused_interrupt_vector,			// 96 : Combined interrupt indication for GPIO1 signal 0 throughout 15
	unused_interrupt_vector,			// 97 : Combined interrupt indication for GPIO1 signal 16 throughout 31
	unused_interrupt_vector,			// 98 : Combined interrupt indication for GPIO2 signal 0 throughout 15
	unused_interrupt_vector,			// 99 : Combined interrupt indication for GPIO2 signal 16 throughout 31
	unused_interrupt_vector,			// 100: Combined interrupt indication for GPIO3 signal 0 throughout 15
	unused_interrupt_vector,			// 101: Combined interrupt indication for GPIO3 signal 16 throughout 31
	unused_interrupt_vector,			// 102: Combined interrupt indication for GPIO4 signal 0 throughout 15
	unused_interrupt_vector,			// 103: Combined interrupt indication for GPIO4 signal 16 throughout 31
	unused_interrupt_vector,			// 104: Combined interrupt indication for GPIO5 signal 0 throughout 15
	unused_interrupt_vector,			// 105: Combined interrupt indication for GPIO5 signal 16 throughout 31
	unused_interrupt_vector,			// 106: FLEXIO1 interrupt
	unused_interrupt_vector,			// 107: FLEXIO2 interrupt
	unused_interrupt_vector,			// 108: WDOG1 interrupt
	unused_interrupt_vector,			// 109: RTWDOG interrupt
	unused_interrupt_vector,			// 110: EWM interrupt
	unused_interrupt_vector,			// 111: CCM IRQ1 interrupt
	unused_interrupt_vector,			// 112: CCM IRQ2 interrupt
	unused_interrupt_vector,			// 113: GPC interrupt
	unused_interrupt_vector,			// 114: SRC interrupt
	0,									// 115: Reserved interrupt
	unused_interrupt_vector,			// 116: GPT1 interrupt
	unused_interrupt_vector,			// 117: GPT2 interrupt
	unused_interrupt_vector,			// 118: PWM1 capture 0, compare 0, or reload 0 interrupt
	unused_interrupt_vector,			// 119: PWM1 capture 1, compare 1, or reload 0 interrupt
	unused_interrupt_vector,			// 120: PWM1 capture 2, compare 2, or reload 0 interrupt
	unused_interrupt_vector,			// 121: PWM1 capture 3, compare 3, or reload 0 interrupt
	unused_interrupt_vector,			// 122: PWM1 fault or reload error interrupt
	unused_interrupt_vector,			// 123: FlexSPI2 interrupt
	unused_interrupt_vector,			// 124: FlexSPI0 interrupt
	unused_interrupt_vector,			// 125: SEMC interrupt
	unused_interrupt_vector,			// 126: USDHC1 interrupt
	unused_interrupt_vector,			// 127: USDHC2 interrupt
	unused_interrupt_vector,			// 128: USBO2 USB OTG2
	USB_OTG1_IRQHandler,				// 129: USBO2 USB OTG1
	unused_interrupt_vector,			// 130: ENET interrupt
	unused_interrupt_vector,			// 131: ENET_1588_Timer interrupt
	unused_interrupt_vector,			// 132: XBARA1 output signal 0, 1 interrupt
	unused_interrupt_vector,			// 133: XBARA1 output signal 2, 3 interrupt
	unused_interrupt_vector,			// 134: ADCETC IRQ0 interrupt
	unused_interrupt_vector,			// 135: ADCETC IRQ1 interrupt
	unused_interrupt_vector,			// 136: ADCETC IRQ2 interrupt
	unused_interrupt_vector,			// 137: ADCETC Error IRQ interrupt
	PIT_IRQHandler,						// 138: PIT interrupt
	unused_interrupt_vector,			// 139: ACMP interrupt
	unused_interrupt_vector,			// 140: ACMP interrupt
	unused_interrupt_vector,			// 141: ACMP interrupt
	unused_interrupt_vector,			// 142: ACMP interrupt
	0,									// 143: Reserved interrupt
	0,									// 144: Reserved interrupt
	unused_interrupt_vector,			// 145: ENC1 interrupt
	unused_interrupt_vector,			// 146: ENC2 interrupt
	unused_interrupt_vector,			// 147: ENC3 interrupt
	unused_interrupt_vector,			// 148: ENC4 interrupt
	unused_interrupt_vector,			// 149: TMR1 interrupt
	unused_interrupt_vector,			// 150: TMR2 interrupt
	unused_interrupt_vector,			// 151: TMR3 interrupt
	unused_interrupt_vector,			// 152: TMR4 interrupt
	unused_interrupt_vector,			// 153: PWM2 capture 0, compare 0, or reload 0 interrupt
	unused_interrupt_vector,			// 154: PWM2 capture 1, compare 1, or reload 0 interrupt
	unused_interrupt_vector,			// 155: PWM2 capture 2, compare 2, or reload 0 interrupt
	unused_interrupt_vector,			// 156: PWM2 capture 3, compare 3, or reload 0 interrupt
	unused_interrupt_vector,			// 157: PWM2 fault or reload error interrupt
	unused_interrupt_vector,			// 158: PWM3 capture 0, compare 0, or reload 0 interrupt
	unused_interrupt_vector,			// 159: PWM3 capture 1, compare 1, or reload 0 interrupt
	unused_interrupt_vector,			// 160: PWM3 capture 2, compare 2, or reload 0 interrupt
	unused_interrupt_vector,			// 161: PWM3 capture 3, compare 3, or reload 0 interrupt
	unused_interrupt_vector,			// 162: PWM3 fault or reload error interrupt
	unused_interrupt_vector,			// 164: PWM4 capture 1, compare 1, or reload 0 interrupt
	unused_interrupt_vector,			// 165: PWM4 capture 2, compare 2, or reload 0 interrupt
	unused_interrupt_vector,			// 163: PWM4 capture 0, compare 0, or reload 0 interrupt
	unused_interrupt_vector,			// 166: PWM4 capture 3, compare 3, or reload 0 interrupt
	unused_interrupt_vector,			// 167: PWM4 fault or reload error interrupt
	unused_interrupt_vector,			// 168: ENET2 interrupt
	unused_interrupt_vector,			// 169: ENET2_1588_Timer interrupt
	unused_interrupt_vector,			// 170: CAN3 interrupt
	0,									// 171: Reserved interrupt
	unused_interrupt_vector,			// 172: FLEXIO3 interrupt
	GPIO6_7_8_9_IRQHandler,				// 173: GPIO6, GPIO7, GPIO8, GPIO9 interrupt
};

volatile uint32_t systick_millis_count=0;
volatile uint32_t systick_cycle_count=0;

void SysTick_Handler(void)
{
	systick_cycle_count=DWT->CYCCNT;
	systick_millis_count++;
}

volatile uint32_t F_CPU_ACTUAL=396000000;
volatile uint32_t F_BUS_ACTUAL=132000000;
uint32_t scale_cpu_cycles_to_microseconds=0;

uint32_t set_arm_clock(uint32_t frequency)
{
	uint32_t cbcdr=CCM->CBCDR; // pg 1021
	uint32_t cbcmr=CCM->CBCMR; // pg 1023
	uint32_t dcdc=DCDC->REG3;

	// compute required voltage
	uint32_t voltage=1150; // default = 1.15V

	if(frequency>528000000)
		voltage=1250; // 1.25V
	else if(frequency<=24000000)
		voltage=950; // 0.95

	// if voltage needs to increase, do it before switch clock speed
	CCM->CCGR6|=CCM_CCGR6_CG3_MASK;

	if((dcdc&DCDC_REG3_TRG_MASK)<DCDC_REG3_TRG((voltage-800)/25))
	{
		dcdc&=~DCDC_REG3_TRG_MASK;
		dcdc|=DCDC_REG3_TRG((voltage-800)/25);
		DCDC->REG3=dcdc;

		while(!(DCDC->REG0&DCDC_REG0_STS_DC_OK_MASK)); // wait voltage settling
	}

	if(!(cbcdr&CCM_CBCDR_PERIPH_CLK_SEL_MASK))
	{
		const uint32_t need1s=CCM_ANALOG_PLL_USB1_TOG_ENABLE_MASK|CCM_ANALOG_PLL_USB1_POWER_MASK|CCM_ANALOG_PLL_USB1_LOCK_MASK|CCM_ANALOG_PLL_USB1_EN_USB_CLKS_MASK;
		uint32_t sel, div;

		if((CCM_ANALOG->PLL_USB1&need1s)==need1s)
		{
			sel=0;
			div=3; // divide down to 120 MHz, so IPG is ok even if IPG_PODF=0
		}
		else
		{
			sel=1;
			div=0;
		}

		if((cbcdr&CCM_CBCDR_PERIPH_CLK2_PODF_MASK)!=CCM_CBCDR_PERIPH_CLK2_PODF(div))
		{
			// PERIPH_CLK2 divider needs to be changed
			cbcdr&=~CCM_CBCDR_PERIPH_CLK2_PODF_MASK;
			cbcdr|=CCM_CBCDR_PERIPH_CLK2_PODF(div);
			CCM->CBCDR=cbcdr;
		}

		if((cbcmr&CCM_CBCMR_PERIPH_CLK2_SEL_MASK)!=CCM_CBCMR_PERIPH_CLK2_SEL(sel))
		{
			// PERIPH_CLK2 source select needs to be changed
			cbcmr&=~CCM_CBCMR_PERIPH_CLK2_SEL_MASK;
			cbcmr|=CCM_CBCMR_PERIPH_CLK2_SEL(sel);
			CCM->CBCMR=cbcmr;

			while(CCM->CDHIPR&CCM_CDHIPR_PERIPH2_CLK_SEL_BUSY_MASK); // wait
		}

		// switch over to PERIPH_CLK2
		cbcdr|=CCM_CBCDR_PERIPH_CLK_SEL_MASK;
		CCM->CBCDR=cbcdr;

		while(CCM->CDHIPR&CCM_CDHIPR_PERIPH_CLK_SEL_BUSY_MASK); // wait
	}

	// DIV_SELECT: 54-108 = official range 648 to 1296 in 12 MHz steps
	uint32_t div_arm=1, div_ahb=1;

	while(frequency*div_arm*div_ahb<648000000)
	{
		if(div_arm<8)
			div_arm++;
		else
		{
			if(div_ahb<5)
			{
				div_ahb++;
				div_arm=1;
			}
			else
				break;
		}
	}

	uint32_t mult=(frequency*div_arm*div_ahb+6000000)/12000000;

	if(mult>108)
		mult=108;

	if(mult<54)
		mult=54;

	frequency=mult*12000000/div_arm/div_ahb;

	const uint32_t arm_pll_mask=CCM_ANALOG_PLL_ARM_LOCK_MASK|CCM_ANALOG_PLL_ARM_BYPASS_MASK|CCM_ANALOG_PLL_ARM_ENABLE_MASK|CCM_ANALOG_PLL_ARM_POWERDOWN_MASK|CCM_ANALOG_PLL_ARM_DIV_SELECT_MASK;

	if((CCM_ANALOG->PLL_ARM&arm_pll_mask)!=(CCM_ANALOG_PLL_ARM_LOCK_MASK|CCM_ANALOG_PLL_ARM_ENABLE_MASK|CCM_ANALOG_PLL_ARM_DIV_SELECT(mult)))
	{
		CCM_ANALOG->PLL_ARM=CCM_ANALOG_PLL_ARM_POWERDOWN_MASK;
		CCM_ANALOG->PLL_ARM=CCM_ANALOG_PLL_ARM_ENABLE_MASK|CCM_ANALOG_PLL_ARM_DIV_SELECT(mult);

		while(!(CCM_ANALOG->PLL_ARM&CCM_ANALOG_PLL_ARM_LOCK_MASK)); // wait for lock
	}

	if((CCM->CACRR&CCM_CACRR_ARM_PODF_MASK)!=(div_arm-1))
	{
		CCM->CACRR=CCM_CACRR_ARM_PODF(div_arm-1);

		while(CCM->CDHIPR&CCM_CDHIPR_ARM_PODF_BUSY_MASK); // wait
	}

	if((cbcdr&CCM_CBCDR_AHB_PODF_MASK)!=CCM_CBCDR_AHB_PODF(div_ahb-1))
	{
		cbcdr&=~CCM_CBCDR_AHB_PODF_MASK;
		cbcdr|=CCM_CBCDR_AHB_PODF(div_ahb-1);
		CCM->CBCDR=cbcdr;
		while(CCM->CDHIPR&CCM_CDHIPR_AHB_PODF_BUSY_MASK); // wait
	}

	uint32_t div_ipg=(frequency+149999999)/150000000;

	if(div_ipg>4)
		div_ipg=4;

	if((cbcdr&CCM_CBCDR_IPG_PODF_MASK)!=(CCM_CBCDR_IPG_PODF(div_ipg-1)))
	{
		cbcdr&=~CCM_CBCDR_IPG_PODF_MASK;
		cbcdr|=CCM_CBCDR_IPG_PODF(div_ipg-1);
		CCM->CBCDR=cbcdr;
	}

	CCM->CBCDR&=~CCM_CBCDR_PERIPH_CLK_SEL_MASK;

	while(CCM->CDHIPR&CCM_CDHIPR_PERIPH_CLK_SEL_BUSY_MASK); // wait

	F_CPU_ACTUAL=frequency;
	F_BUS_ACTUAL=frequency/div_ipg;
	scale_cpu_cycles_to_microseconds=0xFFFFFFFFu/(uint32_t)(frequency/1000000u);

	if((dcdc&DCDC_REG3_TRG_MASK)>DCDC_REG3_TRG((voltage-800)/25))
	{
		dcdc&=~DCDC_REG3_TRG_MASK;
		dcdc|=DCDC_REG3_TRG((voltage-800)/25);
		DCDC->REG3=dcdc;

		while(!(DCDC->REG0&DCDC_REG0_STS_DC_OK_MASK)); // wait voltage settling
	}

	// Set PerClock divider (used by PIT) to 2 (value-1)
	CCM->CSCMR1=(CCM->CSCMR1&~CCM_CSCMR1_PERCLK_PODF_MASK)|CCM_CSCMR1_PERCLK_PODF(1);

	// PerClock mux select
	CCM->CSCMR1=(CCM->CSCMR1&~CCM_CSCMR1_PERCLK_CLK_SEL_MASK)|CCM_CSCMR1_PERCLK_CLK_SEL(0);

	return frequency;
}

__attribute__((section(".flashmem"))) void usb_pll_start()
{
	while(1)
	{
		uint32_t n=CCM_ANALOG->PLL_USB1; // pg 759

		if(n&CCM_ANALOG_PLL_USB1_DIV_SELECT_MASK)
		{
			CCM_ANALOG->PLL_USB1_CLR=CCM_ANALOG_PLL_USB1_BYPASS_CLK_SRC_MASK; // bypass 24 MHz
			CCM_ANALOG->PLL_USB1_SET=CCM_ANALOG_PLL_USB1_BYPASS_MASK; // bypass
			CCM_ANALOG->PLL_USB1_CLR=CCM_ANALOG_PLL_USB1_POWER_MASK|CCM_ANALOG_PLL_USB1_DIV_SELECT_MASK|CCM_ANALOG_PLL_USB1_ENABLE_MASK|CCM_ANALOG_PLL_USB1_EN_USB_CLKS_MASK; // select clock
			continue;
		}

		if(!(n&CCM_ANALOG_PLL_USB1_ENABLE_MASK))
		{
			// TODO: should this be done so early, or later??
			CCM_ANALOG->PLL_USB1_SET=CCM_ANALOG_PLL_USB1_ENABLE_MASK;
			continue;
		}

		if(!(n&CCM_ANALOG_PLL_USB1_POWER_MASK))
		{
			CCM_ANALOG->PLL_USB1_SET=CCM_ANALOG_PLL_USB1_POWER_MASK;
			continue;
		}

		if(!(n&CCM_ANALOG_PLL_USB1_LOCK_MASK))
			continue;

		if(n&CCM_ANALOG_PLL_USB1_BYPASS_MASK)
		{
			CCM_ANALOG->PLL_USB1_CLR=CCM_ANALOG_PLL_USB1_BYPASS_MASK;
			continue;
		}

		if(!(n&CCM_ANALOG_PLL_USB1_EN_USB_CLKS_MASK))
		{
			CCM_ANALOG->PLL_USB1_SET=CCM_ANALOG_PLL_USB1_EN_USB_CLKS_MASK;
			continue;
		}

		return; // everything is as it should be  :-)
	}
}

extern unsigned long _stextload, _stext, _etext;
extern unsigned long _sdataload, _sdata, _edata;
extern unsigned long _sbss, _ebss;
extern unsigned long _flexram_bank_config;

__attribute__((section(".startup"), optimize("no-tree-loop-distribute-patterns"))) static void memory_copy(uint32_t *dest, const uint32_t *src, uint32_t *dest_end)
{
	if(dest==src)
		return;

	while(dest<dest_end)
		*dest++=*src++;
}

__attribute__((section(".startup"), optimize("no-tree-loop-distribute-patterns"))) static void memory_clear(uint32_t *dest, uint32_t *dest_end)
{
	while(dest<dest_end)
		*dest++=0;
}

void main();
void usb_init(void);

__attribute__((section(".startup"), optimize("no-tree-loop-distribute-patterns"), naked)) void ResetISR(void)
{
	// Set vector table
	SCB->VTOR=(uint32_t)_VectorTable;

	// FlexRAM bank configuration
	IOMUXC_GPR->GPR17=(uint32_t)&_flexram_bank_config;
	IOMUXC_GPR->GPR16=0x00000007;
	IOMUXC_GPR->GPR14=0x00AA0000;

	__asm__ volatile ("MSR MSP, %0" ::"r" (&_estack):);

	// Initialize memory
	memory_copy(&_stext, &_stextload, &_etext);
	memory_copy(&_sdata, &_sdataload, &_edata);
	memory_clear(&_sbss, &_ebss);

	// enable FPU
	SCB->CPACR=0x00F00000;

	// Configure core clocks
	set_arm_clock(396000000);

	// Enable cache
	SCB_EnableICache();
	SCB_EnableDCache();

	usb_pll_start();

	// Set up cycle counter
	CoreDebug->DEMCR|=CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CTRL|=DWT_CTRL_CYCCNTENA_Msk; // turn on cycle counter
	systick_cycle_count=DWT->CYCCNT; // compiled 0, corrected w/1st systick

	// Set up SysTick
	NVIC_SetPriority(SysTick_IRQn, (1<<__NVIC_PRIO_BITS)-1);

	SysTick->LOAD=(100000/1000)-1;
	SysTick->VAL=0;
	SysTick->CTRL=SysTick_CTRL_TICKINT_Msk|SysTick_CTRL_ENABLE_Msk;

#if !defined(TEENSY_INIT_USB_DELAY_BEFORE)
#define TEENSY_INIT_USB_DELAY_BEFORE 20
#endif
#if !defined(TEENSY_INIT_USB_DELAY_AFTER)
#define TEENSY_INIT_USB_DELAY_AFTER 280
#endif
	// for background about this startup delay, please see these conversations
	// https://forum.pjrc.com/threads/36606?p=113980&viewfull=1#post113980
	// https://forum.pjrc.com/threads/31290?p=87273&viewfull=1#post87273

	while(systick_millis_count<TEENSY_INIT_USB_DELAY_BEFORE); // wait
	usb_init();
	while(systick_millis_count<TEENSY_INIT_USB_DELAY_AFTER+TEENSY_INIT_USB_DELAY_BEFORE); // wait

	// Call the `main()` function defined in `main.c`.
	main();

	// main should never return...
	while(1);
}

extern unsigned long _flashimagelen;

__attribute__((section(".bootdata"), used)) const uint32_t BootData[3]=
{
	0x60000000,
	(uint32_t)&_flashimagelen,
	0
};


// placeholder for HAB signature
__attribute__((section(".csf"), used)) const uint32_t hab_csf[768];

__attribute__((section(".ivt"), used)) const uint32_t ImageVectorTable[8]=
{
	0x432000D1,					// header
	(uint32_t)&ResetISR,		// program entry
	0,							// reserved
	0,							// dcd
	(uint32_t)BootData,			// abs address of boot data
	(uint32_t)ImageVectorTable, // self
	(uint32_t)hab_csf,			// command sequence file
	0							// reserved
};

// 448 byte common FlexSPI configuration block, 8.6.3.1 page 223 (RT1060 rev 0)
// MCU_Flashloader_Reference_Manual.pdf, 8.2.1, Table 8-2, page 72-75
__attribute__((section(".flashconfig"), used)) uint32_t FlexSPI_NOR_Config[128]=
{
	0x42464346,			// Tag				0x00
	0x56010000,			// Version
	0,					// reserved
	0x00030301,			// columnAdressWidth,dataSetupTime,dataHoldTime,readSampleClkSrc
	0x00000000,			// waitTimeCfgCommands,-,deviceModeCfgEnable
	0,					// deviceModeSeq
	0, 					// deviceModeArg
	0x00000000,			// -,-,-,configCmdEnable
	0, 0, 0, 0,			// configCmdSeqs		0x20
	0, 0, 0, 0,			// cfgCmdArgs			0x30
	0x00000000,			// controllerMiscOption		0x40
	0x00080401,			// lutCustomSeqEnable,serialClkFreq,sflashPadType,deviceType
	0, 0,				// reserved
	0x00200000,			// sflashA1Size			0x50
	0,					// sflashA2Size
	0,					// sflashB1Size
	0,					// sflashB2Size
	0,					// csPadSettingOverride		0x60
	0,					// sclkPadSettingOverride
	0,					// dataPadSettingOverride
	0,					// dqsPadSettingOverride
	0,					// timeoutInMs			0x70
	0,					// commandInterval
	0,					// dataValidTime
	0x00000000,			// busyBitPolarity,busyOffset
	0x0A1804EB,			// lookupTable[0]		0x80
	0x26043206,			// lookupTable[1]
	0,					// lookupTable[2]
	0,					// lookupTable[3]
	0x24040405,			// lookupTable[4]		0x90
	0,					// lookupTable[5]
	0,					// lookupTable[6]
	0,					// lookupTable[7]
	0,					// lookupTable[8]		0xA0
	0,					// lookupTable[9]
	0,					// lookupTable[10]
	0,					// lookupTable[11]
	0x00000406,			// lookupTable[12]		0xB0
	0,					// lookupTable[13]
	0,					// lookupTable[14]
	0,					// lookupTable[15]
	0,					// lookupTable[16]		0xC0
	0,					// lookupTable[17]
	0,					// lookupTable[18]
	0,					// lookupTable[19]
	0x08180420,			// lookupTable[20]		0xD0
	0,					// lookupTable[21]
	0,					// lookupTable[22]
	0,					// lookupTable[23]
	0,					// lookupTable[24]		0xE0
	0,					// lookupTable[25]
	0,					// lookupTable[26]
	0,					// lookupTable[27]
	0,					// lookupTable[28]		0xF0
	0,					// lookupTable[29]
	0,					// lookupTable[30]
	0,					// lookupTable[31]
	0x081804D8,			// lookupTable[32]		0x100
	0,					// lookupTable[33]
	0,					// lookupTable[34]
	0,					// lookupTable[35]
	0x08180402,			// lookupTable[36]		0x110
	0x00002004,			// lookupTable[37]
	0,					// lookupTable[38]
	0,					// lookupTable[39]
	0,					// lookupTable[40]		0x120
	0,					// lookupTable[41]
	0,					// lookupTable[42]
	0,					// lookupTable[43]
	0x00000460,			// lookupTable[44]		0x130
	0,					// lookupTable[45]
	0,					// lookupTable[46]
	0,					// lookupTable[47]
	0,					// lookupTable[48]		0x140
	0,					// lookupTable[49]
	0,					// lookupTable[50]
	0,					// lookupTable[51]
	0,					// lookupTable[52]		0x150
	0,					// lookupTable[53]
	0,					// lookupTable[54]
	0,					// lookupTable[55]
	0,					// lookupTable[56]		0x160
	0,					// lookupTable[57]
	0,					// lookupTable[58]
	0,					// lookupTable[59]
	0,					// lookupTable[60]		0x170
	0,					// lookupTable[61]
	0,					// lookupTable[62]
	0,					// lookupTable[63]
	0,					// LUT 0: Read			0x180
	0,					// LUT 1: ReadStatus
	0,					// LUT 3: WriteEnable
	0,					// LUT 5: EraseSector
	0,					// LUT 9: PageProgram		0x190
	0,					// LUT 11: ChipErase
	0,					// LUT 15: Dummy
	0,					// LUT unused?
	0,					// LUT unused?			0x1A0
	0,					// LUT unused?
	0,					// LUT unused?
	0,					// LUT unused?
	0, 0, 0, 0,			// reserved			0x1B0

	// 64 byte Serial NOR configuration block, 8.6.3.2, page 346
	256,				// pageSize			0x1C0
	4096,				// sectorSize
	1,					// ipCmdSerialClkFreq
	0,					// reserved
	0x00010000,			// block size			0x1D0
	0, 0, 0, 0, 0, 0,	// reserved
	0, 0, 0, 0, 0		// reserved
};
