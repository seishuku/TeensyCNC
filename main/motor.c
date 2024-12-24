// TeensyCNC
// Copyright 2024 Matt Williams
//
// Motor PID control and encoder handling

#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include "pwm.h"

#ifndef clamp
#define clamp(a, min, max) (((a)<(min))?(min):(((a)>(max))?(max):(a)))
#endif

static esp_timer_handle_t PIDtimer=NULL;

static const int8_t Quad_Table[4][4][4]=
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

// Slots encoder pin status into a bit field, as a look up into Quad_Table for quadrature directional information
// Returns 0, 1, 2, or 3, depending on which opto sensor is blocked and when.

// GPIO_X_INPUT_PINS = 0b0000000000000000000000000000000000110000
// GPIO_X_INPUT_PINS = 0b0000000000000000000000110000000000000000

#define GPIO_X_INPUT_PINS ((1ULL<<4)|(1ULL<<5))
#define GPIO_Y_INPUT_PINS ((1ULL<<16)|(1ULL<<17))

static uint8_t XENCODER_GET_PINS(void)
{
	return gpio_get_level(GPIO_NUM_4)|(gpio_get_level(GPIO_NUM_5)<<1);
}

static uint8_t YENCODER_GET_PINS(void)
{
	return gpio_get_level(GPIO_NUM_16)|(gpio_get_level(GPIO_NUM_17)<<1);
}

// Current encoder quadratic value
static uint8_t EncoderQuad[2];
// Last quadratic value
static uint8_t EncoderPrevQuad[2];

volatile int32_t Target[2]={ 0, 0 }; // Encoder coords to target (these do the moving)
volatile int32_t EncoderPos[2]; // Actual encoder tracking coords

// Set X axis motor PWM, neg values run opposite direction
void MotorCtrlX(int32_t PWM)
{
	if(PWM>0)
	{
		PWM_SetRatio(0, clamp(abs(PWM), 0, PWM_MAX_RATIO));
		PWM_SetRatio(1, 0);
	}
	else
	{
		PWM_SetRatio(0, 0);
		PWM_SetRatio(1, clamp(abs(PWM), 0, PWM_MAX_RATIO));
	}
}

// Same, but Y axis
void MotorCtrlY(int32_t PWM)
{
	if(PWM>0)
	{
		PWM_SetRatio(2, clamp(abs(PWM), 0, PWM_MAX_RATIO));
		PWM_SetRatio(3, 0);
	}
	else
	{
		PWM_SetRatio(2, 0);
		PWM_SetRatio(3, clamp(abs(PWM), 0, PWM_MAX_RATIO));
	}
}

// X encoder interrupt
static void IRAM_ATTR Xencoder_isr_callback(void *arg)
{
	uint32_t gpio_num=(uint32_t)arg;

	// Get the encoder status
	uint8_t c12=XENCODER_GET_PINS();
	// Retreive directional data from quadrature lookup table
	// Uses
	int8_t new_step=Quad_Table[EncoderPrevQuad[0]][EncoderQuad[0]][c12];
	// Store the previous, last value
	EncoderPrevQuad[0]=EncoderQuad[0];
	// Store the current, last value
	EncoderQuad[0]=c12;

	if(new_step==3) { } // 3 is an error
	else if(new_step!=0) // It's good?
		EncoderPos[0]+=new_step; // Count it in whatever direction it's going
}

// Y encoder interrupt, exactly as X axis
static void IRAM_ATTR Yencoder_isr_callback(void *arg)
{
	uint32_t gpio_num=(uint32_t)arg;

	uint8_t c12=YENCODER_GET_PINS();
	int8_t new_step=Quad_Table[EncoderPrevQuad[1]][EncoderQuad[1]][c12];
	
	EncoderPrevQuad[1]=EncoderQuad[1];
	EncoderQuad[1]=c12;

	if(new_step==3)
	{
	}
	else if(new_step!=0&&new_step<3)
		EncoderPos[1]+=new_step;
}

// PID stuff

//Position multiplier
#define KP 5000.0f
// Derivative multiplier
#define KD 24000.0f

// Previous derivative error
int32_t lastError[2]={ 0, 0 };

void PID_callback(void)
{
	// Run proportional control
	// find the error term of current position - target
	int32_t error[2]=
	{
		Target[0]-EncoderPos[0],
		Target[1]-EncoderPos[1]
	};

	//generalized PID formula
	//correction = Kp * error + Kd * (error - prevError)
	// MotorCtrlX(KP*error[0]+KD*(error[0]-lastError[0]));
	// MotorCtrlY(KP*error[1]+KD*(error[1]-lastError[1]));

	// Store pervious error
	lastError[0]=error[0];
	lastError[1]=error[1];

	ESP_EARLY_LOGI("TeensyCNC", "%d %d", error[0], error[1]);
}

static bool motorEnabled=false;

// Sets PID interrupt to system clock, enabling it.
void MotorEnable(void)
{
	lastError[0]=0;
	lastError[1]=0;

    esp_timer_start_periodic(PIDtimer, 100000);
}

// Removes clock source from PID interrupt timer, disabling it.
// Also sets axis motors to 0 PWM.
void MotorDisable(void)
{
	MotorCtrlX(0);
	MotorCtrlY(0);

	esp_timer_stop(PIDtimer);
}

void Motor_Init(void)
{
	gpio_install_isr_service(0);

	// Initialize enocder inputs with interrupts on both edges
	ESP_ERROR_CHECK(gpio_config(&(gpio_config_t) {
		.pin_bit_mask=GPIO_X_INPUT_PINS,
    	.mode=GPIO_MODE_INPUT,
		.pull_up_en=GPIO_PULLUP_DISABLE,
		.pull_down_en=GPIO_PULLDOWN_DISABLE,
		.intr_type=GPIO_INTR_ANYEDGE,
	}));
    gpio_isr_handler_add(4, Xencoder_isr_callback, NULL);
    gpio_isr_handler_add(5, Xencoder_isr_callback, NULL);

	ESP_ERROR_CHECK(gpio_config(&(gpio_config_t) {
		.pin_bit_mask=GPIO_Y_INPUT_PINS,
    	.mode=GPIO_MODE_INPUT,
		.pull_up_en=GPIO_PULLUP_DISABLE,
		.pull_down_en=GPIO_PULLDOWN_DISABLE,
		.intr_type=GPIO_INTR_ANYEDGE,
	}));
    gpio_isr_handler_add(16, Yencoder_isr_callback, NULL);
    gpio_isr_handler_add(17, Yencoder_isr_callback, NULL);

	// Initialize encoder variables
	EncoderQuad[0]=XENCODER_GET_PINS();
	EncoderPrevQuad[0]=EncoderQuad[0];

	EncoderQuad[1]=YENCODER_GET_PINS();
	EncoderPrevQuad[1]=EncoderQuad[1];

    ESP_ERROR_CHECK(esp_timer_create(&(esp_timer_create_args_t) {
        .callback=PID_callback,
        .name = "PIDTimer"
    }, &PIDtimer));
}
