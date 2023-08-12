// TeensyCNC
// Copyright 2016 Matt Williams
//
// Motor PID control and encoder handling

// Alun Jones - Interrupt disable/enable while enabling/disabling motor, was causing MCU crashing.

#include "MIMXRT1062.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
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
Teensy 3.2:
0x0040 = 0000 0000 0100 0000 = PORT C6 (encoder xA, pin 11)
0x0080 = 0000 0000 1000 0000 = PORT C7 (encoder xB, pin 12)

0x0001 = 0000 0000 0000 0001 = PORT B0 (encoder yA, pin 16)
0x0002 = 0000 0000 0000 0010 = PORT B1 (encoder yB, pin 17)

Teensy 4.0:
GPIO7 0x00000004 = 0000 0000 0000 0000 0000 0000 0000 0100 = GPIO7/2 (encoder Xa, pin 11, GPIO_B0_02)
GPIO7 0x00000002 = 0000 0000 0000 0000 0000 0000 0000 0010 = GPIO7/1 (encoder Xb, pin 12, GPIO_B0_01)

GPIO6 0x00400000 = 0000 0000 0100 0000 0000 0000 0000 0000 = GPIO6/22 (encoder Ya, pin 17, GPIO_AD_B1_06)
GPIO6 0x00800000 = 0000 0000 1000 0000 0000 0000 0000 0000 = GPIO6/23 (encoder Yb, pin 16, GPIO_AD_B1_07)
*/

// Slots encoder pin status into a bit field, as a look up into Quad_Table for quadrature directional information
// Returns 0, 1, 2, or 3, depending on which opto sensor is blocked and when.
#define XENCODER_GET_PINS() (((GPIO7->DR&0x00000004)>>0x01)|((GPIO7->DR&0x00000002)>>0x01))
#define YENCODER_GET_PINS() (((GPIO6->DR&0x00400000)>>0x15)|((GPIO6->DR&0x00800000)>>0x17))

// Current encoder quadratic value
uint8_t EncoderQuad[2];
// Last quadratic value
uint8_t EncoderPrevQuad[2];

volatile int32_t Target[2]={ 0, 0 }; // Encoder coords to target (these do the moving)
volatile int32_t EncoderPos[2]={ 0, 0 }; // Actual encoder tracking coords

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

