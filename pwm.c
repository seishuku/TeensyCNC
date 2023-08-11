// TeensyCNC
// Copyright 2016 Matt Williams
//
// Motor PWM channel setup and duty cycle setting

#include "MIMXRT1062.h"
#include "pwm.h"

void PWM_Init(void)
{
	// Enable module clock
	SIM->SCGC6|=SIM_SCGC6_FTM0_MASK;

	// Set up mode register
	FTM0->MODE=FTM_MODE_FAULTM(0x00)|FTM_MODE_WPDIS_MASK;

	// Clear status and control register
	FTM0->SC=FTM_SC_CLKS(0x00)|FTM_SC_PS(0x00);

	// Clear counter initial register
	FTM0->CNTIN=FTM_CNTIN_INIT(0x00);
	// Reset counter register
	FTM0->CNT=FTM_CNT_COUNT(0x00);

	// Clear channel status and control registers
	FTM0->CONTROLS[0].CnSC=0x00U;
	FTM0->CONTROLS[1].CnSC=0x00U;
	FTM0->CONTROLS[2].CnSC=0x00U;
	FTM0->CONTROLS[3].CnSC=0x00U;
	FTM0->CONTROLS[4].CnSC=0x00U;
	FTM0->CONTROLS[5].CnSC=0x00U;
	FTM0->CONTROLS[6].CnSC=0x00U;
	FTM0->CONTROLS[7].CnSC=0x00U;

	// Set up modulo register
	// Bus clock / FTM0_MOD = PWM Freq
	// Bus clock / PWM Freq = FTM0_MOD
	// 36MHz / 1440 = 25KHz (0.04ms)
	// 36MHz / 900 = 40KHz (0.025ms)
	// 36MHz / 288 = 125KHz (0.008ms)
	FTM0->MOD=FTM_MOD_MOD(900-1);

	// Set up channel status and control register
	FTM0->CONTROLS[0].CnSC=FTM_CnSC_MSB_MASK|FTM_CnSC_ELSB_MASK|FTM_CnSC_ELSA_MASK;
	// Set up channel value register
	FTM0->CONTROLS[0].CnV=FTM_CnV_VAL(0x00);

	// Set up channel status and control register
	FTM0->CONTROLS[1].CnSC=FTM_CnSC_MSB_MASK|FTM_CnSC_ELSB_MASK|FTM_CnSC_ELSA_MASK;
	// Set up channel value register
	FTM0->CONTROLS[1].CnV=FTM_CnV_VAL(0x00);

	// Set up channel status and control register
	FTM0->CONTROLS[5].CnSC=FTM_CnSC_MSB_MASK|FTM_CnSC_ELSB_MASK|FTM_CnSC_ELSA_MASK;
	// Set up channel value register
	FTM0->CONTROLS[5].CnV=FTM_CnV_VAL(0x00);

	// Set up channel status and control register
	FTM0->CONTROLS[6].CnSC=FTM_CnSC_MSB_MASK|FTM_CnSC_ELSB_MASK|FTM_CnSC_ELSA_MASK;
	// Set up channel value register
	FTM0->CONTROLS[6].CnV=FTM_CnV_VAL(0x00);

	// Set up pin muxes
	PORTC->PCR[1]=(PORTC->PCR[1]&~(PORT_PCR_ISF_MASK|PORT_PCR_MUX(0x03)))|PORT_PCR_MUX(0x04);
	PORTC->PCR[2]=(PORTC->PCR[2]&~(PORT_PCR_ISF_MASK|PORT_PCR_MUX(0x03)))|PORT_PCR_MUX(0x04);
	PORTD->PCR[5]=(PORTD->PCR[5]&~(PORT_PCR_ISF_MASK|PORT_PCR_MUX(0x03)))|PORT_PCR_MUX(0x04);
	PORTD->PCR[6]=(PORTD->PCR[6]&~(PORT_PCR_ISF_MASK|PORT_PCR_MUX(0x03)))|PORT_PCR_MUX(0x04);

	// Set up status and control register
	FTM0->SC=FTM_SC_CLKS(0x01)|FTM_SC_PS(0x01);
}

void PWM_SetRatio(uint8_t Channel, uint16_t Ratio)
{
	uint16_t Period, Duty;

	Period=FTM0->MOD+1;

	if(Period==0)
		Duty=Ratio;
	else
		Duty=((Period*Ratio)+0x8000)>>0x10;

	FTM0->CONTROLS[Channel].CnV=Duty;
}
