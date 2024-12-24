// TeensyCNC
// Copyright 2024 Matt Williams
//
// Motor PWM channel setup and duty cycle setting for ESP32/FreeRTOS
// Uses ESP32 LED PWM driver for generic PWM outputs

#include <stdint.h>
#include <stdbool.h>
#include "driver/ledc.h"
#include "pwm.h"

static ledc_channel_config_t PWM_Channel[4]=
{
    { .channel=LEDC_CHANNEL_0, .duty=0, .gpio_num=22, .speed_mode=LEDC_HIGH_SPEED_MODE, .hpoint=0, .timer_sel=LEDC_TIMER_0, .flags.output_invert=0 },
    { .channel=LEDC_CHANNEL_1, .duty=0, .gpio_num=19, .speed_mode=LEDC_HIGH_SPEED_MODE, .hpoint=0, .timer_sel=LEDC_TIMER_0, .flags.output_invert=0 },
    { .channel=LEDC_CHANNEL_2, .duty=0, .gpio_num=23, .speed_mode=LEDC_HIGH_SPEED_MODE, .hpoint=0, .timer_sel=LEDC_TIMER_0, .flags.output_invert=0 },
    { .channel=LEDC_CHANNEL_3, .duty=0, .gpio_num=18, .speed_mode=LEDC_HIGH_SPEED_MODE, .hpoint=0, .timer_sel=LEDC_TIMER_0, .flags.output_invert=0 },
};

void PWM_Init(void)
{
    ledc_timer_config(&(ledc_timer_config_t)
    {
        .duty_resolution=LEDC_TIMER_10_BIT,
        .freq_hz=40000,
        .speed_mode=LEDC_HIGH_SPEED_MODE,
        .timer_num=LEDC_TIMER_0,
        .clk_cfg=LEDC_AUTO_CLK,
    });

    for(uint32_t i=0;i<4;i++)
        ledc_channel_config(&PWM_Channel[i]);
}

void PWM_SetRatio(uint8_t Channel, uint16_t Ratio)
{
    ledc_set_duty(PWM_Channel[Channel].speed_mode, PWM_Channel[Channel].channel, Ratio);
    ledc_update_duty(PWM_Channel[Channel].speed_mode, PWM_Channel[Channel].channel);
}
