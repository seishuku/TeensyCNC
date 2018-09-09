// TeensyCNC
// Copyright 2016 Matt Williams
//
// Motor PWM channel setup and duty cycle setting

#include "MK20D7.h"
#include "pwm.h"

void PWM_Init(void)
{
	// Enable module clock
	SIM_SCGC6|=SIM_SCGC6_FTM0_MASK;

	// Set up mode register
	FTM0_MODE=FTM_MODE_FAULTM(0x00)|FTM_MODE_WPDIS_MASK;

	// Clear status and control register
	FTM0_SC=FTM_SC_CLKS(0x00)|FTM_SC_PS(0x00);

	// Clear counter initial register
	FTM0_CNTIN=FTM_CNTIN_INIT(0x00);
	// Reset counter register
	FTM0_CNT=FTM_CNT_COUNT(0x00);

	// Clear channel status and control registers
	FTM0_C0SC=0x00U;
	FTM0_C1SC=0x00U;
	FTM0_C2SC=0x00U;
	FTM0_C3SC=0x00U;
	FTM0_C4SC=0x00U;
	FTM0_C5SC=0x00U;
	FTM0_C6SC=0x00U;
	FTM0_C7SC=0x00U;

	// Set up modulo register
	// Bus clock / FTM0_MOD = PWM Freq
	// Bus clock / PWM Freq = FTM0_MOD
	// 36MHz / 1440 = 25KHz (0.04ms)
	// 36MHz / 900 = 40KHz (0.025ms)
	// 36MHz / 288 = 125KHz (0.008ms)
	FTM0_MOD=FTM_MOD_MOD(900-1);

	// Set up channel status and control register
	FTM0_C0SC=FTM_CnSC_MSB_MASK|FTM_CnSC_ELSB_MASK|FTM_CnSC_ELSA_MASK;
	// Set up channel value register
	FTM0_C0V=FTM_CnV_VAL(0x00);

	// Set up channel status and control register
	FTM0_C1SC=FTM_CnSC_MSB_MASK|FTM_CnSC_ELSB_MASK|FTM_CnSC_ELSA_MASK;
	// Set up channel value register
	FTM0_C1V=FTM_CnV_VAL(0x00);

	// Set up channel status and control register
	FTM0_C5SC=FTM_CnSC_MSB_MASK|FTM_CnSC_ELSB_MASK|FTM_CnSC_ELSA_MASK;
	// Set up channel value register
	FTM0_C5V=FTM_CnV_VAL(0x00);

	// Set up channel status and control register
	FTM0_C6SC=FTM_CnSC_MSB_MASK|FTM_CnSC_ELSB_MASK|FTM_CnSC_ELSA_MASK;
	// Set up channel value register
	FTM0_C6V=FTM_CnV_VAL(0x00);

	// Set up pin muxes
	PORTC_PCR1=(PORTC_PCR1&~(PORT_PCR_ISF_MASK|PORT_PCR_MUX(0x03)))|PORT_PCR_MUX(0x04);
	PORTC_PCR2=(PORTC_PCR2&~(PORT_PCR_ISF_MASK|PORT_PCR_MUX(0x03)))|PORT_PCR_MUX(0x04);
	PORTD_PCR5=(PORTD_PCR5&~(PORT_PCR_ISF_MASK|PORT_PCR_MUX(0x03)))|PORT_PCR_MUX(0x04);
	PORTD_PCR6=(PORTD_PCR6&~(PORT_PCR_ISF_MASK|PORT_PCR_MUX(0x03)))|PORT_PCR_MUX(0x04);

	// Set up status and control register
	FTM0_SC=FTM_SC_CLKS(0x01)|FTM_SC_PS(0x01);
}

void PWM_SetRatio(uint8_t Channel, uint16_t Ratio)
{
	uint16_t Period, Duty;

	Period=FTM0_MOD+1;

	if(Period==0)
		Duty=Ratio;
	else
		Duty=((Period*Ratio)+0x8000)>>0x10;

	FTM0_CnV(Channel)=Duty;
}
