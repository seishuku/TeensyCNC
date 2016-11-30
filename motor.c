// TeensyCNC
// Copyright 2016 Matt Williams
//
// Motor PID control and encoder handling

#include "mk20d7.h"
#include <stdlib.h>
#include "pwm.h"

#ifndef clamp
#define clamp(a, min, max) (((a)<(min))?(min):(((a)>(max))?(max):(a)))
#endif

const int8_t Quad_Table[4][4][4]=
{
	{
		{ 0, 1,-1, 3 },
		{-1, 0, 2, 1 },
		{ 1,-2, 0,-1 },
		{ 3,-1, 1, 0 }
	},
	{
		{ 0, 1,-1,-2 },
		{-1, 0, 3, 1 },
		{ 1, 3, 0,-1 },
		{ 2,-1, 1, 0 }
	},
	{
		{ 0, 1,-1, 2 },
		{-1, 0, 3, 1 },
		{ 1, 3, 0,-1 },
		{-2,-1, 1, 0 }
	},
	{
		{ 0, 1,-1, 3 },
		{-1, 0,-2, 1 },
		{ 1, 2, 0,-1 },
		{ 3,-1, 1, 0 }
	}
};

/*
0x0040 = 0000 0000 0100 0000 = PORT C6
0x0080 = 0000 0000 1000 0000 = PORT C7

0x0001 = 0000 0000 0000 0001 = PORT B0
0x0002 = 0000 0000 0000 0010 = PORT B1
*/
// Slots encoder pin status into a bit field, as a look up into Quad_Table for quadrature directional information
// Returns 0, 1, 2, or 3, depending on which opto sensor is blocked and when.
#define XENCODER_GET_PINS() ((GPIOC_PDIR&0x00C0)>>6)
#define YENCODER_GET_PINS() ((GPIOB_PDIR&0x0003)>>0)

// Current encoder quadratic value
uint8_t EncoderQuad[2];
// Last quadratic value
uint8_t EncoderPrevQuad[2];

volatile int32_t Target[2]={ 0, 0 }; // Encoder coords to target (these do the moving)
volatile int32_t EncoderPos[2]; // Actual encoder tracking coords

// Set X axis motor PWM, neg values run opposite direction
void MotorCtrlX(int32_t PWM)
{
	if(PWM>0)
	{
		PWM_SetRatio(0x05, clamp((uint16_t)65535-abs(PWM), 0, 65535));
		PWM_SetRatio(0x06, 65535);
	}
	else
	{
		PWM_SetRatio(0x05, 65535);
		PWM_SetRatio(0x06, clamp((uint16_t)65535-abs(PWM), 0, 65535));
	}
}

// Same, but Y axis
void MotorCtrlY(int32_t PWM)
{
	if(PWM>0)
	{
		PWM_SetRatio(0x00, clamp((uint16_t)65535-abs(PWM), 0, 65535));
		PWM_SetRatio(0x01, 65535);
	}
	else
	{
		PWM_SetRatio(0x00, 65535);
		PWM_SetRatio(0x01, clamp((uint16_t)65535-abs(PWM), 0, 65535));
	}
}

// X encoder interrupt
void __attribute__ ((interrupt)) Cpu_ivINT_PORTC(void)
{
	int8_t new_step;
	uint8_t c12;

	// Check for interrupt flag for either input
	if((PORTC_PCR6&PORT_PCR_ISF_MASK)||(PORTC_PCR7&PORT_PCR_ISF_MASK))
	{
		// Clear the flag(s)
		PORTC_PCR6|=PORT_PCR_ISF_MASK;
		PORTC_PCR7|=PORT_PCR_ISF_MASK;

		// Get the encoder status
		c12=XENCODER_GET_PINS();
		// Retreive directional data from quadrature lookup table
		// Uses
		new_step=Quad_Table[EncoderPrevQuad[0]][EncoderQuad[0]][c12];
		// Store the previous, last value
		EncoderPrevQuad[0]=EncoderQuad[0];
		// Store the current, last value
		EncoderQuad[0]=c12;

		if(new_step==3) { } // 3 is an error
		else if(new_step!=0) // It's good?
			EncoderPos[0]+=new_step; // Count it in whatever direction it's going
	}
}

// Y encoder interrupt, exactly as X axis
void __attribute__ ((interrupt)) Cpu_ivINT_PORTB(void)
{
	int8_t new_step;
	uint8_t c12;

	if((PORTB_PCR0&PORT_PCR_ISF_MASK)||(PORTB_PCR1&PORT_PCR_ISF_MASK))
	{
		PORTB_PCR0|=PORT_PCR_ISF_MASK;
		PORTB_PCR1|=PORT_PCR_ISF_MASK;

		c12=YENCODER_GET_PINS();
		new_step=Quad_Table[EncoderPrevQuad[1]][EncoderQuad[1]][c12];
		EncoderPrevQuad[1]=EncoderQuad[1];
		EncoderQuad[1]=c12;

		if(new_step==3)
		{
		}
		else if(new_step!=0&&new_step<3)
			EncoderPos[1]+=new_step;
	}
}

// PID stuff

//Position multiplier
#define KP 5000.0f
// Derivative multiplier
#define KD 24000.0f

// Previous derivative error
int32_t lastError[2]={ 0, 0 };