void GPIO6_7_8_9_IRQHandler(void)
{
	int8_t new_step;
	uint8_t c12;

	// X encoder interrupt
	// Check for interrupt flag for either input
	if(GPIO7->ISR&0x00000006)
	{
		// Clear the flag(s)
		GPIO7->ISR=0x00000006;

		// Get the encoder status
		c12=XENCODER_GET_PINS();
		// Retrieve directional data from quadrature lookup table
		new_step=Quad_Table[EncoderPrevQuad[0]][EncoderQuad[0]][c12];
		// Store the previous, last value
		EncoderPrevQuad[0]=EncoderQuad[0];
		// Store the current, last value
		EncoderQuad[0]=c12;

		if(new_step==3) { } // 3 is an error
		else if(new_step!=0) // It's good?
			EncoderPos[0]+=new_step; // Count it in whatever direction it's going
	}

	// Y encoder interrupt, exactly as X axis
	if(GPIO6->ISR&0x00C00000)
	{
		GPIO6->ISR=0x00C00000;

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

char buf[255];

void PIT_IRQHandler(void)
{
	// Is the interrupt flag pending?
	if(PIT->CHANNEL[2].TFLG&PIT_TFLG_TIF_MASK)
	{
		// Clear flag
		PIT->CHANNEL[2].TFLG=PIT_TFLG_TIF_MASK;

		// TESTING: Making sure that the quadrature encoders are counting as they should
		sprintf(buf, "X: %ld Y: %ld\r", EncoderPos[0], EncoderPos[1]);
		cdc_print(buf);
#if 0
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
#endif
	}
}

// Enables PID interrupt timer.
void MotorEnable(void)
{
	lastError[0]=0;
	lastError[1]=0;

	__disable_irq();
	PIT->CHANNEL[2].TCTRL|=PIT_TCTRL_TEN_MASK;
	__enable_irq();
}

// Disables PID interrupt timer.
// Also sets axis motors to 0 PWM.
void MotorDisable(void)
{
	__disable_irq();
	PIT->CHANNEL[2].TCTRL&=~PIT_TCTRL_TEN_MASK;
	__enable_irq();

	MotorCtrlX(0);
	MotorCtrlY(0);
}

void Motor_Init(void)
{
	// Initialize encoder inputs with interrupts on both edges:
	// GPIO6 0x00400000 = 0000 0000 0100 0000 0000 0000 0000 0000 = GPIO6/22 (encoder Ya, pin 17, GPIO_AD_B1_06)
	IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_06]=IOMUXC_SW_MUX_CTL_PAD_MUX_MODE(5)|IOMUXC_SW_PAD_CTL_PAD_SPEED(3)|IOMUXC_SW_PAD_CTL_PAD_PUS(3);	// ALT5 (GPIO), max speed, 100k pullup
	GPIO6->EDGE_SEL|=0x00400000; // Set interrupt rising and falling edge
	GPIO6->IMR|=0x00400000; // Enable interrupt
	GPIO6->GDIR&=~0x00400000; // Set direction to input

	// GPIO6 0x00800000 = 0000 0000 1000 0000 0000 0000 0000 0000 = GPIO6/23 (encoder Yb, pin 16, GPIO_AD_B1_07)
	IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_07]=IOMUXC_SW_MUX_CTL_PAD_MUX_MODE(5)|IOMUXC_SW_PAD_CTL_PAD_SPEED(3)|IOMUXC_SW_PAD_CTL_PAD_PUS(3);	// ALT5 (GPIO), max speed, 100k pullup
	GPIO6->EDGE_SEL|=0x00800000; // Set interrupt rising and falling edge
	GPIO6->IMR|=0x00800000; // Enable interrupt
	GPIO6->GDIR&=~0x00800000; // Set direction to input

	// GPIO7 0x00000004 = 0000 0000 0000 0000 0000 0000 0000 0100 = GPIO7/2 (encoder Xa, pin 11, GPIO_B0_02)
	IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_B0_02]=IOMUXC_SW_MUX_CTL_PAD_MUX_MODE(5)|IOMUXC_SW_PAD_CTL_PAD_SPEED(3)|IOMUXC_SW_PAD_CTL_PAD_PUS(3);	// ALT5 (GPIO), max speed, 100k pullup
	GPIO7->EDGE_SEL|=0x00000004; // Set interrupt rising and falling edge
	GPIO7->IMR|=0x00000004; // Enable interrupt
	GPIO7->GDIR&=~0x00000004; // Set direction to input

	// GPIO7 0x00000002 = 0000 0000 0000 0000 0000 0000 0000 0010 = GPIO7/1 (encoder Xb, pin 12, GPIO_B0_01)
	IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_B0_01]=IOMUXC_SW_MUX_CTL_PAD_MUX_MODE(5)|IOMUXC_SW_PAD_CTL_PAD_SPEED(3)|IOMUXC_SW_PAD_CTL_PAD_PUS(3);	// ALT5 (GPIO), max speed, 100k pullup
	GPIO7->EDGE_SEL|=0x00000002; // Set interrupt rising and falling edge
	GPIO7->IMR|=0x00000002; // Enable interrupt
	GPIO7->GDIR&=~0x00000002; // Set direction to input

	// Set interrupt priority
	NVIC_SetPriority(GPIO6_7_8_9_IRQn, 0x50);
	// Enable interrupt
	NVIC_EnableIRQ(GPIO6_7_8_9_IRQn);

	// Initialize interrupt timer for PID control:
	// Enable PIT clock gate
	CCM->CCGR1|=CCM_CCGR1_CG6_MASK;

	// Load value for timer, 17 clocks @ 66MHz
	PIT->CHANNEL[2].LDVAL=66000000/4;
	// Enable interrupts for the timer
	PIT->CHANNEL[2].TCTRL|=PIT_TCTRL_TIE_MASK;

	// Set priority and enable PIT interrupt
	NVIC_SetPriority(PIT_IRQn, 0x10);
	NVIC_EnableIRQ(PIT_IRQn);

	// Initialize encoder variables
	EncoderQuad[0]=XENCODER_GET_PINS();
	EncoderPrevQuad[0]=EncoderQuad[0];

	EncoderQuad[1]=YENCODER_GET_PINS();
	EncoderPrevQuad[1]=EncoderQuad[1];

	MotorEnable();
}
