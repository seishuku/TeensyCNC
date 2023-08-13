// TeensyCNC
// Copyright 2016 Matt Williams
//
// Motor PWM channel setup and duty cycle setting

#include "MIMXRT1062.h"
#include "pwm.h"

extern volatile uint32_t F_BUS_ACTUAL;

static const uint32_t Freq=40000;
static uint16_t Cycles=0;

void PWM_Init(void)
{
	Cycles=(uint16_t)(F_BUS_ACTUAL/Freq);

	// PWM2 setup:
	CCM->CCGR4|=CCM_CCGR4_CG9_MASK;		// PWM2 clock gate

	// Set up fault registers
	PWM2->FCTRL=PWM_FCTRL_FAUTO(0xF)|PWM_FCTRL_FLVL(0xF);
	PWM2->FSTS=PWM_FSTS_FFLAG_MASK;
	PWM2->FFILT=0;

	// Clear load ok flag
	PWM2->MCTRL|=PWM_MCTRL_CLDOK(15);

	// Submodule 0, Select IPB clock source and independant mode
	PWM2->SM[0].CTRL2=PWM_CTRL2_CLK_SEL(0)|PWM_CTRL2_INDEP_MASK|PWM_CTRL2_WAITEN_MASK|PWM_CTRL2_DBGEN_MASK;
	PWM2->SM[0].CTRL=PWM_CTRL_FULL_MASK|PWM_CTRL_PRSC(1);
	// Set up no duty at "cycles" frequency
	PWM2->SM[0].INIT=0;
	PWM2->SM[0].VAL0=Cycles>>1;
	PWM2->SM[0].VAL1=Cycles-1;
	PWM2->SM[0].VAL2=0;
	PWM2->SM[0].VAL3=0;
	// Set pin muxes
	IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_06]=IOMUXC_SW_MUX_CTL_PAD_MUX_MODE(1);	// GPIO_EMC_06, mux 1 (PWM)

	// Submodule 1, Select IPB clock source and independant mode
	PWM2->SM[1].CTRL2=PWM_CTRL2_CLK_SEL(0)|PWM_CTRL2_INDEP_MASK|PWM_CTRL2_WAITEN_MASK|PWM_CTRL2_DBGEN_MASK;
	PWM2->SM[1].CTRL=PWM_CTRL_FULL_MASK|PWM_CTRL_PRSC(1);
	// Set up no duty at "cycles" frequency
	PWM2->SM[1].INIT=0;
	PWM2->SM[1].VAL0=Cycles>>1;
	PWM2->SM[1].VAL1=Cycles-1;
	PWM2->SM[1].VAL2=0;
	PWM2->SM[1].VAL3=0;
	// Set pin muxes
	IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_08]=IOMUXC_SW_MUX_CTL_PAD_MUX_MODE(1);	// GPIO_EMC_08, mux 1 (PWM)

	// Enable out on submodules 0 and 1
	PWM2->OUTEN|=PWM_OUTEN_PWMA_EN(1)|PWM_OUTEN_PWMA_EN(2);
	// Set load ok on all modules and run
	PWM2->MCTRL|=PWM_MCTRL_LDOK(15)|PWM_MCTRL_RUN(15);

	// PWM4 setup:
	CCM->CCGR4|=CCM_CCGR4_CG11_MASK;	// PWM4 clock gate

	// Set up fault registers
	PWM4->FCTRL=PWM_FCTRL_FAUTO(0xF)|PWM_FCTRL_FLVL(0xF);
	PWM4->FSTS=PWM_FSTS_FFLAG_MASK;
	PWM4->FFILT=0;

	// Clear load ok flag
	PWM4->MCTRL|=PWM_MCTRL_CLDOK(15);

	// Submodule 0, Select IPB clock source and independant mode
	PWM4->SM[0].CTRL2=PWM_CTRL2_CLK_SEL(0)|PWM_CTRL2_INDEP_MASK|PWM_CTRL2_WAITEN_MASK|PWM_CTRL2_DBGEN_MASK;
	PWM4->SM[0].CTRL=PWM_CTRL_FULL_MASK|PWM_CTRL_PRSC(1);
	// Set up no duty at "cycles" frequency
	PWM4->SM[0].INIT=0;
	PWM4->SM[0].VAL0=Cycles>>1;
	PWM4->SM[0].VAL1=Cycles-1;
	PWM4->SM[0].VAL2=0;
	PWM4->SM[0].VAL3=0;
	// Set pin muxes
	IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_08]=IOMUXC_SW_MUX_CTL_PAD_MUX_MODE(1);	// GPIO_AD_B1_08, mux 1 (PWM)

	// Submodule 1, Select IPB clock source and independant mode
	PWM4->SM[1].CTRL2=PWM_CTRL2_CLK_SEL(0)|PWM_CTRL2_INDEP_MASK|PWM_CTRL2_WAITEN_MASK|PWM_CTRL2_DBGEN_MASK;
	PWM4->SM[1].CTRL=PWM_CTRL_FULL_MASK|PWM_CTRL_PRSC(1);
	// Set up no duty at "cycles" frequency
	PWM4->SM[1].INIT=0;
	PWM4->SM[1].VAL0=Cycles>>1;
	PWM4->SM[1].VAL1=Cycles-1;
	PWM4->SM[1].VAL2=0;
	PWM4->SM[1].VAL3=0;
	// Set pin muxes
	IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_09]=IOMUXC_SW_MUX_CTL_PAD_MUX_MODE(1);	// GPIO_AD_B1_09, mux 1 (PWM)

	// Enable out on submodules 0 and 1
	PWM4->OUTEN|=PWM_OUTEN_PWMA_EN(1)|PWM_OUTEN_PWMA_EN(2);
	// Set load ok on all modules and run
	PWM4->MCTRL|=PWM_MCTRL_LDOK(15)|PWM_MCTRL_RUN(15);
}

void PWM_SetRatio(uint8_t Channel, uint16_t Ratio)
{
	// Calculate the duty from cycles
	uint16_t Pulse=(Cycles*Ratio)/0xFFFF;

	// Setup the PWM dutycycle
	switch(Channel)
	{
		case 0:
		{
			PWM2->MCTRL|=PWM_MCTRL_CLDOK(0x1);
			PWM2->SM[0].VAL3=Pulse;
			PWM2->MCTRL|=PWM_MCTRL_LDOK(0x1);
			break;
		}

		case 1:
		{
			PWM2->MCTRL|=PWM_MCTRL_CLDOK(0x2);
			PWM2->SM[1].VAL3=Pulse;
			PWM2->MCTRL|=PWM_MCTRL_LDOK(0x2);
			break;
		}

		case 2:
		{
			PWM4->MCTRL|=PWM_MCTRL_CLDOK(0x1);
			PWM4->SM[0].VAL3=Pulse;
			PWM4->MCTRL|=PWM_MCTRL_LDOK(0x1);
			break;
		}

		case 3:
		{
			PWM4->MCTRL|=PWM_MCTRL_CLDOK(0x2);
			PWM4->SM[1].VAL3=Pulse;
			PWM4->MCTRL|=PWM_MCTRL_LDOK(0x2);
			break;
		}

		default:
			break;
	}
}