void __attribute__ ((interrupt)) Cpu_ivINT_FTM1(void)
{
	// Is the overflow interrupt flag pending?
	if(FTM1_SC&FTM_SC_TOF_MASK)
	{
		// Clear flag
		FTM1_SC&=~FTM_SC_TOF_MASK;

		// Run proportional control
		// find the error term of current position - target
		int32_t error[2]=
		{
			Target[0]-EncoderPos[0],
			Target[1]-EncoderPos[1]
		};

		//generalized PID formula
		//correction = Kp * error + Kd * (error - prevError)
		MotorCtrlX(KP*error[0]+KD*(error[0]-lastError[0]));
		MotorCtrlY(KP*error[1]+KD*(error[1]-lastError[1]));

		// Store pervious error
		lastError[0]=error[0];
		lastError[1]=error[1];
	}
}

// Sets PID interrupt to system clock, enabling it.
void MotorEnable(void)
{
	lastError[0]=0;
	lastError[1]=0;

	FTM1_SC=(FTM1_SC&(~(FTM_SC_CLKS_MASK&FTM_SC_TOF_MASK)))|(0x08);
	FTM1_SC=FTM_SC_TOIE_MASK|FTM_SC_CLKS(0x02)|FTM_SC_PS(0x00);
}

// Removes clock source from PID interrupt timer, disabling it.
// Also sets axis motors to 0 PWM.
void MotorDisable(void)
{
	FTM1_SC=(FTM1_SC&(~(FTM_SC_CLKS_MASK&FTM_SC_TOF_MASK)))|(0x00);
	FTM1_SC=FTM_SC_TOIE_MASK|FTM_SC_CLKS(0x00)|FTM_SC_PS(0x00);
	MotorCtrlX(0);
	MotorCtrlY(0);
}

void Motor_Init(void)
{
	// Initialize enocder inputs with interrupts on both edges
	// PB0/PB1 = Y A/B encoder input
	PORTB_PCR0=(PORTB_PCR0&~(PORT_PCR_ISF_MASK|PORT_PCR_MUX(0x06)))|PORT_PCR_MUX(0x01);
	PORTB_PCR0=(PORTB_PCR0&~(PORT_PCR_IRQC(0x04)))|(PORT_PCR_ISF_MASK|PORT_PCR_IRQC(0x0B));

	PORTB_PCR1=(PORTB_PCR1&~(PORT_PCR_ISF_MASK|PORT_PCR_MUX(0x06)))|PORT_PCR_MUX(0x01);
	PORTB_PCR1=(PORTB_PCR1&~(PORT_PCR_IRQC(0x04)))|(PORT_PCR_ISF_MASK|PORT_PCR_IRQC(0x0B));

	NVICIP88=NVIC_IP_PRI88(0x50);
	NVICISER2|=NVIC_ISER_SETENA(0x01000000);

	// PC6/PC7 = X A/B encoder input
	PORTC_PCR6=(PORTC_PCR6&~(PORT_PCR_ISF_MASK|PORT_PCR_MUX(0x06)))|PORT_PCR_MUX(0x01);
	PORTC_PCR6=(PORTC_PCR6&~(PORT_PCR_IRQC(0x04)))|(PORT_PCR_ISF_MASK|PORT_PCR_IRQC(0x0B));

	PORTC_PCR7=(PORTC_PCR7&~(PORT_PCR_ISF_MASK|PORT_PCR_MUX(0x06)))|PORT_PCR_MUX(0x01);
	PORTC_PCR7=(PORTC_PCR7&~(PORT_PCR_IRQC(0x04)))|(PORT_PCR_ISF_MASK|PORT_PCR_IRQC(0x0B));

	NVICIP89=NVIC_IP_PRI89(0x50);
	NVICISER2|=NVIC_ISER_SETENA(0x02000000);

	// Initialize interrupt timer for PID control
	SIM_SCGC6|=SIM_SCGC6_FTM1_MASK;

	// Set up mode register
	FTM1_MODE=FTM_MODE_FAULTM(0x00)|FTM_MODE_WPDIS_MASK;
	// Clear status and control register
	FTM1_SC=FTM_SC_CLKS(0x00)|FTM_SC_PS(0x00);
	// Clear counter initial register
	FTM1_CNTIN=FTM_CNTIN_INIT(0x00);
	// Reset counter register
	FTM1_CNT=FTM_CNT_COUNT(0x00);
	// Clear channel status and control register
	FTM1_C0SC=0x00;
	// Clear channel status and control register
	FTM1_C1SC=0x00;

	// Set up modulo register
	// Bus clock / Freq = FTM1_MOD
	// 36MHz / Freq = FTM1_MOD
	// MOD = 9 = 4000000Hz (4Mhz)
	FTM1_MOD=FTM_MOD_MOD(9-1);

	NVICIP63=NVIC_IP_PRI63(0x10);
	NVICISER1|=NVIC_ISER_SETENA(0x80000000);

	// Set up status and control register
	FTM1_SC=FTM_SC_TOIE_MASK|FTM_SC_CLKS(0x02)|FTM_SC_PS(0x00);

	// Initialize encoder variables
	EncoderQuad[0]=XENCODER_GET_PINS();
	EncoderPrevQuad[0]=EncoderQuad[0];

	EncoderQuad[1]=YENCODER_GET_PINS();
	EncoderPrevQuad[1]=EncoderQuad[1];
}
